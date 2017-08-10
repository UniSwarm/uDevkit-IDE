#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QObject>
#include <QList>

class Project;

#include "fileprojectinfo.h"

class ProjectItem : public QObject
{
    Q_OBJECT
public:
    explicit ProjectItem(Project *project, const QString path);
    ~ProjectItem();

    int count() const;
    ProjectItem *child(int row) const;
    int row() const;
    void addChild(ProjectItem *child);

    QVariant data(int column, int role) const;

    ProjectItem *parentItem() const;

    const FileProjectInfo &info() const;

signals:

public slots:

protected:
    QList<ProjectItem*> _childrens;
    FileProjectInfo _info;
    ProjectItem *_parentItem;
};

#endif // PROJECTITEM_H
