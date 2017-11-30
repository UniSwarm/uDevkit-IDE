#ifndef ICONNEDDOCKSTYLE_H
#define ICONNEDDOCKSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionComplex>

class IconnedDockStyle : public QProxyStyle
{
    Q_OBJECT
public:
    IconnedDockStyle(const QIcon& icon,  QStyle* style = 0);

    virtual void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget = 0) const;

protected:
    QIcon _icon;
};

#endif // ICONNEDDOCKSTYLE_H
