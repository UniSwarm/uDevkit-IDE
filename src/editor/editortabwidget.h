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

#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include "editortabswitchwidget.h"
#include <QMap>
#include <QStack>
#include <QTabWidget>

#include <project/projecticonprovider.h>

#include "editor.h"
#include "project/project.h"

class EditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    EditorTabWidget(Project *project);

    void addEditor(Editor *editor);
    Editor *currentEditor() const;
    QString currentFilePath() const;
    Editor *editor(int i) const;

public slots:
    void openFileEditor(const QString &url);
    void closeFileEditor(const QString &filePath);

    int closeEditor(int id = -1);
    int closeAllEditors();
    void saveCurrentEditor() const;
    void saveAllEditors();

    void switchHeader();

    void initiateSwitchTab(bool next);
    void nextTab();
    void previousTab();
    void endSwitchTab();

    void undo() const;
    void redo() const;
    void cut() const;
    void copy() const;
    void paste() const;
    void format() const;

signals:
    void editorChange(Editor *editor);
    void currentEditorModified(Editor *editor);
    void currentFileChanged(QString filePath);
    void statusChanged(QString status);

    void undoAvailable(bool available);
    void redoAvailable(bool available);
    void copyAvailable(bool available);

protected slots:
    void updateTab();
    void activeTab(int id);
    void tabContextMenu(const QPoint &pos);
    void changeStatus(const QString &status);

    void undoUpdate(bool available);
    void redoUpdate(bool available);
    void copyUpdate(bool available);

protected:
    bool eventFilter(QObject *o, QEvent *e) override;
    void registerAction();

    Project *_project;
    QMap<QString, Editor *> _mapPathEditor;
    ProjectIconProvider *_iconProvider;
    int _idColor;

    // tab switching
    EditorTabSwitchWidget *_switchTabListWidget;
    QStack<int> _activedTabs;
    bool _switchTabActive;
    void updateSwitchTab();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif  // EDITORTABWIDGET_H
