#include "iconneddockstyle.h"

IconnedDockStyle::IconnedDockStyle(const QIcon &icon, QStyle *style)
    : QProxyStyle(style), _icon(icon)
{
}

void IconnedDockStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == QStyle::CE_DockWidgetTitle)
    {
        //width of the icon
        int width = pixelMetric(QStyle::PM_ToolBarIconSize);
        //margin of title from frame
        int margin = baseStyle()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);

        QPoint iconPos(margin + option->rect.left() + width / 6, margin + option->rect.y() + width / 6);

        painter->drawPixmap(iconPos, _icon.pixmap(width * 2 / 3, width * 2 / 3));

        const_cast<QStyleOption*>(option)->rect = option->rect.adjusted(width + margin * 2, 0, 0, 0);
    }
    baseStyle()->drawControl(element, option, painter, widget);
}
