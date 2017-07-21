#ifndef FILEPROJECTITEMMODEL_H
#define FILEPROJECTITEMMODEL_H

#include <QFileSystemModel>

class Project;

class FileProjectItemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    FileProjectItemModel(Project *project);

public slots:
    void filesUpdated(QSet<QString> filesPath);

    // QFileSystemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const;

protected:
    Project *_project;
};

#endif // FILEPROJECTITEMMODEL_H
