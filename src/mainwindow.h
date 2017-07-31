#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>

#include "editor/editortabwidget.h"

#include "project/fileprojectwidget.h"

#include "ui/searchreplacewidget.h"

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
    QTextEdit *_logWidget;

    QDockWidget *_fileProjectDock;
    FileProjectWidget *_fileProjectWidget;

    QDockWidget *_searchReplaceDock;
    SearchReplaceWidget *_searchReplaceWidget;

    QProcess *_process;

    void createDocks();
    void createMenus();

public slots:
    bool openDir(const QString &paths=QString());
    bool openFiles(const QStringList &paths=QStringList());

protected slots:
    void git();
    void readProcess();
    void updateTitle(Editor *editor);

    // QObject interface
public:
    virtual bool event(QEvent *event);
};

#endif // MAINWINDOW_H
