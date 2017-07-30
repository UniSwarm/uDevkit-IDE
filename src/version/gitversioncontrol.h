#ifndef GITVERSIONCONTROL_H
#define GITVERSIONCONTROL_H

#include "abstractversioncontrol.h"

#include <QFileSystemWatcher>

class QProcess;

class GitVersionControl : public AbstractVersionControl
{
    Q_OBJECT
public:
    GitVersionControl();
    virtual ~GitVersionControl();

protected slots:
    void reqModifFiles();
    void reqTrackedFiles();
    void reqValidatedFiles();
    void indexCheck();
    void processEnd();

protected:
    void parseFilesList(QSet<QString> &oldSed, QSet<QString> &outgoingFiles, QSet<QString> &incomingFiles);

protected:
    QString findGitDir();
    virtual void analysePath();

    enum State {
        None,
        ModifiedFiles,
        TrackedFiles,
        ValidatedFiles,
        DiffFile
    };
    State _state;

    QProcess *_processGit;
    QFileSystemWatcher *_indexWatcher;
    QString _gitPath;
};

#endif // GITVERSIONCONTROL_H
