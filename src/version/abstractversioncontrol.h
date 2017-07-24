#ifndef ABSTRACTVERSIONCONTROL_H
#define ABSTRACTVERSIONCONTROL_H

#include <QObject>
#include <QSet>

class AbstractVersionControl : public QObject
{
    Q_OBJECT
public:
    explicit AbstractVersionControl(QObject *parent = nullptr);

    void setPath(const QString &path);

    bool isFileModified(const QString &filePath) const;
    bool isDirModified(const QString &filePath) const;
    bool isFileTracked(const QString &filePath) const;

    const QSet<QString> &trackedFiles();
    const QSet<QString> &modifiedFiles();

protected:
    virtual void analysePath();

signals:
    void newModifiedFiles(QSet<QString> modifiedFiles);
    void newValidatedFiles(QSet<QString> modifiedFiles);

protected:
    QString _path;
    QSet<QString> _trackedFiles;
    QSet<QString> _modifiedFiles;
};

#endif // ABSTRACTVERSIONCONTROL_H
