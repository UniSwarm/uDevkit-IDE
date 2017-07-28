#include "searchreplacewidget.h"

#include <QBoxLayout>

SearchReplaceWidget::SearchReplaceWidget(QWidget *parent) : QWidget(parent)
{
    _editor = nullptr;

    createWidgets();
    setEditor(nullptr);
}

void SearchReplaceWidget::setEditor(Editor *editor)
{
    _editor = editor;

    if (!_editor || !_editor->hasResearch())
    {
        setEnabled(false);
        return;
    }

    setEnabled(true);
    _regexpCheckbox->setEnabled(_editor->hasRegExp());
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

    int found = _editor->search(_searchLineEdit->text());
    _statusLabel->setText(QString("%1 occurence%2 found").arg(found).arg(found>1 ? "s" : ""));
}

void SearchReplaceWidget::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    _searchLineEdit = new QLineEdit();
    _searchLineEdit->setPlaceholderText("search");
    layout->addWidget(_searchLineEdit);
    connect(_searchLineEdit, &QLineEdit::textChanged, this, &SearchReplaceWidget::upadteSearch);

    _regexpCheckbox = new QCheckBox("regexp");
    _regexpCheckbox->setChecked(true);
    layout->addWidget(_regexpCheckbox);
    connect(_regexpCheckbox, &QCheckBox::stateChanged, this, &SearchReplaceWidget::upadteSearch);

    _statusLabel = new QLabel();
    _statusLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(_statusLabel);

    setLayout(layout);
}
