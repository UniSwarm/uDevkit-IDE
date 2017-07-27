#ifndef FILEPROJECTPROXYMODEL_H
#define FILEPROJECTPROXYMODEL_H

#include <QSortFilterProxyModel>

class Project;

class FileProjectProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FileProjectProxyModel(Project *project);

public slots:
    void setHiddenFilter(const QRegExp &regExp);
    void setHiddenFilter(const QString &pattern);
    void setShowFilter(const QRegExp &regExp);
    void setShowFilter(const QString &pattern);

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

protected:
    Project *_project;
    QRegExp _hiddenFilter;
    QRegExp _showFilter;
};

#endif // FILEPROJECTPROXYMODEL_H
