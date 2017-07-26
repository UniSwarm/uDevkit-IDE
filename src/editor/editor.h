#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);

    virtual int openFile(const QString &filePath) =0;

    QString fileName() const;
    QString filePath() const;

    virtual bool modified() const =0;

signals:
    void filePathChanged(QString filePath);
    void modified(bool modified);

public:
    enum Type {
        Code,
        Hexa,
        Image,
        ELF
    };
    static Editor *createEditor(Type type, QWidget *parent = nullptr);
    static Editor *createEditor(const QString &filePath, QWidget *parent = nullptr);

protected:
    void setFilePath(const QString &filePath);
    QString _filePath;
};

#endif // EDITOR_H
