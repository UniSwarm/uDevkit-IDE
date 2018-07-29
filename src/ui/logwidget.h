#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTextBrowser>

#include <QProcess>

class Project;

class LogWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit LogWidget(Project *project, QWidget *parent = Q_NULLPTR);
    virtual ~LogWidget();

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
    void anchorClick(const QUrl &link);
};

#endif // LOGWIDGET_H
