#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QFileSystemWatcher>

class Project;
class ProjectItemModel;

#include "fileprojectinfo.h"

class ProjectItem : public QObject
{
    Q_OBJECT
public:
    enum Type {
        RealDir,
        DirFile,
        LogicDir,
        IndividualFile
    };

    explicit ProjectItem(Project *project, const QString path, Type type, ProjectItemModel *model);
    ~ProjectItem();

    int count() const;
    ProjectItem *child(int row) const;
    ProjectItem *child(const QString &name) const;
    int row() const;
    ProjectItem *parentItem() const;
    void addChild(ProjectItem *child);
    void removeChild(ProjectItem *child);
    void clear();
    Type type() const;

    void addFileItem(const QString &path);
    void addRealDirItem(const QString &path);
    void addLogicDirItem(const QString &name);

    QVariant data(int column, int role) const;

    const FileProjectInfo &info() const;
    inline QString filePath() const {return _info.filePath();}
    inline QString fileName() const {return _info.fileName();}

    /*bool haveChildren() const;
    bool canFetchMore() const;
    void fetchMore();*/

signals:

public slots:

protected slots:
    void updateModif(const QString &path=QString());

protected:
    Type _type;
    FileProjectInfo _info;
    QList<ProjectItem*> _childrens;
    QHash<QString, ProjectItem*> _childrensMap;
    ProjectItem *_parentItem;

    QFileSystemWatcher *_watcher;
    ProjectItemModel *_model;
    bool _loaded;
};

#endif // PROJECTITEM_H
