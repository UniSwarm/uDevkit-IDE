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

    void insertedText(edbee::TextBufferChange change);

private:
    static bool initialized;
    QVariant _searchTerm;
    CodeEditorMarginDelegate *_codeEditorMarginDelegate;
    CodeEditorScrollBar *_codeEditorScrollBar;
    FileVersionChange _localFileChange;
    void notifyModificationChanged();
};

#endif // CODEEDITOR_H
