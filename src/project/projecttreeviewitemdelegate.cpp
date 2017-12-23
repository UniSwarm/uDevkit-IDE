#include "projecttreeviewitemdelegate.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>

ProjectTreeViewItemDelegate::ProjectTreeViewItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ProjectTreeViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*QStyledItemDelegate::paint(painter, option, index);
    return;*/
    Q_ASSERT(index.isValid());

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    painter->save();
    painter->setRenderHint(QPainter::TextAntialiasing);
    QRect rect = option.rect;
    QColor textColor(opt.palette.color(QPalette::Foreground));
    if (index.data(Qt::TextColorRole).canConvert(QMetaType::QColor))
        textColor = index.data(Qt::TextColorRole).value<QColor>();
    if (opt.state & QStyle::State_Selected || opt.state & QStyle::State_MouseOver)
    {
        if (opt.state & QStyle::State_Selected)
            painter->fillRect(opt.rect, QColor(0x287399));
        else
            painter->fillRect(opt.rect, QColor(0x18465d));
    }
    if (index.data(Qt::DecorationRole).canConvert(QMetaType::QIcon))
    {
        const QIcon &icon = index.data(Qt::DecorationRole).value<QIcon>();
        icon.paint(painter, rect.x()+1, rect.y()+1, rect.height()-2, rect.height()-2, Qt::AlignVCenter | Qt::AlignHCenter);
        rect = rect.adjusted(rect.height()+5, 0, 0, 0);
    }
    painter->setPen(textColor);
    painter->setFont(opt.font);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    painter->restore();
}

QSize ProjectTreeViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index) + QSize(2, 2);
}
