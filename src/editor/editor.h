#ifndef EDITOR_H
#define EDITOR_H

#include <QVariant>
#include <QWidget>

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);

    QString fileName() const;
    QString filePath() const;

    int saveFile(const QString &filePath=QString());

    virtual bool isModified() const =0;

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
    virtual void nextSearch();
    virtual void prevSearch();

signals:
    void filePathChanged(QString filePath);
    void modified(bool modified);

public:
    static Editor *createEditor(Type type, QWidget *parent = nullptr);
    static Editor *createEditor(const QString &filePath, QWidget *parent = nullptr);

protected:
    void setFilePath(const QString &filePath);
    QString _filePath;

    virtual int openFileData(const QString &filePath) =0;
    virtual int saveFileData(const QString &filePath=QString()) =0;
};

#endif // EDITOR_H
