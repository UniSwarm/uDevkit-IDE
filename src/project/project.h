#ifndef PROJECT_H
#define PROJECT_H

#include <QDir>
#include <QObject>

#include "version/abstractversioncontrol.h"

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(const QString &path=QString());

    const QDir &rootDir() const;
    QString rootPath() const;
    void setRootPath(const QString &path);

    AbstractVersionControl *versionControl() const;

signals:

public slots:

protected:
    QDir _rootDir;
    AbstractVersionControl *_versionControl;
};

#endif // PROJECT_H
