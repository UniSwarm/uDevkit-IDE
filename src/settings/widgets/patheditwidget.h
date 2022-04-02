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

#ifndef PATHEDITWIDGET_H
#define PATHEDITWIDGET_H

#include <QFileSystemModel>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QToolButton>
#include <QWidget>

class PathEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PathEditWidget(QWidget *parent = nullptr);

    QString programm() const;
    void setProgramm(const QString &programm);

    QString path() const;
    void setPath(const QString &path);

    QString versionArg() const;
    void setVersionArg(const QString &versionArg);

    bool isValid() const;

    void setEnv(QProcessEnvironment env);

public slots:
    void checkProgramm();

private slots:
    void buttonClicked();
    void checkLineEdit();

protected:
    QString _programm;
    QString _path;
    QString _versionArg;

    void setupWidgets();
    QLineEdit *_pathLineEdit;
    QToolButton *_button;
    QLabel *_labelVersion;

    QFileSystemModel *_filesModel;
    QProcessEnvironment _env;
};

#endif  // PATHEDITWIDGET_H
