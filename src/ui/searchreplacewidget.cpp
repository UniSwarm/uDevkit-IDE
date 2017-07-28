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
    if(parentWidget() && parentWidget()->parentWidget())
        parentWidget()->parentWidget()->show();
    show();
    _searchLineEdit->setFocus();
    _searchLineEdit->selectAll();
}

void SearchReplaceWidget::upadteSearch()
{
    if (!_editor)
        return;

    Editor::SearchFlags flags;
    if (_regexpCheckbox->isEnabled() && _regexpCheckbox->isChecked())
        flags |= Editor::RegExpMode;
    int found = _editor->search(_searchLineEdit->text(), flags);
    _statusLabel->setText(QString("%1 occurence%2 found").arg(found).arg(found>1 ? "s" : ""));
}

void SearchReplaceWidget::next()
{
    if (!_editor)
        return;
    _editor->searchNext();
}

void SearchReplaceWidget::prev()
{
    if (!_editor)
        return;
    _editor->searchPrev();
}

void SearchReplaceWidget::all()
{
    if (!_editor)
        return;
    _editor->searchSelectAll();
}

void SearchReplaceWidget::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    // search and next prev buttons
    QHBoxLayout *searchLineLayout = new QHBoxLayout();
    searchLineLayout->setMargin(0);
    _searchLineEdit = new QLineEdit();
    _searchLineEdit->setPlaceholderText("search");
    searchLineLayout->addWidget(_searchLineEdit);
    connect(_searchLineEdit, &QLineEdit::textChanged, this, &SearchReplaceWidget::upadteSearch);

    _prevButton = new QToolButton();
    _prevButton->setText("<");
    searchLineLayout->addWidget(_prevButton);
    connect(_prevButton, &QToolButton::clicked, this, &SearchReplaceWidget::prev);

    _nextButton = new QToolButton();
    _nextButton->setText(">");
    searchLineLayout->addWidget(_nextButton);
    connect(_nextButton, &QToolButton::clicked, this, &SearchReplaceWidget::next);

    _allButton = new QToolButton();
    _allButton->setText("*");
    searchLineLayout->addWidget(_allButton);
    connect(_allButton, &QToolButton::clicked, this, &SearchReplaceWidget::all);

    layout->addLayout(searchLineLayout);

    // options
    _regexpCheckbox = new QCheckBox("regexp");
    _regexpCheckbox->setChecked(true);
    layout->addWidget(_regexpCheckbox);
    connect(_regexpCheckbox, &QCheckBox::stateChanged, this, &SearchReplaceWidget::upadteSearch);

    // status
    _statusLabel = new QLabel();
    _statusLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(_statusLabel);

    setLayout(layout);
}
