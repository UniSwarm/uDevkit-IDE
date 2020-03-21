#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "editor.h"

class QImage;
class QLabel;

class ImageEditor : public Editor
{
    Q_OBJECT
public:
    explicit ImageEditor(Project *project, QWidget *parent = nullptr);

    bool isModified() const override;

    Type type() const override
    {
        return Image;
    }

    SearchCaps searchCap() const override;

protected:
    QImage _imageData;
    QLabel *_imageView;
    bool _modified;

    int openFileData(const QString &filePath) override;
    int saveFileData(const QString &filePath = QString()) override;
};

#endif // IMAGEEDITOR_H
