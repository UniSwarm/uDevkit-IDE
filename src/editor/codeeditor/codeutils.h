#ifndef CODEUTILS_H
#define CODEUTILS_H

#include <QRegularExpression>

class CodeUtils
{
public:
    static QString extRegExpReplace(const QString &before, const QRegularExpression &re, const QString &after);
};

#endif  // CODEUTILS_H
