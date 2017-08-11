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

    enum Role {
        FileIconRole = Qt::DecorationRole,
        FilePathRole = Qt::UserRole + 1,
        FileNameRole = Qt::UserRole + 2
    };
    const ProjectItem *item(const QModelIndex &index) const;
    //QModelIndex index(const ProjectItem *item) const;
    QModelIndex index(const QString path) const;
    bool isDir(const QModelIndex &index) const;
    bool isFile(const QModelIndex &index) const;
    QString filePath(const QModelIndex &index) const;
    QString fileName(const QModelIndex &index) const;
    bool rmdir(const QModelIndex &index);
    bool remove(const QModelIndex &index);

    // high level interface
    void addExternalSource(QSet<QString> sourceFiles);
    void removeExternalSource(QSet<QString> sourceFiles);

public slots:
    void filesUpdated(QSet<QString> filesPath);

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

protected:
    ProjectItem *_root;
    Project *_project;
    QFileIconProvider *_iconProvider;
    QMap<QString, ProjectItem*> _pathCache;

    // high level interface
    ProjectItem *_externalFiles;
};

#endif // PROJECTMODEL_H
