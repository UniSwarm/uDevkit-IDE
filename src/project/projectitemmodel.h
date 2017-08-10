#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>
#include <QFileIconProvider>

#include "projectitem.h"

class ProjectItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProjectItemModel(Project *project);
    ~ProjectItemModel();

    void addFileItem(const QString &path);
    void addRealDirItem(const QString &path);
    void addLogicDirItem(const QString &name);
    void addItem(ProjectItem *item);

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    ProjectItem *_root;
    Project *_project;
    QFileIconProvider *_iconProvider;
};

#endif // PROJECTMODEL_H
