#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include "editor.h"

class QHexEdit;

class HexEditor : public Editor
{
    Q_OBJECT
public:
    explicit HexEditor(Project *project, QWidget *parent = nullptr);
    ~HexEditor() override;

    bool isModified() const override;

    Type type() const override
    {
        return Hexa;
    }

    SearchCaps searchCap() const override;

protected slots:
    void modificationAppend();
    void updatePos();

protected:
    QHexEdit *_hexEditor;
    QFile _file;
    bool _modified;

    int openFileData(const QString &filePath) override;
    int saveFileData(const QString &filePath = QString()) override;

    void giveFocus() override;

    void undoCommand() override;
    void redoCommand() override;
    void cutCommand() override;
    void copyCommand() override;
    void pasteCommand() override;
};

#endif // HEXEDITOR_H
