#ifndef HEXFILEEDITOR_H
#define HEXFILEEDITOR_H

#include "hexeditor.h"

class HexFileEditor : public HexEditor
{
    Q_OBJECT
public:
    explicit HexFileEditor(Project *project, QWidget *parent = nullptr);
    ~HexFileEditor() override;

    Type type() const override
    {
        return Hexa;
    }

protected:
    int openFileData(const QString &filePath) override;
    int saveFileData(const QString &filePath = QString()) override;
};

#endif // HEXFILEEDITOR_H
