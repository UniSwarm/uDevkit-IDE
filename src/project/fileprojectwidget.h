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

public slots:
    void selectFile(const QString &fileName);

protected slots:
    void doubleClickDir(const QString &fileName);
    void doubleClickFile(const QString &fileName);

protected:
    Project *_project;
    FileTreeView *_fileView;

    QLineEdit *_filterEdit;
};

#endif // FILEPROJECT_H
