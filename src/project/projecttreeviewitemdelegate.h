#ifndef PROJECTTREEVIEWITEMDELEGATE_H
#define PROJECTTREEVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

class ProjectTreeViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProjectTreeViewItemDelegate(QObject *parent = Q_NULLPTR);

    // QStyledItemDelegate interface
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PROJECTTREEVIEWITEMDELEGATE_H
