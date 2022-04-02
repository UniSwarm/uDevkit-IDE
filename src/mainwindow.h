/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QTextEdit>

#include "editor/editortabwidget.h"

#include "project/projectwidget.h"

#include "ui/logwidget.h"
#include "ui/searchreplacewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Project *project = nullptr, QWidget *parent = nullptr);
    ~MainWindow() override;

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
    bool openDir(const QString &paths = QString());
    bool openFiles(const QStringList &paths = QStringList());

protected slots:
    void git();
    void makeall();
    void makeprog();
    void makeclean();
    void makesim();

    void launchFormat();

    void updateTitle(Editor *editor = nullptr);
    void updateAction(Editor *editor = nullptr);
    void updateStatus(const QString &status);

    void showSettings();

    void openRecentFile();

    void about();

    // QObject interface
public:
    bool event(QEvent *event) override;

private:
    static const int MaxOldProject;
    QList<QString> _oldProjects;
    QList<QAction *> _oldProjectsActions;

    QAction *_pasteAction;
    QAction *_searchAction;
    QAction *_replaceAction;
};

#endif // MAINWINDOW_H
