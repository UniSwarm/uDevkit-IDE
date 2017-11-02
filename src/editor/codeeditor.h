#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "editor.h"

namespace edbee {
    class TextEditorWidget;
}

class CodeEditor : public Editor
{
    Q_OBJECT
public:
    explicit CodeEditor(Project *project, QWidget *parent = 0);

    bool isModified() const;

    void gotoLine(int x, int y=-1);

    Type type() const {return Code;}

    virtual SearchCaps searchCap() const;
    virtual int search(const QVariant &searchTerm, SearchFlags flags=SearchFlag(NoFlag));
    virtual void searchNext();
    virtual void searchPrev();
    virtual void searchSelectAll();

    virtual void replace(const QVariant &replacePattern, SearchFlags flags=SearchFlag(NoFlag), bool next=true);
    virtual void replaceAll(const QVariant &replacePattern, SearchFlags flags=SearchFlag(NoFlag));

protected:
    edbee::TextEditorWidget* _editorWidget;
    void initialiseWidget();
    void giveFocus();

    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());

protected slots:
    void modificationAppend();
    void help();

private:
    static bool initialized;
    QVariant _searchTerm;
};

#endif // CODEEDITOR_H
