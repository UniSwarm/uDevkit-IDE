#include "settingswindow.h"

#include <QBoxLayout>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog (parent)
{
    setMinimumSize(800, 600);
    createWidgets();
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
    itemList->setTextAlignment(Qt::AlignHCenter);
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
}

void SettingsWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    _sectionsStack->setCurrentIndex(_sectionsList->row(current));
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
    connect(_sectionsList, &QListWidget::currentItemChanged, this, &SettingsWindow::changePage);
    layoutV->addWidget(_sectionsList);

    _sectionsStack = new QStackedWidget();
    layoutV->addWidget(_sectionsStack);

    layout->addLayout(layoutV);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(layout);
}
