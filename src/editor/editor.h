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

    // research / replace interface
    virtual bool hasResearch() const;
    virtual void search(const QVariant &searchTerm);

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
