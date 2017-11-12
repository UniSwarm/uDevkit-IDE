#ifndef CODEEDITORMARGINDELEGATE_H
#define CODEEDITORMARGINDELEGATE_H

#include "edbee/views/components/textmargincomponent.h"

#include "version/fileversionchange.h"

class CodeEditorMarginDelegate : public edbee::TextMarginComponentDelegate
{
public:
    CodeEditorMarginDelegate();
    virtual ~CodeEditorMarginDelegate();

    FileVersionChange *fileChange() const;
    void setFileChange(FileVersionChange *fileChange);

    // TextMarginComponentDelegate interface
public:
    virtual int widthBeforeLineNumber();
    virtual void renderAfter(QPainter *painter, int startLine, int endLine, int width);

protected:
    FileVersionChange *_fileChange;

    // TextMarginComponentDelegate interface
public:
    virtual void mousePressEvent(int line, QMouseEvent *event);
};

#endif // CODEEDITORMARGINDELEGATE_H
