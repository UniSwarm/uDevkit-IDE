#ifndef HEXFILE_H
#define HEXFILE_H

#include <QByteArray>
#include <QString>

class HexFile
{
public:
    HexFile(const QString &fileName = QString());

    bool read();
    const QByteArray &prog() const;

    const unsigned short &checksum() const;

private:
    QByteArray _prog;
    QString _fileName;
    unsigned short _checksum;
};

#endif // HEXFILE_H
