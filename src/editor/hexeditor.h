#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include "editor.h"

class QHexEdit;

class HexEditor : public Editor
{
    Q_OBJECT
public:
    explicit HexEditor(Project *project, QWidget *parent = 0);
    ~HexEditor();

    bool isModified() const;

    Type type() const {return Hexa;}

    virtual SearchCaps searchCap() const;

protected slots:
    void modificationAppend();
    void updatePos();

protected:
    QHexEdit *_hexEditor;
    QFile _file;
    bool _modified;

    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());

    void giveFocus();

    virtual void undoCommand();
    virtual void redoCommand();
    virtual void cutCommand();
    virtual void copyCommand();
    virtual void pasteCommand();
};

#endif // HEXEDITOR_H
