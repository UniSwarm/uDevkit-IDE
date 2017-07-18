#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "project/fileproject.h"
#include "editor/codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    FileProject *_fileView;
    CodeEditor *_editorWidget;
};

#endif // MAINWINDOW_H
