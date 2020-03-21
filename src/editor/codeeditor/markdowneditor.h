#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include "htmleditor.h"

class MarkdownEditor : public HtmlEditor
{
    Q_OBJECT
public:
    MarkdownEditor(Project *project, QWidget *parent = nullptr);

protected slots:
    void updatePreview() override;

protected:
};

#endif // MARKDOWNEDITOR_H
