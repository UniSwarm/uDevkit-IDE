/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "settingswindow.h"

#include <QAbstractButton>
#include <QBoxLayout>

#include "codeeditorsettings.h"
#include "hexeditorsettings.h"
#include "htmleditorsettings.h"
#include "imageeditorsettings.h"

#include "gitversionsettings.h"

#include "toolsversionsettings.h"

#include "globalsettings.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    setMinimumSize(800, 600);
    createWidgets();

    SettingsCateg *categ;
    categ = addCategory(QIcon(":/icons/img/dark/icons8-editor.png"), tr("Editors"));
    categ->addPage(new CodeEditorSettings());
    categ->addPage(new HtmlEditorSettings());
    categ->addPage(new HexEditorSettings());
    categ->addPage(new ImageEditorSettings());

    categ = addCategory(QIcon(":/icons/img/dark/icons8-versions.png"), tr("Version management"));
    categ->addPage(new GitVersionSettings());

    categ = addCategory(QIcon(":/icons/img/dark/icons8-toolbox.png"), tr("Tools"));
    categ->addPage(new ToolsVersionSettings());

    categ = addCategory(QIcon(":/icons/img/dark/icons8-settings.png"), tr("Global"));
    categ->addPage(new GlobalSettings());

    QModelIndex modelIndex = _sectionsList->model()->index(0, 0, _sectionsList->rootIndex());
    _sectionsList->setCurrentIndex(modelIndex);
}

SettingsCateg *SettingsWindow::category(int i) const
{
    return dynamic_cast<SettingsCateg *>(_sectionsStack->widget(i));
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
    for (int i = 0; i < _sectionsStack->count(); i++)
    {
        SettingsCateg *categ = dynamic_cast<SettingsCateg *>(_sectionsStack->widget(i));
        categ->commitChange();
    }
    SettingsManager::save();
}

void SettingsWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }

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
