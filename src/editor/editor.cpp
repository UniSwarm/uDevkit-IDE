#include "editor.h"

#include "editors.h"

#include <QFileInfo>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
}

QString Editor::fileName() const
{
    return QFileInfo(_filePath).fileName();
}

QString Editor::filePath() const
{
    return _filePath;
}

void Editor::setFilePath(const QString &filePath)
{
    _filePath = filePath;
    emit filePathChanged(_filePath);
}

Editor *Editor::createEditor(Editor::Type type, QWidget *parent)
{
    switch (type) {
    case Editor::Code:
        return new CodeEditor(parent);
    case Editor::Hexa:
        break;
    case Editor::Image:
        break;
    case Editor::ELF:
        break;
    }
    return nullptr;
}

Editor *Editor::createEditor(const QString &filePath, QWidget *parent)
{
    Editor *editor;
    editor = createEditor(Editor::Code, parent);
    if (editor)
    {
        editor->openFile(filePath);
    }
    return editor;
}
