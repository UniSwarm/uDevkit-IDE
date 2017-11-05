#ifndef GITVERSIONCONTROL_H
#define GITVERSIONCONTROL_H

#include "abstractversioncontrol.h"

#include <QFileSystemWatcher>
#include <QQueue>

class QProcess;

class GitVersionControl : public AbstractVersionControl
{
    Q_OBJECT
public:
    GitVersionControl();
    virtual ~GitVersionControl();

    virtual QString versionControlName() const;
    QString basePath() const;

    virtual void validFile(const QSet<QString> &filesPath);
    virtual void inValidFile(const QSet<QString> &filesPath);
    virtual void checkoutFile(const QSet<QString> &filesPath);

    virtual bool isValid() const;

    virtual void requestFileModifications(const QString &filePath);

protected slots:
    void reqFetch();
    void reqModifFiles();
    void reqTrackedFiles();
    void reqValidatedFiles();
    void indexCheck();
    void processEnd();

    void reqFileModif(const QString &filePath);
    void processDiffEnd();

protected:
    void parseFilesList(QSet<QString> &oldSed, QSet<QString> &outgoingFiles, QSet<QString> &incomingFiles);

protected:
    void findGitDir();
    virtual void analysePath();

    enum State {
        None,
        Check,
        Fetch,
        ModifiedFiles,
        TrackedFiles,
        ValidatedFiles,
        DiffFile
    };
    State _state;

    QProcess *_processGitState;
    QFileSystemWatcher *_indexWatcher;
    QString _gitPath;
    QString _basePath;

    QProcess *_processGitDiff;
    QQueue<QString> _diffRequestQueue;
};

#endif // GITVERSIONCONTROL_H
