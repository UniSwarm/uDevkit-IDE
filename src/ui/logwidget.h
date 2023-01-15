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

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTextBrowser>

#include <QProcess>

class Project;

class LogWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit LogWidget(Project *project, QWidget *parent = nullptr);
    ~LogWidget() override;

signals:
    void openFileRequested(QString filePath);

public slots:
    void start(const QString &program, const QStringList &arguments);

protected:
    Project *_project;
    QProcess *_process;
    QString _color;
    void parseOutput(QByteArray data, bool error);

protected slots:
    void readProcess();
    void errorProcess();
    void finishProcess(int errorCode, QProcess::ExitStatus exitStatus);
    void anchorClick(const QUrl &link);
    void append(const QString &string);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif  // LOGWIDGET_H
