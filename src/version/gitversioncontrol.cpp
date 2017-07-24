#include "gitversioncontrol.h"

#include <QDirIterator>
#include <QDebug>
#include <QProcess>
#include <QTextStream>

GitVersionControl::GitVersionControl()
    : AbstractVersionControl ()
{
    _indexWatcher = nullptr;
    _state = None;

    _processGit = new QProcess(this);
    //connect(_process, &QProcess::finished, this, &GitVersionControl::parseModifiedFiles); // does not work...
    connect(_processGit, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){processEnd();}); // but this crap is recomended
}

GitVersionControl::~GitVersionControl()
{
    _processGit->deleteLater();
}

void GitVersionControl::reqModifFiles()
{
    _state = ModifiedFiles;
    _processGit->start("git", QStringList()<<"ls-files"<<"-m"<<".");
}

void GitVersionControl::reqTrackedFiles()
{
    _state = TrackedFiles;
    _processGit->start("git", QStringList()<<"ls-files"<<".");
}

void GitVersionControl::indexCheck()
{
    _indexWatcher->addPath(_gitPath+"index");
    reqModifFiles();
}

void GitVersionControl::processEnd()
{
    QSet<QString> newmodifiedFiles, validedFile;

    switch (_state) {
    case GitVersionControl::None:
        break;
    case GitVersionControl::ModifiedFiles:
        parseFilesList(_modifiedFiles, validedFile, newmodifiedFiles);
        reqTrackedFiles();
        break;
    case GitVersionControl::TrackedFiles:
        parseFilesList(_trackedFiles, validedFile, newmodifiedFiles);
        break;
    case GitVersionControl::DiffFile:
        break;

    }

    if (!newmodifiedFiles.isEmpty())
        emit newModifiedFiles(newmodifiedFiles);
    if (!validedFile.isEmpty())
        emit newValidatedFiles(validedFile);
}

void GitVersionControl::parseFilesList(QSet<QString> &oldSed, QSet<QString> &outgoingFiles, QSet<QString> &incomingFiles)
{
    QSet<QString> modifiedFiles;
    if (_processGit->exitStatus() == QProcess::NormalExit)
    {
        QTextStream stream(_processGit);
        while (!stream.atEnd())
            modifiedFiles.insert(_path+"/"+stream.readLine(1000));
    }
    if (!oldSed.isEmpty())
    {
        outgoingFiles = oldSed;
        outgoingFiles.subtract(modifiedFiles);

        incomingFiles = modifiedFiles;
        incomingFiles.subtract(oldSed);
    }

    oldSed = modifiedFiles;
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
    _processGit->setWorkingDirectory(_path);
    connect(_indexWatcher, &QFileSystemWatcher::fileChanged, this, &GitVersionControl::indexCheck);
    reqModifFiles();
}
