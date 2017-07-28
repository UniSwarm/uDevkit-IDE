#ifndef SEARCHREPLACEWIDGET_H
#define SEARCHREPLACEWIDGET_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
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
    void next();
    void prev();
    void all();

protected:
    void createWidgets();
    QLineEdit *_searchLineEdit;
    QCheckBox *_regexpCheckbox;
    QLabel *_statusLabel;

    QToolButton *_nextButton;
    QToolButton *_prevButton;
    QToolButton *_allButton;

    Editor *_editor;
};

#endif // SEARCHREPLACEWIDGET_H
