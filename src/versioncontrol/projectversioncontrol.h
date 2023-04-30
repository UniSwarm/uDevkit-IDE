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

#ifndef PROJECTVERSIONCONTROL_H
#define PROJECTVERSIONCONTROL_H

#include "abstractversioncontrol.h"

class ProjectVersionControl : public AbstractVersionControl
{
    Q_OBJECT
public:
    explicit ProjectVersionControl(QObject *parent = nullptr);
    ~ProjectVersionControl();

    AbstractVersionControl *versionControl(const QString &filePath) const;

protected:
    AbstractVersionControl *_baseVersionControl;
    void addNewVersionControl(AbstractVersionControl *versionControl, const QString &path) const;
    AbstractVersionControl *createVersionRoot(const QString &path) const;

    mutable QMap<QString, AbstractVersionControl *> _versionControlMap;
    mutable QList<AbstractVersionControl *> _versionControls;

    void processModifiedFiles(const QSet<QString> &modifiedFiles);
    void processValidatedFiles(const QSet<QString> &validatedFiles);
    void processModifAvailable(const QString &file);

    QString findVersionRoot(const QString &path) const;

    // AbstractVersionControl interface
public:
    QString versionControlName() const override;
    void validFile(const QSet<QString> &filesPath) override;
    void inValidFile(const QSet<QString> &filesPath) override;
    void checkoutFile(const QSet<QString> &filesPath) override;
    bool isValid() const override;
    void requestFileModifications(const QString &filePath) override;
    FileVersionChange fileModifications(const QString &filePath) override;

    AbstractVersionControl *baseVersionControl() const;

protected:
    void analysePath() override;
};

#endif  // PROJECTVERSIONCONTROL_H
