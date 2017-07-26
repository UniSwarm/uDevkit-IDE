#include "editortabwidget.h"

EditorTabWidget::EditorTabWidget()
{
    setTabsClosable(true);
    setTabShape(QTabWidget::Triangular);
    setDocumentMode(true);
    setMovable(true);
    setUsesScrollButtons(true);
    setFocusPolicy( Qt::NoFocus );
}

void EditorTabWidget::addEditor(Editor *editor)
{
    addTab(editor, editor->fileName());
    setCurrentIndex(count()-1);
}

void EditorTabWidget::addFileEditor(const QString &filePath)
{
    Editor *editor;
    editor = Editor::createEditor(filePath);
    if (editor)
        addEditor(editor);
}
