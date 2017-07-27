#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <QStack>
#include <QTabWidget>

#include "editor.h"

class EditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    EditorTabWidget();

    void addEditor(Editor *editor);
    Editor *currentEditor() const;

public slots:
    void addFileEditor(const QString &filePath);

    void closeEditor(int id=-1);
    void saveEditor();

    void nextTab();
    void previousTab();

protected slots:
    void updateTab();
    void activeTab(int id);

protected:
    bool eventFilter(QObject *o, QEvent *e);
    void registerAction();

    QStack<int> _activedTab;
};

#endif // EDITORTABWIDGET_H
