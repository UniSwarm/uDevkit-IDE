#ifndef MAKEPARSER_H
#define MAKEPARSER_H

#include <QObject>
#include <QMultiMap>
#include <QStringList>

class QProcess;

class MakeParser : public QObject
{
    Q_OBJECT
public:
    explicit MakeParser(const QString &basePath=QString());
    ~MakeParser();

    void setPath(const QString &basePath);
    QString resolveFilePath(const QString &filePath);

    const QMultiMap<QString, QString> &variables() const;
    const QStringList &sourceFiles() const;

signals:
    void sourceChanged();

public slots:

protected slots:
    void processEnd();

protected:
    QProcess *_processMake;
    QString _basePath;

    QStringList _sourceFiles;
    QMultiMap<QString, QString> _variables;
    QMultiMap<QString, QString> _vpath;
};

#endif // MAKEPARSER_H
