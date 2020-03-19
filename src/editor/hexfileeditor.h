#ifndef HEXFILEEDITOR_H
#define HEXFILEEDITOR_H

#include "hexeditor.h"

class HexFileEditor : public HexEditor
{
    Q_OBJECT
public:
    explicit HexFileEditor(Project *project, QWidget *parent = 0);
    ~HexFileEditor();

    Type type() const {return Hexa;}

protected:
    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());
};

#endif // HEXFILEEDITOR_H
