#include "iconneddockstyle.h"

IconnedDockStyle::IconnedDockStyle(const QIcon &icon, QStyle *style)
    : QProxyStyle(style), _icon(icon)
{

}

void IconnedDockStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::CE_DockWidgetTitle)
    {
        //width of the icon
        int width = pixelMetric(QStyle::PM_ToolBarIconSize);
        //margin of title from frame
        int margin = baseStyle()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);

        QPoint iconPos(margin + option->rect.left(), margin + option->rect.center().y() - width * 2 / 3);

        painter->drawPixmap(iconPos, _icon.pixmap(width, width));

        const_cast<QStyleOption*>(option)->rect = option->rect.adjusted(width * 3 / 2, 0, 0, 0);
    }
    baseStyle()->drawControl(element, option, painter, widget);
}
