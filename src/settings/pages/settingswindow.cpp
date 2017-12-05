#include "settingswindow.h"

#include <QAbstractButton>
#include <QBoxLayout>

#include "codeeditorsettings.h"
#include "htmleditorsettings.h"
#include "hexeditorsettings.h"
#include "imageeditorsettings.h"

#include "gitversionsettings.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog (parent)
{
    setMinimumSize(800, 600);
    createWidgets();

    SettingsCateg *categ;
    categ = addCategory(QIcon(":/icons/img/dark/icons8-editor.png"), "Editors");
    categ->addPage(new CodeEditorSettings());
    categ->addPage(new HtmlEditorSettings());
    categ->addPage(new HexEditorSettings());
    categ->addPage(new ImageEditorSettings());

    categ = addCategory(QIcon(":/icons/img/dark/icons8-versions.png"), "Version management");
    categ->addPage(new GitVersionSettings());

    categ = addCategory(QIcon(":/icons/img/dark/icons8-toolbox.png"), "Tools");
    //categ->addPage(new GitVersionSettings());

    QModelIndex modelIndex = _sectionsList->model()->index(0, 0, _sectionsList->rootIndex());
    _sectionsList->setCurrentIndex(modelIndex);
}

SettingsCateg *SettingsWindow::category(int i) const
{
    return static_cast<SettingsCateg *>(_sectionsStack->widget(i));
}

SettingsCateg *SettingsWindow::addCategory(const QIcon &icon, const QString &label)
{
    SettingsCateg *categ = new SettingsCateg(icon, label);
    addCategory(categ);
    return categ;
}

void SettingsWindow::addCategory(SettingsCateg *category)
{
    // adds to list
    QListWidgetItem *itemList = new QListWidgetItem(_sectionsList);
    itemList->setIcon(category->icon());
    itemList->setText(category->label());
    itemList->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    itemList->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    // added to stack widget
    _sectionsStack->addWidget(category);
}

void SettingsWindow::commitChange()
{
    for (int i=0; i<_sectionsStack->count(); i++)
    {
        SettingsCateg *categ = static_cast<SettingsCateg *>(_sectionsStack->widget(i));
        categ->commitChange();
    }
    SettingsManager::save();
}

void SettingsWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    _sectionsStack->setCurrentIndex(_sectionsList->row(current));
}

void SettingsWindow::buttonClick(QAbstractButton *button)
{
    switch (_dialogButtonBox->buttonRole(button))
    {
    case QDialogButtonBox::AcceptRole:
        commitChange();
        accept();
        break;
    case QDialogButtonBox::RejectRole:
        reject();
        break;
    case QDialogButtonBox::ApplyRole:
        commitChange();
        break;
    default:
        break;
    }
}

void SettingsWindow::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(8);

    QHBoxLayout *layoutV = new QHBoxLayout();
    layoutV->setMargin(0);

    _sectionsList = new QListWidget();
    _sectionsList->setViewMode(QListView::ListMode);
    _sectionsList->setMaximumWidth(200);
    _sectionsList->setIconSize(QSize(48, 48));
    _sectionsList->setUniformItemSizes(true);
    _sectionsList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(_sectionsList, &QListWidget::currentItemChanged, this, &SettingsWindow::changePage);
    layoutV->addWidget(_sectionsList);

    _sectionsStack = new QStackedWidget();
    layoutV->addWidget(_sectionsStack);

    layout->addLayout(layoutV);
    _dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    layout->addWidget(_dialogButtonBox);
    connect(_dialogButtonBox, &QDialogButtonBox::clicked, this, &SettingsWindow::buttonClick);

    setLayout(layout);
}
