#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>

#include "editor/editortabwidget.h"

#include "project/fileprojectwidget.h"
#include "project/project.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Project *project = 0, QWidget *parent = 0);
    ~MainWindow();

protected:
    Project *_project;
    EditorTabWidget *_editorTabWidget;

    QDockWidget *_dockLog;
    QTextEdit *_logWidget;

    QDockWidget *_dockFileProject;
    FileProjectWidget *_fileProjectWidget;

    QProcess *_process;

    void createDocks();
    void registerAction();

protected slots:
    void git();
    void readProcess();
    void updateTitle(Editor *editor);
};

#endif // MAINWINDOW_H
