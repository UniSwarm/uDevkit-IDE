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

protected slots:
    void reqFiles();
    void indexCheck();
    void parseModifiedFiles();

protected:
    QString findGitDir();
    virtual void analysePath();

    QProcess *_process;
    QFileSystemWatcher *_indexWatcher;
    QString _gitPath;
};

#endif // GITVERSIONCONTROL_H
