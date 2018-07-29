#include "makeparser.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSet>
#include <QTextStream>
#include <QStandardPaths>

#include "settings/settingsmanager.h"

MakeParser::MakeParser(const QString &basePath)
    : QObject(nullptr)
{
    _processMake = new QProcess(this);
    connect(_processMake, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){processEnd();});
    //connect(_processMake, &QProcess::readyReadStandardOutput, this, &MakeParser::processEnd);
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
        return;

    _sourceFiles.clear();
    _vpath.clear();
    _basePath = QDir(basePath).canonicalPath();
    _makefileFilePath = QDir::cleanPath(_basePath+"/Makefile");
    _processMake->setWorkingDirectory(_basePath);

    analyseMakefile(_makefileFilePath);
}

QString MakeParser::resolveFilePath(const QString &filePath)
{
    QFileInfo info(filePath);

    if (QFile(_basePath+"/"+filePath).exists())
        return QDir::cleanPath(_basePath+"/"+filePath);

    QString suffixe = info.suffix();
    foreach (QString path, _vpath.values(suffixe))
        if (QFile(path+"/"+filePath).exists())
            return QDir::cleanPath(path+"/"+filePath);
    return QString();
}

void MakeParser::processEnd()
{
    //qDebug()<<QStandardPaths::findExecutable("make");
    if (_processMake->exitStatus() != QProcess::NormalExit)
        return;

    QRegExp regVar(" *([A-Za-z0-9\\\\\\/.\\_\\-]+) *:*= *");
    _vpath.clear();
    _variables.clear();

    QDir makeDir(_basePath);
    QTextStream stream(_processMake);
    while (!stream.atEnd())
    {
        QString line = stream.readLine(10000);
        if (line.startsWith('#') || line.size() == 0) // comments
            continue;

        if (regVar.indexIn(line) == 0) // variable
        {
            int pos = regVar.matchedLength();

            // TODO add a state machine parser for parenthesys matching
            //QRegExp regVarValue("([A-Za-z0-9\\\\\\/.\\_\\-]+|\\$\\([A-Za-z0-9\\\\\\\\(\\)/.\\_\\-]+\\)) *");
            QRegExp regVarValue("([A-Za-z0-9\\\\\\/.\\_\\-]+) *");
            while ((pos = regVarValue.indexIn(line, pos)) != -1)
            {
                QString value = regVarValue.cap(1);
                _variables.insert(regVar.cap(1), value);
                pos += regVarValue.matchedLength();
            }
            continue;
        }

        if (line.startsWith("vpath")) // vpath
        {
            QRegExp regVpath("vpath \\%\\.([a-zA-Z0-9]+) ");
            QRegExp regPath("([A-Za-z0-9\\\\\\/.\\_\\-]+):*");
            int pos = regVpath.indexIn(line);
            if (pos == 0)
            {
                pos += regVpath.matchedLength();
                QString suffixe = regVpath.cap(1);
                while ((pos = regPath.indexIn(line, pos)) != -1)
                {
                    QString path = QDir::cleanPath(makeDir.path()+"/"+regPath.cap(1));
                    _vpath.insert(suffixe, path);
                    pos += regPath.matchedLength();
                }
            }
            continue;
        }
    }

    QSet<QString> sourceFiles;
    QSet<QString> outgoingFiles, incomingFiles;

    // TODO complete with generic name of src variable name or
    //   improve with an auto detection of source
    addSource(sourceFiles, _variables.values("SRC"));
    addSource(sourceFiles, _variables.values("ARCHI_SRC"));
    addSource(sourceFiles, _variables.values("SOURCEFILES"));
    addSource(sourceFiles, _variables.values("HEADER"));
    addSource(sourceFiles, _variables.values("MAKEFILE_LIST"));
    //qDebug()<<_variables;

    outgoingFiles = _sourceFiles;
    outgoingFiles.subtract(sourceFiles);

    incomingFiles = sourceFiles;
    incomingFiles.subtract(_sourceFiles);
    _sourceFiles = sourceFiles;

    emit sourceChanged();
    if (!incomingFiles.isEmpty())
        emit sourceFilesAdded(incomingFiles);
    if (!outgoingFiles.isEmpty())
        emit sourceFilesRemoved(outgoingFiles);
}

void MakeParser::analyseMakefile(const QString path)
{
    if (path != _makefileFilePath)
        return;
    _makeWatcher->removePath(_basePath);
    if (QFile(_makefileFilePath).exists()) // TODO add a Makefile detection an -f name option in case of different file name
    {
        _processMake->start(_programPath, QStringList()<<"-pnR");
        _makeWatcher->addPath(_makefileFilePath);
    }
    else
        _makeWatcher->addPath(_basePath);
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
        _programPath = "make";
    _processMake->setProcessEnvironment(env);
}

void MakeParser::addSource(QSet<QString> &sourcesList, const QStringList &varList)
{
    foreach ( QString file, varList)
    {
        QString path = resolveFilePath(file);
        if (!path.isEmpty())
            sourcesList << path;
    }
}

QStringList MakeParser::sourceFiles() const
{
    return _sourceFiles.toList();
}

const QMultiMap<QString, QString> &MakeParser::variables() const
{
    return _variables;
}
