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

    _processGitDiff = new QProcess(this);
    connect(_processGitDiff, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int, QProcess::ExitStatus){processDiffEnd();}); // but this crap is recomended

    _settingsClass = SettingsManager::registerClass("gitversion");
    connect(_settingsClass, &SettingsClass::classModified, this, &GitVersionControl::updateSettings);
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

    QDir dir(_basePath);
    foreach (QString filePath, filesPath)
        args<<dir.relativeFilePath(filePath);

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

    QDir dir(_basePath);
    foreach (QString filePath, filesPath)
        args<<dir.relativeFilePath(filePath);

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

    QDir dir(_basePath);
    foreach (QString filePath, filesPath)
        args<<dir.relativeFilePath(filePath);

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

void GitVersionControl::requestFileModifications(const QString &filePath)
{
    _diffRequestQueue.enqueue(filePath);
    if (_processGitDiff->state() == QProcess::NotRunning)
        reqFileModif(_diffRequestQueue.head());
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

void GitVersionControl::updateSettings()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();;
#if defined(Q_OS_WIN)
    char listSep = ';';
#else
    char listSep = ':';
#endif
    env.insert("PATH", _settingsClass->setting("path").toString() + listSep + env.value("PATH") );
    _processGitState->setProcessEnvironment(env);
    _processGitDiff->setProcessEnvironment(env);
}

void GitVersionControl::reqFileModif(const QString &filePath)
{
    QDir dir(_basePath);
    _processGitDiff->start("git", QStringList()<<"diff"<<"--no-color"<<"--unified=0"<<dir.relativeFilePath(filePath));
}

void GitVersionControl::processDiffEnd()
{
    QTextStream stream(_processGitDiff);
    FileVersionChange fileChanges;

    QRegExp regContext("@@ -([0-9]+)(,([0-9]+))* \\+([0-9]+)(,([0-9]+))* @@");
    bool valid = false;
    VersionChange *change = new VersionChange();
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        if (line.startsWith("@@")) // new modif
        {
            if (valid && change->isValid())
            {
                fileChanges.changes().append(change);
                change = new VersionChange();
            }

            regContext.indexIn(line);
            int lineOld = regContext.cap(1).toInt();
            int lineNew = regContext.cap(4).toInt();
            change->setLineOld(lineOld);
            change->setLineNew(lineNew);
            valid = true;
        }
        if (!valid)
            continue;
        if (line.startsWith('+'))
            change->addAddedLine(line.mid(1));
        else if (line.startsWith('-'))
            change->addRemovedLine(line.mid(1));
    }
    if (valid && change->isValid())
        fileChanges.changes().append(change);
    else
        delete change;

    QString fileGitDiff = _diffRequestQueue.dequeue();
    _changeForFile[fileGitDiff] = fileChanges;
    emit fileModificationsAvailable(fileGitDiff);

    if (!_diffRequestQueue.isEmpty())
        reqFileModif(_diffRequestQueue.head());
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
    _processGitDiff->setWorkingDirectory(_basePath);
    connect(_indexWatcher, &QFileSystemWatcher::fileChanged, this, &GitVersionControl::indexCheck);

    _state = Check;
    _processGitState->start("git", QStringList()<<"status");
}
