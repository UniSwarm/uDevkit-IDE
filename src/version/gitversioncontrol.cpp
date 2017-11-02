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

    _processGitState = new QProcess(this);
    //connect(_process, &QProcess::finished, this, &GitVersionControl::parseModifiedFiles); // does not work...
    connect(_processGitState, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){processEnd();}); // but this crap is recomended
}

GitVersionControl::~GitVersionControl()
{
    _processGitState->deleteLater();
}

QString GitVersionControl::versionControlName() const
{
    return "git";
}

QString GitVersionControl::basePath() const
{
    return _basePath;
}

void GitVersionControl::validFile(const QSet<QString> &filesPath)
{
    if (!isValid())
        return;
    QStringList args;
    args << "add";
    args << filesPath.toList();
    QProcess *newProcess = new QProcess(this);
    newProcess->setWorkingDirectory(_basePath);
    connect(newProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){delete sender();});
    newProcess->start("git", args);
}

void GitVersionControl::inValidFile(const QSet<QString> &filesPath)
{
    if (!isValid())
        return;
    QStringList args;
    args << "reset" << "HEAD";
    args << filesPath.toList();
    QProcess *newProcess = new QProcess(this);
    newProcess->setWorkingDirectory(_basePath);
    connect(newProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){delete sender();});
    newProcess->start("git", args);
}

void GitVersionControl::checkoutFile(const QSet<QString> &filesPath)
{
    if (!isValid())
        return;
    QStringList args;
    args << "checkout";
    args << filesPath.toList();
    QProcess *newProcess = new QProcess(this);
    newProcess->setWorkingDirectory(_basePath);
    connect(newProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){delete sender();});
    newProcess->start("git", args);
}

bool GitVersionControl::isValid() const
{
    return !_gitPath.isEmpty();
}

void GitVersionControl::reqFetch()
{
    _state = Fetch;
    _processGitState->start("git", QStringList()<<"fetch");
}

void GitVersionControl::reqModifFiles()
{
    _state = ModifiedFiles;
    _processGitState->start("git", QStringList()<<"ls-files"<<"-m"<<".");
}

void GitVersionControl::reqTrackedFiles()
{
    _state = TrackedFiles;
    _processGitState->start("git", QStringList()<<"ls-files"<<".");
}

void GitVersionControl::reqValidatedFiles()
{
    _state = ValidatedFiles;
    _processGitState->start("git", QStringList()<<"diff"<<"--cached"<<"--name-only");
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
    case GitVersionControl::Fetch:
        reqModifFiles();
        break;
    case GitVersionControl::Check:
        if (_processGitState->exitCode() != 0)
            reqFetch();
        else
            reqModifFiles();
        break;
    case GitVersionControl::ModifiedFiles:
        parseFilesList(_modifiedFiles, validedFile, newmodifiedFiles);
        reqTrackedFiles();
        break;
    case GitVersionControl::TrackedFiles:
        parseFilesList(_trackedFiles, validedFile, newmodifiedFiles);
        reqValidatedFiles();
        break;
    case GitVersionControl::ValidatedFiles:
        parseFilesList(_validatedFiles, validedFile, newmodifiedFiles);
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
    if (_processGitState->exitStatus() == QProcess::NormalExit)
    {
        QTextStream stream(_processGitState);
        while (!stream.atEnd())
            modifiedFiles.insert(_basePath+"/"+stream.readLine(1000));
    }
    //if (!oldSed.isEmpty())
    {
        outgoingFiles = oldSed;
        outgoingFiles.subtract(modifiedFiles);

        incomingFiles = modifiedFiles;
        incomingFiles.subtract(oldSed);
    }

    oldSed = modifiedFiles;
}

void GitVersionControl::findGitDir()
{
    QDir dir(_path);
    do
    {
        if (dir.exists(".git"))
        {
            if (dir.exists(".git/index"))
            {
                _gitPath = dir.canonicalPath() + "/.git/";
                _basePath = dir.path();
                return;
            }
            else
            {
                QString parentPath;
                QFile gitFile(dir.canonicalPath() + "/.git");
                if (!gitFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    continue;
                QTextStream stream(&gitFile);
                QString word;
                while (!stream.atEnd())
                {
                    stream >> word;
                    if (word != "gitdir:")
                        continue;
                    stream >> parentPath;
                    if (dir.exists(parentPath))
                    {
                        _gitPath = dir.absoluteFilePath(parentPath) +"/";
                        _basePath = dir.path();
                        return;
                    }
                }
            }
        }
    } while(dir.cdUp());
}

void GitVersionControl::analysePath()
{
    findGitDir();
    if (_gitPath.isEmpty())
        return;

    delete _indexWatcher;
    _indexWatcher = new QFileSystemWatcher();
    _indexWatcher->addPath(_gitPath+"index");
    _processGitState->setWorkingDirectory(_basePath);
    connect(_indexWatcher, &QFileSystemWatcher::fileChanged, this, &GitVersionControl::indexCheck);

    _state = Check;
    _processGitState->start("git", QStringList()<<"status");
}
