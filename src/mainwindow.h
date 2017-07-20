#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QDockWidget>
#include <QProcess>

#include "project/fileprojectwidget.h"
#include "editor/codeeditor.h"
#include "project/project.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    Project *_projet;

    FileProjectWidget *_fileView;
    CodeEditor *_editorWidget;

    QDockWidget *_dockLog;
    QTextEdit *_logWidget;

    QProcess *_process;

protected slots:
    void git();
    void readProcess();
};

#endif // MAINWINDOW_H
