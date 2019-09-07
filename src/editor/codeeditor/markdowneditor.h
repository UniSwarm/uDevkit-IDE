#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include "htmleditor.h"

class MarkdownEditor : public HtmlEditor
{
    Q_OBJECT
public:
    MarkdownEditor(Project *project, QWidget *parent = 0);

protected slots:
    virtual void updatePreview() override;

protected:
};

#endif // MARKDOWNEDITOR_H
