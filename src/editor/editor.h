#ifndef EDITOR_H
#define EDITOR_H

#include <QFileSystemWatcher>
#include <QVariant>
#include <QWidget>

#include "project/project.h"

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(Project *project, QWidget *parent = nullptr);
    virtual ~Editor();

    QString fileName() const;
    QString filePath() const;

    int saveFile(const QString &filePath=QString());

    virtual bool isModified() const =0;

    bool isActiveEditor() const;

    enum Type {
        Code,
        Hexa,
        Image,
        ELF
    };
    virtual Type type() const =0;

    // search / replace interface
    enum SearchCap {
        NoSearch   = 0x0000,
        HasSearch  = 0x0001,
        HasReplace = 0x0002,
        HasRegexp  = 0x0004
    };
    Q_DECLARE_FLAGS(SearchCaps, SearchCap)
    virtual SearchCaps searchCap() const;
    bool hasResearch() const;
    bool hasRegExp() const;

    enum SearchFlag {
        NoFlag        = 0x0000,
        RegExpMode    = 0x0001,
        HighlightAll  = 0x0002
    };
    Q_DECLARE_FLAGS(SearchFlags, SearchFlag)
    virtual int search(const QVariant &searchTerm, SearchFlags flags=SearchFlag(NoFlag));
    virtual void searchNext();
    virtual void searchPrev();
    virtual void searchSelectAll();

public slots:
    void reload(const QString &path=QString());
    void active();

signals:
    void filePathChanged(QString filePath);
    void modified(bool modified);

public:
    static Editor *createEditor(Type type, Project *project, QWidget *parent = nullptr);
    static Editor *createEditor(const QString &filePath, Project *project, QWidget *parent = nullptr);

protected:
    void setFilePath(const QString &filePath);

    Project *_project;
    QString _filePath;
    QFileSystemWatcher *_reloadWatcher;
    bool _extModifDetected;

    virtual int openFileData(const QString &filePath) =0;
    virtual int saveFileData(const QString &filePath=QString()) =0;
};

#endif // EDITOR_H
