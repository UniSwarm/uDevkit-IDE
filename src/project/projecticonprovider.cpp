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

    QIcon makefile(":/icons/img/mimetype/text-x-makefile.png");
    _icons.insert("mk", makefile);
}

QIcon ProjectIconProvider::icon(const QFileInfo &info) const
{
    const QIcon &icon = _icons[info.suffix()];

    if (icon.isNull())
        return QFileIconProvider::icon(info);
    return icon;
}
