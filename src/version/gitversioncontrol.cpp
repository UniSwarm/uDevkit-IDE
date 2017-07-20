#include "gitversioncontrol.h"

#include <QDirIterator>
#include <QDebug>
#include <QProcess>
#include <QTextStream>

GitVersionControl::GitVersionControl()
    : AbstractVersionControl ()
{
    _process = nullptr;
    _indexWatcher = nullptr;
}

void GitVersionControl::reqFiles()
{
    _process = new QProcess(this);

    //connect(_process, &QProcess::finished, this, &GitVersionControl::parseModifiedFiles); // does not work...
    connect(_process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          [=](int, QProcess::ExitStatus){parseModifiedFiles();}); // but this crap is recomended

    _process->setWorkingDirectory(_path);
    _process->start("git", QStringList()<<"ls-files"<<"-m"<<".");
}

void GitVersionControl::indexCheck()
{
    _indexWatcher->addPath(_gitPath+"index");
    reqFiles();
}

void GitVersionControl::parseModifiedFiles()
{
    QSet<QString> modifiedFiles;
    if (_process->exitStatus() == QProcess::NormalExit)
    {
        QTextStream stream(_process);
        while (!stream.atEnd())
            modifiedFiles.insert(_path+"/"+stream.readLine(1000));
    }
    _process->deleteLater();
    _process = nullptr;

    QSet<QString> newmodifiedFiles = _modifiedFiles;
    newmodifiedFiles.subtract(modifiedFiles);

    QSet<QString> validedFile = modifiedFiles;
    validedFile.subtract(_modifiedFiles);

    /*qDebug()<<newmodifiedFiles;
    qDebug()<<validedFile;
    qDebug()<<"";
    emit bla();*/
    _modifiedFiles = modifiedFiles;
}

QString GitVersionControl::findGitDir()
{
    QDir dir(_path);
    do
    {
        if (dir.exists(".git"))
            return dir.path() + "/.git/";
    } while(dir.cdUp());
    return QString();
}

void GitVersionControl::analysePath()
{
    _gitPath = findGitDir();
    if (_gitPath.isEmpty())
        return;

    delete _indexWatcher;
    _indexWatcher = new QFileSystemWatcher();
    _indexWatcher->addPath(_gitPath+"index");
    connect(_indexWatcher, &QFileSystemWatcher::fileChanged, this, &GitVersionControl::indexCheck);
    reqFiles();
}
