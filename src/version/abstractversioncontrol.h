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

#ifndef ABSTRACTVERSIONCONTROL_H
#define ABSTRACTVERSIONCONTROL_H

#include <QMap>
#include <QObject>
#include <QSet>

#include "fileversionchange.h"

class AbstractVersionControl : public QObject
{
    Q_OBJECT
public:
    explicit AbstractVersionControl(QObject *parent = Q_NULLPTR);

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
