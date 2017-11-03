#ifndef ABSTRACTVERSIONCONTROL_H
#define ABSTRACTVERSIONCONTROL_H

#include <QObject>
#include <QSet>

#include "fileversionchange.h"

class AbstractVersionControl : public QObject
{
    Q_OBJECT
public:
    explicit AbstractVersionControl(QObject *parent = nullptr);

    virtual QString versionControlName() const;
    void setPath(const QString &path);
    virtual QString basePath() const;

    bool isDirTracked(const QString &filePath) const;
    bool isFileTracked(const QString &filePath) const;
    const QSet<QString> &trackedFiles() const;

    bool isFileModified(const QString &filePath) const;
    bool isDirModified(const QString &filePath) const;
    const QSet<QString> &modifiedFiles() const;

    bool isFileValidated(const QString &filePath) const;
    const QSet<QString> &validatedFiles() const;
    virtual void validFile(const QSet<QString> &filesPath);
    virtual void inValidFile(const QSet<QString> &filesPath);
    virtual void checkoutFile(const QSet<QString> &filesPath);

    virtual bool isValid() const;
    void modifFile(const QSet<QString> &filesPath);

    virtual void requestFileModifications(const QString &filePath);
    FileVersionChange fileModifications(const QString &filePath);

protected:
    virtual void analysePath();

signals:
    void newModifiedFiles(QSet<QString> modifiedFiles);
    void newValidatedFiles(QSet<QString> modifiedFiles);
    void fileModificationsAvailable(QString modifiedFiles);

protected:
    QString _path;
    QSet<QString> _validatedFiles;
    QSet<QString> _trackedFiles;
    QSet<QString> _modifiedFiles;
    QMap<QString, FileVersionChange> _changeForFile;
};

#endif // ABSTRACTVERSIONCONTROL_H
