#include "fileprojectinfo.h"

#include "project.h"

#include <QDateTime>

FileProjectInfo::FileProjectInfo(const QString &filePath, Project *project)
    : QFileInfo(filePath), _project(project)
{
}

bool FileProjectInfo::isTracked() const
{
    if(isDir())
        return _project->versionControl()->isDirTracked(filePath());
    else
        return _project->versionControl()->isFileTracked(filePath());
}

bool FileProjectInfo::isModified() const
{
    if(isDir())
        return _project->versionControl()->isDirModified(filePath());
    else
        return _project->versionControl()->isFileModified(filePath());
}

bool FileProjectInfo::isValidated() const
{
    return _project->versionControl()->isFileValidated(filePath());
}

bool FileProjectInfo::isOpened() const
{
    return _project->isOpenedFile(filePath());
}

bool FileProjectInfo::isInProject() const
{
    return filePath().startsWith(_project->rootPath());
}

QString FileProjectInfo::humanSize() const
{
    double num = size();
    QStringList list;
    list << "kB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',0)+" "+unit;
}

QString FileProjectInfo::toolTips() const
{
    QString toolTip;
    toolTip.append("Size: ");
    toolTip.append(humanSize());
    toolTip.append("<br/>");

    toolTip.append("Modified: ");
    toolTip.append(lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    toolTip.append("<br/>");
    return toolTip;
}

Project *FileProjectInfo::project() const
{
    return _project;
}
