#ifndef MAKEPARSER_H
#define MAKEPARSER_H

#include <QObject>
#include <QMultiMap>
#include <QStringList>
#include <QSet>
#include <QFileSystemWatcher>

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
    QStringList sourceFiles() const;

signals:
    void sourceChanged();
    void sourceFilesAdded(QSet<QString> newSourcesFiles);
    void sourceFilesRemoved(QSet<QString> oldSourcesFiles);

public slots:

protected slots:
    void processEnd();
    void analyseMakefile(const QString path);

protected:
    QFileSystemWatcher *_makeWatcher;
    QProcess *_processMake;
    QString _basePath;
    QString _makefileFilePath;

    void addSource(QSet<QString> &sourcesList, const QStringList &varList);
    QSet<QString> _sourceFiles;
    QMultiMap<QString, QString> _variables;
    QMultiMap<QString, QString> _vpath;
};

#endif // MAKEPARSER_H
