#include "fileproject.h"

FileProject::FileProject(const QString &filePath, Project *project):
    _info(filePath, project), _project(project)
{
    _versionControl = Q_NULLPTR;
    _fileChange = Q_NULLPTR;
}

const FileProjectInfo &FileProject::info() const
{
    return _info;
}

QList<Editor *> FileProject::openedEditors()
{
    // TODO implement me
    return QList<Editor *>();
}

void FileProject::valid(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
        return;
    _versionControl->validFile(QSet<QString>()<<_info.filePath());
}

void FileProject::checkout(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
        return;
    _versionControl->checkoutFile(QSet<QString>()<<_info.filePath());
}

void FileProject::inValid(int line)
{
    Q_UNUSED(line)
    if (!_versionControl)
        return;
    _versionControl->inValidFile(QSet<QString>()<<_info.filePath());
}

void FileProject::updateDiff()
{
    if (!_versionControl)
        return;
    _versionControl->requestFileModifications(_info.filePath());
}
