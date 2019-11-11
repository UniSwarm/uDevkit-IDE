#ifndef MAKERULE_H
#define MAKERULE_H

#include <QString>
#include <QStringList>

class MakeRule
{
public:
    MakeRule();

    QString target;
    QStringList dependencies;
    QStringList cmds;
    bool isTarget;
};

#endif // MAKERULE_H
