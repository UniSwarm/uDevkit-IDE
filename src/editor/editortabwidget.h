#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <QStack>
#include <QMap>
#include <QTabWidget>

#include "editor.h"
#include "project/project.h"

class EditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    EditorTabWidget(Project *project);

    void addEditor(Editor *editor);
    Editor *currentEditor() const;
    Editor *editor(int i) const;

public slots:
    void openFileEditor(const QString &filePath);
    void closeFileEditor(const QString &filePath);

    int closeEditor(int id=-1);
    int closeAllEditors();
    void saveCurrentEditor();
    void saveAllEditors();

    void nextTab();
    void previousTab();

signals:
    void editorChange(Editor *editor);
    void currentEditorModified(Editor *editor);
    void currentFileChanged(QString filePath);

protected slots:
    void updateTab();
    void activeTab(int id);

protected:
    bool eventFilter(QObject *o, QEvent *e);
    void registerAction();

    Project *_project;
    QStack<int> _activedTab;
    QMap<QString, Editor* > _mapPathEditor;
};

#endif // EDITORTABWIDGET_H
