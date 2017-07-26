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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    Project *_projet;

    FileProjectWidget *_fileView;
    EditorTabWidget *_editorTabWidget;

    QDockWidget *_dockLog;
    QTextEdit *_logWidget;

    QProcess *_process;

protected slots:
    void git();
    void readProcess();
};

#endif // MAINWINDOW_H
