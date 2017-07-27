#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

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

protected slots:
    void updateTab();

protected:
    bool eventFilter(QObject *o, QEvent *e);
    void registerAction();
};

#endif // EDITORTABWIDGET_H
