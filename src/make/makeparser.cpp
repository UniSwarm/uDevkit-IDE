#include "makeparser.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSet>
#include <QStack>
#include <QStandardPaths>
#include <QTextStream>

#include "settings/settingsmanager.h"

MakeParser::MakeParser(const QString &basePath)
    : QObject(Q_NULLPTR)
{
    _processMake = new QProcess(this);
    connect(_processMake, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=](int, QProcess::ExitStatus) { processEnd(); });
    // connect(_processMake, &QProcess::readyReadStandardOutput, this, &MakeParser::processEnd);
    setPath(basePath);

    _makeWatcher = new QFileSystemWatcher();
    connect(_makeWatcher, &QFileSystemWatcher::fileChanged, this, &MakeParser::analyseMakefile);

    _settingPath = rtset()->registerSetting("tools/make/path");
    connect(_settingPath, &Setting::valueChanged, this, &MakeParser::updateSettings);
    updateSettings();
}

MakeParser::~MakeParser()
{
    delete _processMake;
}

void MakeParser::setPath(const QString &basePath)
{
    if (basePath.isEmpty())
    {
        return;
    }

    _sourceFiles.clear();
    _vpath.clear();
    _basePath = QDir(basePath).canonicalPath();
    _makefileFilePath = QDir::cleanPath(_basePath + "/Makefile");
    _processMake->setWorkingDirectory(_basePath);

    analyseMakefile(_makefileFilePath);
}

QString MakeParser::resolveFilePath(const QString &filePath)
{
    QFileInfo info(filePath);

    if (QFile(filePath).exists())
    {
        return QDir::cleanPath(filePath);
    }
    if (QFile(_basePath + "/" + filePath).exists())
    {
        return QDir::cleanPath(_basePath + "/" + filePath);
    }

    QString suffixe = info.suffix();
    foreach (QString path, _vpath.values(suffixe))
    {
        if (QFile(path + "/" + filePath).exists())
        {
            return QDir::cleanPath(path + "/" + filePath);
        }
    }
    return QString();
}

void MakeParser::processEnd()
{
    QStack<QString> directoryStack;
    directoryStack.push(_basePath);
    QDir dir(_basePath);

    QRegExp regVar(" *([A-Za-z0-9\\\\\\/.\\_\\-]+) *:*= *");
    QRegExp regVarValue("([A-Za-z0-9\\\\\\/.\\_\\-]+) *");
    QRegExp regVpath("vpath \\%\\.([a-zA-Z0-9]+) ");
    QRegExp regPath("([A-Za-z0-9\\\\\\/.\\_\\-]+):*");
    QRegExp regRule("^([A-Za-z0-9\\\\\\/.\\_\\-]+):");
    QRegExp regEnterDir("^# make\\[1\\]: (Entering|Leaving) directory \\'(.+)\\'");
    bool notATarget = false;

    qint64 start = QDateTime::currentMSecsSinceEpoch();
    if (_processMake->exitStatus() != QProcess::NormalExit)
    {
        return;
    }

    _vpath.clear();
    _variables.clear();

    QDir makeDir(_basePath);
    QTextStream stream(_processMake);
    while (!stream.atEnd())
    {
        QString line = stream.readLine(10000);
        // qDebug()<<line;

        if (line == "# Not a target:")
        {
            notATarget = true;
            continue;
        }
        if (regEnterDir.indexIn(line) == 0) // (Entering|Leaving) directory
        {
            if (regEnterDir.cap(1) == "Entering")
            {
                directoryStack.push(regEnterDir.cap(2));
            }
            else
            {
                directoryStack.pop();
            }
            dir.setPath(directoryStack.back());
            // qDebug()<<"directory"<<dir.path();
            notATarget = true;
            continue;
        }

        if (line.startsWith('#') || line.size() == 0) // comments
        {
            notATarget = false;
            continue;
        }

        if (regVar.indexIn(line) == 0) // variable
        {
            int pos = regVar.matchedLength();

            // TODO add a state machine parser for parenthesys matching
            // QRegExp regVarValue("([A-Za-z0-9\\\\\\/.\\_\\-]+|\\$\\([A-Za-z0-9\\\\\\\\(\\)/.\\_\\-]+\\)) *");
            while ((pos = regVarValue.indexIn(line, pos)) != -1)
            {
                QString value = regVarValue.cap(1);
                _variables.insert(regVar.cap(1), value);
                pos += regVarValue.matchedLength();
            }
            notATarget = false;
            continue;
        }

        if (line.startsWith("vpath")) // vpath
        {
            int pos = regVpath.indexIn(line);
            if (pos == 0)
            {
                pos += regVpath.matchedLength();
                QString suffixe = regVpath.cap(1);
                while ((pos = regPath.indexIn(line, pos)) != -1)
                {
                    QString path = QDir::cleanPath(regPath.cap(1));
                    if (!QDir(path).isAbsolute())
                    {
                        path = QDir::cleanPath(makeDir.path() + "/" + regPath.cap(1));
                    }
                    _vpath.insert(suffixe, path);
                    pos += regPath.matchedLength();
                }
            }
            notATarget = false;
            continue;
        }

        if (regRule.indexIn(line) == 0) // variable
        {
            MakeRule rule;
            rule.target = makeDir.relativeFilePath(dir.path() + "/" + regRule.cap(1));
            rule.isTarget = !notATarget;

            int pos = regRule.matchedLength() + 1;
            while ((pos = regVarValue.indexIn(line, pos)) != -1)
            {
                QString value = regVarValue.cap(1);
                if (!QFileInfo(regVarValue.cap(1)).isAbsolute())
                {
                    value = makeDir.relativeFilePath(dir.path() + "/" + regVarValue.cap(1));
                }
                rule.dependencies.append(value);
                pos += regVarValue.matchedLength();
            }
            rule.dependencies.removeDuplicates();

            // qDebug()<<">>>" << rule.isTarget << rule.target << rule.dependencies;
            _rules.insert(rule.target, rule);
            notATarget = false;
            continue;
        }
    }
    // qDebug()<<"end"<<QDateTime::currentMSecsSinceEpoch() - start;

    QSet<QString> sourceFiles;
    QSet<QString> outgoingFiles, incomingFiles;

    // TODO complete with generic name of src variable name or
    //   improve with an auto detection of source
    addSource(sourceFiles, _variables.values("SRC"));
    addSource(sourceFiles, _variables.values("ARCHI_SRC"));
    addSource(sourceFiles, _variables.values("SOURCEFILES"));
    addSource(sourceFiles, _variables.values("HEADER"));
    addSource(sourceFiles, _variables.values("MAKEFILE_LIST"));
    // qDebug()<<_variables;

    outgoingFiles = _sourceFiles;
    outgoingFiles.subtract(sourceFiles);

    incomingFiles = sourceFiles;
    incomingFiles.subtract(_sourceFiles);
    _sourceFiles = sourceFiles;

    emit sourceChanged();
    if (!incomingFiles.isEmpty())
    {
        emit sourceFilesAdded(incomingFiles);
    }
    if (!outgoingFiles.isEmpty())
    {
        emit sourceFilesRemoved(outgoingFiles);
    }
}

