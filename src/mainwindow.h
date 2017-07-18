#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "edbee/texteditorwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile(const QString &fileName);

protected:
    edbee::TextEditorWidget* _editorWidget;
};

#endif // MAINWINDOW_H
