#ifndef SEARCHREPLACEWIDGET_H
#define SEARCHREPLACEWIDGET_H

#include <QLineEdit>
#include <QWidget>

#include "editor/editor.h"

class SearchReplaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchReplaceWidget(QWidget *parent = nullptr);

signals:

public slots:
    void setEditor(Editor *editor);
    void activate();

protected slots:
    void upadteSearch();

protected:
    void createWidgets();
    QLineEdit *_searchLineEdit;

    Editor *_editor;
};

#endif // SEARCHREPLACEWIDGET_H
