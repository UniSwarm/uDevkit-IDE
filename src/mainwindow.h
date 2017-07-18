#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemModel>
#include <QTreeView>

#include <QMainWindow>
#include "edbee/texteditorwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    int openFile(const QString &fileName);
    void openIndex(const QModelIndex &index);

protected:
    edbee::TextEditorWidget* _editorWidget;
    QTreeView *_fileView;
    QFileSystemModel *_fileItemModel;
};

#endif // MAINWINDOW_H
