#ifndef FILEPROJECT_H
#define FILEPROJECT_H

#include <QLineEdit>

#include "project.h"

#include "filetreeview.h"

class FileProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileProjectWidget(Project *project, QWidget *parent = 0);

signals:
    void doubleClickedFile(QString fileName);
    void doubleClickedDir(QString dirName);

protected slots:
    void doubleClickDir(QString fileName);
    void doubleClickFile(QString fileName);

protected:
    Project *_project;
    FileTreeView *_fileView;

    QLineEdit *_filterEdit;
};

#endif // FILEPROJECT_H
