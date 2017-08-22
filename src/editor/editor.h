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

    virtual void gotoLine(int x, int y=-1);

    enum Type {
        Code,
        Hexa,
        HTML,
        Image,
        ELF
    };
    virtual Type type() const =0;

    // search replace capabilities
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
    bool hasReplace() const;

    // search / replace interface
    enum SearchFlag {
        NoFlag        = 0x0000,
        RegExpMode    = 0x0001,
        CaseSensitive = 0x0002,
        HighlightAll  = 0x0004
    };
    Q_DECLARE_FLAGS(SearchFlags, SearchFlag)
    virtual int search(const QVariant &searchTerm, SearchFlags flags=SearchFlag(NoFlag));
    virtual void searchNext();
    virtual void searchPrev();
    virtual void searchSelectAll();

    virtual void replaceAll(const QVariant &replacePattern, SearchFlags flags=SearchFlag(NoFlag));

    // preview interface
    virtual QWidget *previewWidget() const;
    virtual bool hasPreview() const;

public slots:
    void reload();
    void active();

protected slots:
    void prepareReload();

signals:
    void filePathChanged(QString filePath);
    void modified(bool modified);
    void helpRequest(QString term);

public:
    static Editor *createEditor(Type type, Project *project, QWidget *parent = nullptr);
    static Editor *createEditor(const QString &filePath, Project *project, QWidget *parent = nullptr);
    static Type typeFromExt(const QString &ext);

protected:
    void setFilePath(const QString &filePath);
    virtual void initialiseWidget();
    virtual void giveFocus();

    Project *_project;
    QString _filePath;
    QFileSystemWatcher *_reloadWatcher;
    bool _extModifDetected;

    virtual int openFileData(const QString &filePath) =0;
    virtual int saveFileData(const QString &filePath=QString()) =0;
};

#endif // EDITOR_H
