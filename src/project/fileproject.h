#ifndef FILEPROJECT_H
#define FILEPROJECT_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QDebug>

#include "fileprojectproxymodel.h"

class FileProject : public QWidget
{
    Q_OBJECT
public:
    explicit FileProject(QWidget *parent = 0);

signals:
    void doubleClickFile(QString fileName);

protected slots:
    void openIndex(const QModelIndex &index);

protected:
    QTreeView *_fileView;
    FileProjectProxyModel *_proxy;
    QFileSystemModel *_fileItemModel;
};

#endif // FILEPROJECT_H
