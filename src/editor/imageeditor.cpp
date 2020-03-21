#include "imageeditor.h"

#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

ImageEditor::ImageEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    _imageView = new QLabel(this);
    _imageView->setAlignment(Qt::AlignCenter);

    layout->addWidget(_imageView);
    setLayout(layout);
}

bool ImageEditor::isModified() const
{
    return _modified;
}

Editor::SearchCaps ImageEditor::searchCap() const
{
    return NoSearch;
}

int ImageEditor::openFileData(const QString &filePath)
{
    QImage image(filePath);
    if (image.isNull())
    {
        return -1;
    }
    _imageData = image;

    _imageView->setPixmap(QPixmap::fromImage(_imageData));

    _modified = false;
    emit modified(isModified());
    setFilePath(filePath);
    return 0;
}

int ImageEditor::saveFileData(const QString &filePath)
{
    Q_UNUSED(filePath)
    return -1;
}
