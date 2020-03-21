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
    void saveCurrentEditor();
    void saveAllEditors();

    void switchHeader();

    void initiateSwitchTab(bool next);
    void nextTab();
    void previousTab();
    void endSwitchTab();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void format();

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
    void changeStatus(QString status);

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
    QStack<int> _activedTab;
    bool _switchTabActive;
    void updateSwitchTab();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // EDITORTABWIDGET_H
