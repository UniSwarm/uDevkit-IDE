#include "mainwindow.h"

#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSplitter *spitter = new QSplitter(this);

    _fileView = new FileProject();
    spitter->addWidget(_fileView);

    _editorWidget = new CodeEditor();
    spitter->addWidget(_editorWidget);

    connect(_fileView, &FileProject::doubleClickFile, _editorWidget, &CodeEditor::openFile);

    spitter->setSizes(QList<int>()<<100<<500);

    setCentralWidget(spitter);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

MainWindow::~MainWindow()
{
}
