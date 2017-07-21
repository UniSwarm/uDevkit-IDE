#ifndef FILEPROJECTPROXYMODEL_H
#define FILEPROJECTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QFileSystemModel>

class FileProjectProxyModel : public QSortFilterProxyModel
{
public:
    explicit FileProjectProxyModel();

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // FILEPROJECTPROXYMODEL_H
