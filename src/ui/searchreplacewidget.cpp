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

Editor::SearchFlags SearchReplaceWidget::flags()
{
    Editor::SearchFlags mflags;
    if (_regexpCheckbox->isEnabled() && _regexpCheckbox->isChecked())
        mflags |= Editor::RegExpMode;
    if (_caseSensitivityCheckbox->isEnabled() && _caseSensitivityCheckbox->isChecked())
        mflags |= Editor::CaseSensitive;
    return mflags;
}

void SearchReplaceWidget::upadteSearch()
{
    if (!_editor)
        return;

    int found = _editor->search(_searchLineEdit->text(), flags());
    _statusLabel->setText(QString("%1 occurence%2 found").arg(found).arg(found>1 ? "s" : ""));
}

void SearchReplaceWidget::searchNext()
{
    if (!_editor)
        return;
    _editor->searchNext();
}

void SearchReplaceWidget::searchPrev()
{
    if (!_editor)
        return;
    _editor->searchPrev();
}

void SearchReplaceWidget::searchAll()
{
    if (!_editor)
        return;
    _editor->searchSelectAll();
}

void SearchReplaceWidget::replaceNext()
{
    if (!_editor)
        return;
    _editor->replaceAll(_replaceLineEdit->text());
}

void SearchReplaceWidget::replacePrev()
{
    if (!_editor)
        return;
    _editor->replaceAll(_replaceLineEdit->text());
}

void SearchReplaceWidget::replaceAll()
{
    if (!_editor)
        return;

    _editor->replaceAll(_replaceLineEdit->text(), flags());
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
    connect(_prevButton, &QToolButton::clicked, this, &SearchReplaceWidget::searchPrev);

    _nextButton = new QToolButton();
    _nextButton->setText(">");
    searchLineLayout->addWidget(_nextButton);
    connect(_nextButton, &QToolButton::clicked, this, &SearchReplaceWidget::searchNext);

    _allButton = new QToolButton();
    _allButton->setText("*");
    searchLineLayout->addWidget(_allButton);
    connect(_allButton, &QToolButton::clicked, this, &SearchReplaceWidget::searchAll);

    layout->addLayout(searchLineLayout);

    // search and next prev buttons
    QHBoxLayout *replaceLineLayout = new QHBoxLayout();
    replaceLineLayout->setMargin(0);
    _replaceLineEdit = new QLineEdit();
    _replaceLineEdit->setPlaceholderText("replace");
    replaceLineLayout->addWidget(_replaceLineEdit);

    _replacePrevButton = new QToolButton();
    _replacePrevButton->setText("<");
    replaceLineLayout->addWidget(_replacePrevButton);
    connect(_replacePrevButton, &QToolButton::clicked, this, &SearchReplaceWidget::replacePrev);

    _replaceNextButton = new QToolButton();
    _replaceNextButton->setText(">");
    replaceLineLayout->addWidget(_replaceNextButton);
    connect(_replaceNextButton, &QToolButton::clicked, this, &SearchReplaceWidget::replaceNext);

    _replaceAllButton = new QToolButton();
    _replaceAllButton->setText("*");
    replaceLineLayout->addWidget(_replaceAllButton);
    connect(_replaceAllButton, &QToolButton::clicked, this, &SearchReplaceWidget::replaceAll);

    layout->addLayout(replaceLineLayout);

    // options
    QHBoxLayout *optionsLineLayout = new QHBoxLayout();
    optionsLineLayout->setMargin(0);

    _regexpCheckbox = new QCheckBox("regexp");
    _regexpCheckbox->setChecked(true);
    layout->addWidget(_regexpCheckbox);
    connect(_regexpCheckbox, &QCheckBox::stateChanged, this, &SearchReplaceWidget::upadteSearch);

    _caseSensitivityCheckbox = new QCheckBox("case sensitive");
    _caseSensitivityCheckbox->setChecked(true);
    layout->addWidget(_caseSensitivityCheckbox);
    connect(_caseSensitivityCheckbox, &QCheckBox::stateChanged, this, &SearchReplaceWidget::upadteSearch);

    layout->addLayout(optionsLineLayout);

    // status
    _statusLabel = new QLabel();
    _statusLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(_statusLabel);

    setLayout(layout);
}
