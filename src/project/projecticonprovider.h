#ifndef PROJECTICONPROVIDER_H
#define PROJECTICONPROVIDER_H

#include <QFileIconProvider>

#include <QIcon>
#include <QMap>
#include <QStringList>

class ProjectIconProvider : public QFileIconProvider
{
public:
    ProjectIconProvider();

    // QFileIconProvider interface
public:
    QIcon icon(const QFileInfo &info) const override;

protected:
    QMap<QString, QIcon> _icons;
};

#endif // PROJECTICONPROVIDER_H
