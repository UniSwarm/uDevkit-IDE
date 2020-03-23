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

#include "projecticonprovider.h"

ProjectIconProvider::ProjectIconProvider()
{
    QIcon cpphdr(":/icons/img/mimetype/text-x-c++hdr.png");
    _icons.insert("hpp", cpphdr);

    QIcon cppsrc(":/icons/img/mimetype/text-x-c++src.png");
    _icons.insert("cpp", cppsrc);

    QIcon chdr(":/icons/img/mimetype/text-x-chdr.png");
    _icons.insert("h", chdr);

    QIcon csrc(":/icons/img/mimetype/text-x-csrc.png");
    _icons.insert("c", csrc);

    QIcon asmsrc(":/icons/img/mimetype/text-x-asm.png");
    _icons.insert("s", asmsrc);

    QIcon hexsrc(":/icons/img/mimetype/text-x-hex.png");
    _icons.insert("hex", hexsrc);

    QIcon xml(":/icons/img/mimetype/text-xml.png");
    _icons.insert("xml", xml);

    QIcon readme(":/icons/img/mimetype/text-x-readme.png");
    _icons.insert("README", readme);
    _icons.insert("md", readme);

    QIcon script(":/icons/img/mimetype/text-x-script.png");
    _icons.insert("sh", script);
    _icons.insert("bash", script);

    QIcon yacc(":/icons/img/mimetype/text-x-yacc.png");
    _icons.insert("y", yacc);
    _icons.insert("yml", yacc);

    QIcon obj(":/icons/img/mimetype/application-octet-stream.png");
    _icons.insert("o", obj);

    QIcon exe(":/icons/img/mimetype/application-x-executable.png");
    _icons.insert("exe", exe);

    QIcon makefile(":/icons/img/mimetype/text-x-makefile.png");
    _icons.insert("mk", makefile);
    _icons.insert("Makefile", makefile);
}

QIcon ProjectIconProvider::icon(const QFileInfo &info) const
{
    QString ext = info.suffix();
    if (ext.isEmpty())
    {
        ext = info.fileName();
    }

    QMap<QString, QIcon>::const_iterator find = _icons.find(ext);
    if (find != _icons.cend())
    {
        return *find;
    }

    return QFileIconProvider::icon(info);
}
