#include "editortabwidget.h"

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabBar>

EditorTabWidget::EditorTabWidget()
{
    setTabsClosable(true);
    setTabShape(QTabWidget::Triangular);
    setDocumentMode(true);
    setMovable(true);
    setUsesScrollButtons(true);
    setFocusPolicy(Qt::NoFocus);

    tabBar()->installEventFilter(this);
    registerAction();

    connect(this, &QTabWidget::tabCloseRequested, this, &EditorTabWidget::closeEditor);
}

void EditorTabWidget::addEditor(Editor *editor)
{
    addTab(editor, editor->fileName());
    setCurrentIndex(count()-1);
    connect(editor, &Editor::filePathChanged, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::modified, this, &EditorTabWidget::updateTab);
}

Editor *EditorTabWidget::currentEditor() const
{
    return static_cast<Editor *>(currentWidget());
}

void EditorTabWidget::addFileEditor(const QString &filePath)
{
    Editor *editor = Editor::createEditor(filePath);
    if (editor)
        addEditor(editor);
}

void EditorTabWidget::closeEditor(int id)
{
    Editor *editor;
    if (id == -1)
    {
        editor = currentEditor();
        id = currentIndex();
    }
    else
        editor = static_cast<Editor *>(widget(id));
    if (!editor)
        return;

    if (editor->isModified())
    {
        int response = QMessageBox::question(this, tr("File modified"),
                                             tr("File '%1' has been modified, do you want to save it?").arg(editor->fileName()),
                                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort,
                                             QMessageBox::Yes);
        if (response == QMessageBox::Abort)
            return;
        if (response == QMessageBox::Yes)
            editor->saveFile();
    }
    removeTab(id);
}

void EditorTabWidget::saveEditor()
{
    Editor *editor = currentEditor();
    if (editor)
        editor->saveFile();
}

void EditorTabWidget::updateTab()
{
    Editor* editor = qobject_cast<Editor*>(sender());
    if (editor == NULL)
        return;

    int id = indexOf(editor);
    if (id == -1)
        return;

    QString tabText = editor->fileName();
    if(editor->isModified())
        tabText.append(" *");
    setTabText(id, tabText);

}

bool EditorTabWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == tabBar() && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        if (mouseEvent->button() == Qt::MiddleButton)
        {
            closeEditor(tabBar()->tabAt(mouseEvent->pos()));
            return true;
        }
    }
    return QTabWidget::eventFilter(o, e);
}

void EditorTabWidget::registerAction()
{
    QAction *action;

    action = new QAction(QString("save"), this);
    action->setShortcut(QKeySequence::Save);
    connect(action, &QAction::triggered, this, &EditorTabWidget::saveEditor);
    addAction(action);

    action = new QAction(QString("close tab"), this);
    action->setShortcut(QKeySequence::Close);
    connect(action, &QAction::triggered, this, &EditorTabWidget::closeEditor);
    addAction(action);
}
