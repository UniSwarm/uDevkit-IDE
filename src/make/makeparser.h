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

#ifndef MAKEPARSER_H
#define MAKEPARSER_H

#include <QFileSystemWatcher>
#include <QMultiMap>
#include <QObject>
#include <QSet>
#include <QStringList>

#include "makerule.h"

#include "settings/settingsclass.h"

class QProcess;

class MakeParser : public QObject
{
    Q_OBJECT
public:
    explicit MakeParser(const QString &basePath = QString());
    ~MakeParser() override;

    void setPath(const QString &basePath);
    QString resolveFilePath(const QString &filePath);

    const QMultiMap<QString, QString> &variables() const;
    QStringList sourceFiles() const;

    const QMap<QString, MakeRule> &rules() const;
    MakeRule buildRule(const QString &filePath) const;
    QList<MakeRule> includedInRules(const QString &filePath) const;
    QList<MakeRule> targets() const;

    QStringList evalVariable(const QString &varName) const;

signals:
    void sourceChanged();
    void sourceFilesAdded(QSet<QString> newSourcesFiles);
    void sourceFilesRemoved(QSet<QString> oldSourcesFiles);

public slots:

protected slots:
    void processEnd();
    void analyseMakefile(const QString &path);
    void updateSettings();

protected:
    QFileSystemWatcher *_makeWatcher;
    QProcess *_processMake;
    QString _basePath;
    QString _makefileFilePath;

    void addSource(QSet<QString> &sourcesList, const QStringList &varList);
    QSet<QString> _sourceFiles;
    QMultiMap<QString, QString> _variables;
    QMultiMap<QString, QString> _vpath;
    QMap<QString, MakeRule> _rules;

    QString _programPath;
    Setting *_settingPath;
};

#endif // MAKEPARSER_H
