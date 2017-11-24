#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QTextEdit>

#include "editor/editortabwidget.h"

#include "project/projectwidget.h"

#include "ui/searchreplacewidget.h"
#include "ui/logwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Project *project = 0, QWidget *parent = 0);
    ~MainWindow();

protected:
    Project *_project;
    EditorTabWidget *_editorTabWidget;

    QDockWidget *_logDock;
    LogWidget *_logWidget;

    QDockWidget *_fileProjectDock;
    ProjectWidget *_fileProjectWidget;

    QDockWidget *_searchReplaceDock;
    SearchReplaceWidget *_searchReplaceWidget;

    void createDocks();
    void createMenus();

    void writeSettings();
    void readSettings();
    void updateOldProjects();

public slots:
    bool openDir(const QString &paths=QString());
    bool openFiles(const QStringList &paths=QStringList());

protected slots:
    void git();
    void makeall();
    void makeprog();
    void makeclean();
    void makesim();

    void updateTitle(Editor *editor=Q_NULLPTR);
    void updateStatus(QString status);

    void openRecentFile();

    void about();

    // QObject interface
public:
    virtual bool event(QEvent *event);

private:
    static const int MaxOldProject;
    QList<QString> _oldProjects;
    QList<QAction*> _oldProjectsActions;
};

#endif // MAINWINDOW_H
