#ifndef PROJECTICONPROVIDER_H
#define PROJECTICONPROVIDER_H

#include <QFileIconProvider>

#include <QMap>
#include <QStringList>
#include <QIcon>

class ProjectIconProvider : public QFileIconProvider
{
public:
    ProjectIconProvider();

    // QFileIconProvider interface
public:
    virtual QIcon icon(const QFileInfo &info) const;

protected:
    QMap<QString, QIcon> _icons;
};

#endif // PROJECTICONPROVIDER_H
