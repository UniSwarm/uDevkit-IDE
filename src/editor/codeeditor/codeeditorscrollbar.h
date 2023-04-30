/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
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

#ifndef CODEEDITORSCROLLBAR_H
#define CODEEDITORSCROLLBAR_H

#include <QScrollBar>

#include "edbee/views/textrenderer.h"
#include "versioncontrol/fileversionchange.h"

class CodeEditorScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CodeEditorScrollBar(QWidget *parent = nullptr);
    explicit CodeEditorScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);

    FileVersionChange *fileChange() const;
    void setFileChange(FileVersionChange *fileChange);

    edbee::TextRenderer *textRender() const;
    void setTextRender(edbee::TextRenderer *textRender);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

protected:
    FileVersionChange *_fileChange;
    edbee::TextRenderer *_textRender;
};

#endif  // CODEEDITORSCROLLBAR_H
