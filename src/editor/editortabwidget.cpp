#include "editortabwidget.h"

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabBar>

EditorTabWidget::EditorTabWidget(Project *project)
    : _project(project)
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
    connect(this, &QTabWidget::currentChanged, this, &EditorTabWidget::activeTab);
}

void EditorTabWidget::addEditor(Editor *editor)
{
    const QString &path = QFileInfo(editor->filePath()).absoluteFilePath();
    addTab(editor, editor->fileName());
    setCurrentIndex(count()-1);
    connect(editor, &Editor::filePathChanged, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::modified, this, &EditorTabWidget::updateTab);
    _mapPathEditor.insert(path, editor);
    _project->addOpenedFiles(QSet<QString>()<<path);
}

Editor *EditorTabWidget::currentEditor() const
{
    return static_cast<Editor *>(currentWidget());
}

Editor *EditorTabWidget::editor(int i) const
{
    return static_cast<Editor *>(widget(i));
}

void EditorTabWidget::openFileEditor(const QString &filePath)
{
    if(_mapPathEditor.contains(QFileInfo(filePath).absoluteFilePath()))
    {
        Editor *editor = _mapPathEditor.value(filePath);
        int id = indexOf(editor);
        if (id == -1)
            return;

        setCurrentIndex(id);
        return;
    }
    Editor *editor = Editor::createEditor(filePath, _project);
    if (editor)
        addEditor(editor);
}

void EditorTabWidget::closeFileEditor(const QString &filePath)
{
    if(_mapPathEditor.contains(QFileInfo(filePath).absoluteFilePath()))
    {
        Editor *editor = _mapPathEditor.value(filePath);
        int id = indexOf(editor);
        closeEditor(id);
    }
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

    const QString &path = QFileInfo(editor->filePath()).absoluteFilePath();
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
    _mapPathEditor.remove(path);
    _project->removeOpenedFiles(QSet<QString>()<<path);
    removeTab(id);
    delete editor;
}

void EditorTabWidget::closeAllEditors()
{
    while (count() > 0)
    {
        setCurrentIndex(0);
        closeEditor(0);
    }
}

void EditorTabWidget::saveCurrentEditor()
{
    Editor *editor = currentEditor();
    if (editor)
        editor->saveFile();
}

void EditorTabWidget::saveAllEditors()
{
    for (int i=0; i<count(); i++)
        if (editor(i)->isModified())
            editor(i)->saveFile();
}

void EditorTabWidget::nextTab()
{
    if(_activedTab.count() < 2)
        return;
    setCurrentIndex(_activedTab[1]);
    // TODO improve me
}

void EditorTabWidget::previousTab()
{
    // TODO implement me
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

    // update map of editors
    _mapPathEditor.remove(_mapPathEditor.key(editor));
    _mapPathEditor.insert(QFileInfo(editor->filePath()).absoluteFilePath(), editor);

    emit currentEditorModified(editor);
}

void EditorTabWidget::activeTab(int id)
{
    // update stack of active tab (ctrl + tab)
    int pos = _activedTab.indexOf(id);
    if (pos != -1)
        _activedTab.removeAt(pos);
    _activedTab.prepend(id);
    //qDebug()<<_activedTab;

    Editor *editor = this->editor(id);
    emit editorChange(editor);
    if (editor)
    {
        currentFileChanged(editor->filePath());
        editor->active();
    }
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
    connect(action, &QAction::triggered, this, &EditorTabWidget::saveCurrentEditor);
    addAction(action);

    action = new QAction(QString("save all"), this);
    action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(action, &QAction::triggered, this, &EditorTabWidget::saveAllEditors);
    addAction(action);

    action = new QAction(QString("close tab"), this);
    action->setShortcut(QKeySequence::Close);
    connect(action, &QAction::triggered, this, &EditorTabWidget::closeEditor);
    addAction(action);
}
