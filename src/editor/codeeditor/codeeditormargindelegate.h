#ifndef CODEEDITORMARGINDELEGATE_H
#define CODEEDITORMARGINDELEGATE_H

#include "edbee/views/components/textmargincomponent.h"

#include "version/fileversionchange.h"

class CodeEditorMarginDelegate : public edbee::TextMarginComponentDelegate
{
public:
    CodeEditorMarginDelegate();
    ~CodeEditorMarginDelegate() override;

    FileVersionChange *fileChange() const;
    void setFileChange(FileVersionChange *fileChange);

    // TextMarginComponentDelegate interface
public:
    int widthBeforeLineNumber() override;
    void renderAfter(QPainter *painter, int startLine, int endLine, int width) override;

protected:
    FileVersionChange *_fileChange;

    // TextMarginComponentDelegate interface
public:
    void mousePressEvent(int line, QMouseEvent *event) override;
};

#endif // CODEEDITORMARGINDELEGATE_H
