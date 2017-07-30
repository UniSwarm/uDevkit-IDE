#ifndef FILEPROJECTINFO_H
#define FILEPROJECTINFO_H

#include "project.h"

#include <QFileInfo>
#include <QString>

class FileProjectInfo : public QFileInfo
{
public:
    FileProjectInfo(const QString &filePath, Project *project);

    bool isTracked() const;
    bool isModified() const;
    bool isValidated() const;

    QString humanSize() const;
    QString toolTips() const;

protected:
    Project *_project;
};

#endif // FILEPROJECTINFO_H
