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

public slots:
    void addFileEditor(const QString &filePath);
};

#endif // EDITORTABWIDGET_H
