#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QLineEdit>

#include "project.h"

#include "projecttreeview.h"

class ProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectWidget(Project *project, QWidget *parent = 0);

signals:
    void openedFile(QString fileName);
    void closedFile(QString fileName);

public slots:
    void selectFile(const QString &fileName);

protected slots:
    void openFile(const QString &fileName);
    void closeFile(const QString &fileName);

protected:
    Project *_project;
    ProjectTreeView *_fileView;

    QLineEdit *_filterEdit;
};

#endif // PROJECTWIDGET_H
