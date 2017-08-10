#include "makeparser.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSet>
#include <QTextStream>

MakeParser::MakeParser(const QString &basePath)
    : QObject(nullptr)
{
    _processMake = new QProcess(this);
    connect(_processMake, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){processEnd();});
    //connect(_processMake, &QProcess::readyReadStandardOutput, this, &MakeParser::processEnd);
    setBasePath(basePath);
}

MakeParser::~MakeParser()
{
    delete _processMake;
}

void MakeParser::setBasePath(const QString &basePath)
{
    _basePath = QDir(basePath).canonicalPath();
    _processMake->setWorkingDirectory(_basePath);

    if (QFile(_basePath+"/Makefile").exists()) // TODO add a Makefile detection an -f name option in case of different file name
        _processMake->start("make", QStringList()<<"-pRnq");
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
    if (_processMake->exitStatus() != QProcess::NormalExit)
        return;

    QRegExp regVar(" *([A-Za-z0-9\\\\\\/.\\_\\-]+) *:= *");

    QDir makeDir(_basePath);
    QTextStream stream(_processMake);
    while (!stream.atEnd())
    {
        QString line = stream.readLine(1000);
        if (line.startsWith('#') || line.size() == 0) // comments
            continue;

        if (regVar.indexIn(line) == 0) // variable
        {
            int pos = regVar.matchedLength();

            QRegExp regVarValue("([A-Za-z0-9\\\\\\/.\\_\\-]+) *");
            while ((pos = regVarValue.indexIn(line, pos)) != -1)
            {
                _variables.insert(regVar.cap(1), regVarValue.cap(1));
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

    _sourceFiles.clear();
    foreach (QString file, _variables.values("SRC")) // TODO complete with generic name of src variable name or
        _sourceFiles<<resolveFilePath(file);         //   improve with an auto detection of source
}

const QStringList &MakeParser::sourceFiles() const
{
    return _sourceFiles;
}

const QMultiMap<QString, QString> &MakeParser::variables() const
{
    return _variables;
}
