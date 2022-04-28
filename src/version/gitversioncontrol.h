/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef GITVERSIONCONTROL_H
#define GITVERSIONCONTROL_H

#include "abstractversioncontrol.h"

#include <QFileSystemWatcher>
#include <QQueue>

class QProcess;
class SettingsClass;

class GitVersionControl : public AbstractVersionControl
{
    Q_OBJECT
public:
    GitVersionControl();
    ~GitVersionControl() override;

    QString versionControlName() const override;
    QString basePath() const override;

    void validFile(const QSet<QString> &filesPath) override;
    void inValidFile(const QSet<QString> &filesPath) override;
    void checkoutFile(const QSet<QString> &filesPath) override;

    bool isValid() const override;

    void requestFileModifications(const QString &filePath) override;

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
    void analysePath() override;

    enum StatusState
    {
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

    enum DiffState
    {
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

#endif  // GITVERSIONCONTROL_H
