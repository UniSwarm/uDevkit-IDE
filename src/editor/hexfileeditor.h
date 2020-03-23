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
