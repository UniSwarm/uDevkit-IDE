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
    explicit SearchReplaceWidget(QWidget *parent = Q_NULLPTR);

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
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif  // SEARCHREPLACEWIDGET_H
