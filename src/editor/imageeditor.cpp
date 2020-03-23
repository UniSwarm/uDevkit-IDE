/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

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
