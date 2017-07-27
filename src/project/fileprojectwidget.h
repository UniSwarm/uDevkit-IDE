#ifndef FILEPROJECT_H
#define FILEPROJECT_H

#include <QLineEdit>
#include <QTreeView>

#include "project.h"

#include "fileprojectproxymodel.h"

class FileProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileProjectWidget(Project *project, QWidget *parent = 0);

signals:
    void doubleClickFile(QString fileName);

protected slots:
    void openIndex(const QModelIndex &index);

protected:
    Project *_project;
    QTreeView *_fileView;
    FileProjectProxyModel *_proxy;

    QLineEdit *_filterEdit;
};

#endif // FILEPROJECT_H
