#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QAction>
#include <QTreeView>

#include "project.h"
#include "projectitemproxymodel.h"

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(Project *project, QWidget *parent = Q_NULLPTR);

    ProjectItemProxyModel *proxy() const;

public slots:
    void selectFile(const QString &fileName);

    void setHiddenFilter(const QRegExp &regExp);
    void setHiddenFilter(const QString &pattern);
    void enableHiddenFilter(bool enable = true);

    void setShowFilter(const QRegExp &regExp);
    void setShowFilter(const QString &pattern);

    void remove();
    void rename();

signals:
    void openedFile(QString fileName);
    void closedFile(QString fileName);

protected:
    Project *_project;
    ProjectItemProxyModel *_proxy;

    QAction *_removeAction;
    QAction *_fileRenameAction;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // PROJECTTREEVIEW_H
