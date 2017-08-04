#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "editor.h"

class QImage;
class QLabel;

class ImageEditor : public Editor
{
    Q_OBJECT
public:
    explicit ImageEditor(Project *project, QWidget *parent = 0);

    bool isModified() const;

    Type type() const {return Image;}

    virtual SearchCaps searchCap() const;

protected:
    QImage _imageData;
    QLabel *_imageView;
    bool _modified;

    int openFileData(const QString &filePath);
    int saveFileData(const QString &filePath=QString());
};

#endif // IMAGEEDITOR_H
