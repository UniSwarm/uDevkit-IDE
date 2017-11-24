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

    void activateResearch();
    void activateReplace();

    Editor::SearchFlags flags();

protected slots:
    void upadteSearch();
    void searchNext();
    void searchPrev();
    void searchAll();

    void replaceNext();
    void replacePrev();
    void replaceAll();

protected:
    void createWidgets();
    QLineEdit *_searchLineEdit;
    QToolButton *_nextButton;
    QToolButton *_prevButton;
    QToolButton *_allButton;

    QLineEdit *_replaceLineEdit;
    QToolButton *_replaceNextButton;
    QToolButton *_replacePrevButton;
    QToolButton *_replaceAllButton;

    QCheckBox *_regexpCheckbox;
    QCheckBox *_caseSensitivityCheckbox;
    QLabel *_statusLabel;

    Editor *_editor;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif // SEARCHREPLACEWIDGET_H
