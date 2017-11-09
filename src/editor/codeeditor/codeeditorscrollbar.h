#ifndef CODEEDITORSCROLLBAR_H
#define CODEEDITORSCROLLBAR_H

#include <QScrollBar>

#include "edbee/views/textrenderer.h"
#include "version/fileversionchange.h"

class CodeEditorScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CodeEditorScrollBar(QWidget *parent = Q_NULLPTR);
    explicit CodeEditorScrollBar(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);

    FileVersionChange fileChange() const;
    void setFileChange(const FileVersionChange &fileChange);

    edbee::TextRenderer *textRender() const;
    void setTextRender(edbee::TextRenderer *textRender);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

protected:
    FileVersionChange _fileChange;
    edbee::TextRenderer *_textRender;
};

#endif // CODEEDITORSCROLLBAR_H
