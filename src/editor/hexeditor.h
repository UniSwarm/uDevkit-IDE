#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include "editor.h"

class QHexEdit;

class HexEditor : public Editor
{
    Q_OBJECT
public:
    explicit HexEditor(Project *project, QWidget *parent = 0);

    bool isModified() const;

    Type type() const {return Hexa;}

    virtual SearchCaps searchCap() const;

protected slots:
    void modificationAppend();

protected:
    QHexEdit *_hexEditor;
    QFile _file;
    bool _modified;

    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());
};

#endif // HEXEDITOR_H
