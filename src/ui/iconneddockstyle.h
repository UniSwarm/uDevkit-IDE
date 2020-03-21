#ifndef ICONNEDDOCKSTYLE_H
#define ICONNEDDOCKSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionComplex>

class IconnedDockStyle : public QProxyStyle
{
    Q_OBJECT
public:
    IconnedDockStyle(const QIcon &icon, QStyle *style = nullptr);

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;

protected:
    QIcon _icon;
};

#endif // ICONNEDDOCKSTYLE_H
