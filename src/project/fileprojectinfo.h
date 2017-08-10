#ifndef FILEPROJECTINFO_H
#define FILEPROJECTINFO_H

#include <QFileInfo>
#include <QString>

class Project;

class FileProjectInfo : public QFileInfo
{
public:
    FileProjectInfo(const QString &filePath, Project *project);

    bool isTracked() const;
    bool isModified() const;
    bool isValidated() const;
    bool isOpened() const;
    bool isInProject() const;

    QString humanSize() const;
    QString toolTips() const;

    Project *project() const;

protected:
    Project *_project;
};

#endif // FILEPROJECTINFO_H
