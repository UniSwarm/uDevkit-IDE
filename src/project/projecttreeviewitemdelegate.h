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
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PROJECTTREEVIEWITEMDELEGATE_H
