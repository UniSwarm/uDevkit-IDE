#ifndef CODEEDITORMARGINDELEGATE_H
#define CODEEDITORMARGINDELEGATE_H

#include "edbee/views/components/textmargincomponent.h"

#include "version/fileversionchange.h"

class CodeEditorMarginDelegate : public edbee::TextMarginComponentDelegate
{
public:
    CodeEditorMarginDelegate();
    virtual ~CodeEditorMarginDelegate();

    const FileVersionChange &fileChange() const;
    void setFileChange(const FileVersionChange &fileChange);

    // TextMarginComponentDelegate interface
public:
    virtual int widthBeforeLineNumber();
    virtual void renderBefore(QPainter *painter, int startLine, int endLine, int width);

protected:
    FileVersionChange _fileChange;
};

#endif // CODEEDITORMARGINDELEGATE_H
