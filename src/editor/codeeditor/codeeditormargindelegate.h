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

#ifndef CODEEDITORMARGINDELEGATE_H
#define CODEEDITORMARGINDELEGATE_H

#include "edbee/views/components/textmargincomponent.h"

#include "versioncontrol/fileversionchange.h"

class CodeEditorMarginDelegate : public edbee::TextMarginComponentDelegate
{
public:
    CodeEditorMarginDelegate();
    ~CodeEditorMarginDelegate() override;

    FileVersionChange *fileChange() const;
    void setFileChange(FileVersionChange *fileChange);

    // TextMarginComponentDelegate interface
public:
    int widthBeforeLineNumber() override;
    void renderAfter(QPainter *painter, int startLine, int endLine, int width) override;

protected:
    FileVersionChange *_fileChange;

    // TextMarginComponentDelegate interface
public:
    void mousePressEvent(int line, QMouseEvent *event) override;
};

#endif  // CODEEDITORMARGINDELEGATE_H
