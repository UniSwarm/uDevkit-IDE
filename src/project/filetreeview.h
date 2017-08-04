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

public slots:
    void selectFile(const QString &fileName);

    void setHiddenFilter(const QRegExp &regExp);
    void setHiddenFilter(const QString &pattern);
    void setShowFilter(const QRegExp &regExp);
    void setShowFilter(const QString &pattern);

signals:
    void openedFile(QString fileName);
    void closedFile(QString fileName);

protected:
    Project *_project;
    FileProjectProxyModel *_proxy;

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // FILETREEVIEW_H
