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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "editor/editor.h"

#include "codeeditormargindelegate.h"
#include "codeeditorscrollbar.h"

#include "settings/settingsclass.h"

namespace edbee
{
class TextEditorWidget;
}

class CodeEditor : public Editor
{
    Q_OBJECT
public:
    explicit CodeEditor(Project *project, QWidget *parent = nullptr);

    bool isModified() const override;

    void gotoLine(int x, int y = -1) override;

    Type type() const override
    {
        return Code;
    }

    void setText(const QString &text);
    void setGrammar(const QString &grammarName);
    void setSettingsClass(SettingsClass *settingsClass);

    SearchCaps searchCap() const override;
    int search(const QVariant &searchTerm, SearchFlags flags = SearchFlag(NoFlag)) override;
    void searchNext() override;
    void searchPrev() override;
    void searchSelectAll() override;

    void replace(const QVariant &replacePattern, SearchFlags flags = SearchFlag(NoFlag), bool next = true) override;
    void replaceAll(const QVariant &replacePattern, SearchFlags flags = SearchFlag(NoFlag)) override;

    QWidget *editorWidget() override;

protected:
    edbee::TextEditorWidget *_editorWidget;
    void initialiseWidget() override;
    void giveFocus() override;

    SettingsClass *_settingsClass;

    void undoCommand() override;
    void redoCommand() override;
    void cutCommand() override;
    void copyCommand() override;
    void pasteCommand() override;
    void formatCommand() override;

    int openFileData(const QString &filePath) override;
    int saveFileData(const QString &filePath = QString()) override;

protected slots:
    void updateSettings();
    void modificationAppend();
    void help();
    void updatePos();
    void updateModifications(const QString &filePath);

    void insertedText(const edbee::TextBufferChange &change);

private:
    static bool initialized;
    QVariant _searchTerm;
    CodeEditorMarginDelegate *_codeEditorMarginDelegate;
    CodeEditorScrollBar *_codeEditorScrollBar;
    FileVersionChange _localFileChange;
    void notifyModificationChanged();
};

#endif // CODEEDITOR_H
