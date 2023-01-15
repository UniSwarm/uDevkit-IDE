#include "codeutils.h"

QString CodeUtils::extRegExpReplace(const QString &before, const QRegularExpression &re, const QString &after)
{
    QString final;

    QRegularExpressionMatch match = re.match(before);
    if (!match.isValid())
    {
        return after;
    }

    const int al = after.length();
    const QChar *ac = after.unicode();
    int lastPos = 0;
    for (int i = 0; i < al - 1; i++)
    {
        if (ac[i] == QChar('\\'))
        {
            char modifier = '\0';
            int ref = 0;
            int offset = 1;
            if (ac[i + offset].isLetter())
            {
                modifier = ac[i + offset].toLatin1();
                offset++;
            }
            if (ac[i + offset].isDigit())
            {
                ref = ac[i + offset].digitValue();
                offset++;
                if (ac[i + offset].isDigit())
                {
                    ref = ref * 10 + ac[i + offset].digitValue();
                }
            }
            if (ref == 0 || ref > match.capturedTexts().count() - 1)
            {
                continue;  // invalid
            }

            final.append(after.mid(lastPos, i - lastPos));
            lastPos = i + offset;

            switch (modifier)
            {
                case '\0':
                    final.append(match.captured(ref));
                    break;

                case 'U':
                case 'u':
                    final.append(match.captured(ref).toUpper());
                    break;

                case 'l':
                case 'L':
                    final.append(match.captured(ref).toLower());
                    break;

                default:
                    break;
            }
        }
    }
    final.append(after.mid(lastPos));

    return final;
}
