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

#endif // PATHEDITWIDGET_H