void MakeParser::analyseMakefile(const QString path)
{
    if (path != _makefileFilePath)
    {
        return;
    }
    _makeWatcher->removePath(_basePath);
    if (QFile(_makefileFilePath).exists()) // TODO add a Makefile detection an -f name option in case of different file name
    {
        _processMake->start(_programPath, QStringList() << "-pnR");
        _makeWatcher->addPath(_makefileFilePath);
    }
    else
    {
        _makeWatcher->addPath(_basePath);
    }
}

void MakeParser::updateSettings()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString makePath = _settingPath->value().toString();
#if defined(Q_OS_WIN)
    QChar listSep = ';';
#else
    QChar listSep = ':';
#endif
    if (!makePath.isEmpty())
    {
        makePath = QDir::toNativeSeparators(makePath);
        env.insert("PATH", makePath + listSep + env.value("PATH"));
        _programPath = QStandardPaths::findExecutable("make", env.value("PATH").split(listSep));
    }
    else
    {
        _programPath = "make";
    }
    _processMake->setProcessEnvironment(env);
}

void MakeParser::addSource(QSet<QString> &sourcesList, const QStringList &varList)
{
    foreach (QString file, varList)
    {
        QString path = resolveFilePath(file);
        if (!path.isEmpty())
        {
            sourcesList << path;
        }
    }
}

const QMap<QString, MakeRule> &MakeParser::rules() const
{
    return _rules;
}

const MakeRule MakeParser::buildRule(const QString &filePath) const
{
    QDir makeDir(_basePath);
    QString makeFilePath = makeDir.relativeFilePath(filePath);
    foreach (const MakeRule &rule, _rules)
    {
        if (rule.dependencies.isEmpty())
        {
            continue;
        }
        if (rule.dependencies.first() == makeFilePath)
        {
            return rule;
        }
    }
    return MakeRule();
}

const QList<MakeRule> MakeParser::includedInRules(const QString &filePath) const
{
    QList<MakeRule> includedInRules;
    QDir makeDir(_basePath);
    QString makeFilePath = makeDir.relativeFilePath(filePath);
    foreach (const MakeRule &rule, _rules)
    {
        if (rule.dependencies.isEmpty())
        {
            continue;
        }
        if (rule.dependencies.contains(makeFilePath))
        {
            includedInRules.append(rule);
        }
    }
    return includedInRules;
}

const QList<MakeRule> MakeParser::targets() const
{
    QList<MakeRule> targetRules;
    foreach (const MakeRule &rule, _rules)
    {
        if (!rule.isTarget)
        {
            continue;
        }
        targetRules.append(rule);
    }
    return targetRules;
}

QStringList MakeParser::sourceFiles() const
{
    return _sourceFiles.toList();
}

const QMultiMap<QString, QString> &MakeParser::variables() const
{
    return _variables;
}
