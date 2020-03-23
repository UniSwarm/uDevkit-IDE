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

#include "iconneddockstyle.h"

IconnedDockStyle::IconnedDockStyle(const QIcon &icon, QStyle *style)
    : QProxyStyle(style)
    , _icon(icon)
{
}

void IconnedDockStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == QStyle::CE_DockWidgetTitle)
    {
        // width of the icon
        int width = pixelMetric(QStyle::PM_ToolBarIconSize);
        // margin of title from frame
        int margin = baseStyle()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);

        QPoint iconPos(margin + option->rect.left() + width / 6, margin + option->rect.y() + width / 6);

        painter->drawPixmap(iconPos, _icon.pixmap(width * 2 / 3, width * 2 / 3));

        const_cast<QStyleOption *>(option)->rect = option->rect.adjusted(width + margin * 2, 0, 0, 0);
    }
    baseStyle()->drawControl(element, option, painter, widget);
}
