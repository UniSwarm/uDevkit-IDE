#ifndef ABSTRACTVERSIONCONTROL_H
#define ABSTRACTVERSIONCONTROL_H

#include <QObject>
#include <QSet>

class AbstractVersionControl : public QObject
{
    Q_OBJECT
public:
    explicit AbstractVersionControl(QObject *parent = nullptr);

    virtual QString versionControlName() const;
    void setPath(const QString &path);

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

protected:
    virtual void analysePath();

signals:
    void newModifiedFiles(QSet<QString> modifiedFiles);
    void newValidatedFiles(QSet<QString> modifiedFiles);

protected:
    QString _path;
    QSet<QString> _validatedFiles;
    QSet<QString> _trackedFiles;
    QSet<QString> _modifiedFiles;
};

#endif // ABSTRACTVERSIONCONTROL_H
