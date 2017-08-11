#ifndef PROJECT_H
#define PROJECT_H

#include <QDir>
#include <QObject>

#include "version/abstractversioncontrol.h"
#include "make/makeparser.h"

#include "projectitemmodel.h"

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(const QString &path=QString());
    ~Project();

    const QDir &rootDir() const;
    QString rootPath() const;
    void setRootPath(const QString &path);

    AbstractVersionControl *versionControl() const;

    ProjectItemModel *projectItemModel() const;

    bool isOpenedFile(const QString &path) const;
    const QSet<QString> &openedFiles() const;
    void addOpenedFiles(QSet<QString> openedFiles);
    void removeOpenedFiles(QSet<QString> openedFiles);

    MakeParser *make() const;

signals:
    void rootPathChanged();

public slots:

protected slots:
    void updateSources();
    void newSource(QSet<QString> sources);
    void oldSource(QSet<QString> sources);

protected:
    QDir _rootDir;
    AbstractVersionControl *_versionControl;
    QSet<QString> _openedFiles;
    MakeParser *_make;

    ProjectItemModel *_projectItemModel;
};

#endif // PROJECT_H
