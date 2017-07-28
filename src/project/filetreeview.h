#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include <QTreeView>

#include "project.h"
#include "fileprojectproxymodel.h"

class FileTreeView : public QTreeView
{
    Q_OBJECT
public:
    FileTreeView(Project *project, QWidget *parent = 0);

    FileProjectProxyModel *proxy() const;

signals:
    void doubleClickedFile(QString fileName);
    void doubleClickedDir(QString dirName);

protected:
    Project *_project;
    FileProjectProxyModel *_proxy;

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
};

#endif // FILETREEVIEW_H
