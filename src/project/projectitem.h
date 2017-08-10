#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QObject>
#include <QList>
#include <QFileSystemWatcher>

class Project;

#include "fileprojectinfo.h"

class ProjectItem : public QObject
{
    Q_OBJECT
public:
    enum Type {
        RealDir,
        LogicDir,
        File
    };

    explicit ProjectItem(Project *project, const QString path, Type type=File);
    ~ProjectItem();

    int count() const;
    ProjectItem *child(int row) const;
    int row() const;
    void addChild(ProjectItem *child);
    Type type() const;

    void addFileItem(const QString &path);
    void addRealDirItem(const QString &path);
    void addLogicDirItem(const QString &name);

    QVariant data(int column, int role) const;
    ProjectItem *parentItem() const;
    const FileProjectInfo &info() const;

signals:

public slots:

protected slots:
    void updateModif(const QString &path);

protected:
    Type _type;
    FileProjectInfo _info;
    QList<ProjectItem*> _childrens;
    ProjectItem *_parentItem;

    QFileSystemWatcher *_watcher;
};

#endif // PROJECTITEM_H
