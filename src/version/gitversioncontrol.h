#ifndef GITVERSIONCONTROL_H
#define GITVERSIONCONTROL_H

#include "abstractversioncontrol.h"

#include <QFileSystemWatcher>
#include <QQueue>

#include "settings/settingsmanager.h"

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
    void updateSettings();

    void reqFileModifHead(const QString &filePath);
    void reqFileModifNormal(const QString &filePath);
    void processDiffEnd();

protected:
    void parseFilesList(QSet<QString> &oldSed, QSet<QString> &outgoingFiles, QSet<QString> &incomingFiles);

protected:
    void findGitDir();
    virtual void analysePath();

    enum StatusState {
        StatusNone,
        StatusCheck,
        StatusFetch,
        StatusModifiedFiles,
        StatusTrackedFiles,
        StatusValidatedFiles
    };
    StatusState _statusState;
    QProcess *_processGitState;

    QFileSystemWatcher *_indexWatcher;
    QString _gitPath;
    QString _basePath;

    SettingsClass *_settingsClass;
    QString _programPath;

    enum DiffState {
        DiffNone,
        DiffHead,
        DiffNormal
    };
    DiffState _diffState;
    QString _fileGitDiff;
    FileVersionChange _fileChanges;
    QProcess *_processGitDiff;
    QQueue<QString> _diffRequestQueue;

    void launch(const QString &program, const QStringList &arguments);
};

#endif // GITVERSIONCONTROL_H
