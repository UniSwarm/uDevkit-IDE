#include "searchreplacewidget.h"

#include <QBoxLayout>

SearchReplaceWidget::SearchReplaceWidget(QWidget *parent) : QWidget(parent)
{
    _editor = nullptr;

    createWidgets();
}

void SearchReplaceWidget::setEditor(Editor *editor)
{
    _editor = editor;
}

void SearchReplaceWidget::activate()
{
    show();
    _searchLineEdit->setFocus();
    _searchLineEdit->selectAll();
}

void SearchReplaceWidget::upadteSearch()
{
    if (!_editor)
        return;
    if (_editor->hasResearch())
        _editor->search(_searchLineEdit->text());
}

void SearchReplaceWidget::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    _searchLineEdit = new QLineEdit();
    _searchLineEdit->setPlaceholderText("search");
    layout->addWidget(_searchLineEdit);
    connect(_searchLineEdit, &QLineEdit::textChanged, this, &SearchReplaceWidget::upadteSearch);

    setLayout(layout);
}
