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
    _idColor = 0;

    _iconProvider = new ProjectIconProvider();

    tabBar()->installEventFilter(this);
    registerAction();

    connect(this, &QTabWidget::tabCloseRequested, this, &EditorTabWidget::closeEditor);
    connect(this, &QTabWidget::currentChanged, this, &EditorTabWidget::activeTab);
}

void EditorTabWidget::addEditor(Editor *editor)
{
    QFileInfo info(editor->filePath());
    const QString &path = info.absoluteFilePath();
    int index = count();
    QColor color;
    for (int i=count()-1; i>=0; i--)
    {
        QFileInfo infoEditor(this->editor(i)->filePath());
        if (infoEditor.absoluteDir() == info.absoluteDir())
        {
            color = tabBar()->tabTextColor(i);
            if (!color.isValid())
            {
                _idColor++;
                color = QColor::fromHsv(_idColor * 30, 255, 255);
                tabBar()->setTabTextColor(i, color);
            }
            index = i + 1;
            break;
        }
    }
    insertTab(index, editor, _iconProvider->icon(info), editor->fileName());
    if (color.isValid())
        tabBar()->setTabTextColor(index, color);
    setCurrentIndex(index);
    connect(editor, &Editor::filePathChanged, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::modified, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::statusChanged, this, &EditorTabWidget::changeStatus);
    connect(editor, &Editor::undoAvailable, this, &EditorTabWidget::undoUpdate);
    connect(editor, &Editor::redoAvailable, this, &EditorTabWidget::redoUpdate);
    connect(editor, &Editor::copyAvailable, this, &EditorTabWidget::copyUpdate);
    _mapPathEditor.insert(path, editor);
    _project->addOpenedFiles(QSet<QString>()<<path);
}

Editor *EditorTabWidget::currentEditor() const
{
    return static_cast<Editor *>(currentWidget());
}

QString EditorTabWidget::currentFilePath() const
{
    if (currentEditor())
        return currentEditor()->filePath();
    return QString();
}

Editor *EditorTabWidget::editor(int i) const
{
    return static_cast<Editor *>(widget(i));
}

void EditorTabWidget::openFileEditor(const QString &url)
{
    QString filePath, line, column;
    QRegExp urlFilePath("(..[^:]*)(:[0-9]+)*(:[0-9]+)*");

    if (urlFilePath.indexIn(url) == -1)
        return;
    filePath = urlFilePath.cap(1);

    if (urlFilePath.captureCount()>1)
        line = urlFilePath.cap(2).mid(1);
    if (urlFilePath.captureCount()>2)
        column = urlFilePath.cap(3).mid(1);

    if (!QFile(filePath).exists())
        filePath = _project->make()->resolveFilePath(filePath);
    if (!QFile(filePath).exists())
        return;

    Editor *editor = nullptr;
    if(!_mapPathEditor.contains(QFileInfo(filePath).absoluteFilePath()))
    {
        editor = Editor::createEditor(filePath, _project);
        if (!editor)
            return;
        addEditor(editor);
    }
    else
    {
        editor = _mapPathEditor.value(filePath);
        int id = indexOf(editor);
        if (id == -1)
            return;

        setCurrentIndex(id);
    }

    if (!line.isEmpty())
    {
        if (column.isEmpty())
            editor->gotoLine(line.toInt());
        else
            editor->gotoLine(line.toInt(), column.toInt());
    }
    editor->active();
}

void EditorTabWidget::closeFileEditor(const QString &filePath)
{
    QFileInfo infoPath(filePath);
    QList<Editor*> editors;

    if (infoPath.isDir())
    {
        QDir dir(infoPath.absoluteFilePath());
        QMap<QString, Editor* >::const_iterator it, end;
        for (it = _mapPathEditor.cbegin(), end = _mapPathEditor.cend(); it != end; ++it)
        {
            QFileInfo editorInfo(it.key());
            if (editorInfo.absoluteDir() == dir)
                editors.append(it.value());
        }
        foreach (Editor *editor, editors)
        {
            int id = indexOf(editor);
            closeEditor(id);
        }
    }
    else
    {
        if(_mapPathEditor.contains(infoPath.absoluteFilePath()))
        {
            Editor *editor = _mapPathEditor.value(infoPath.absoluteFilePath());
            int id = indexOf(editor);
            closeEditor(id);
        }
    }
}

int EditorTabWidget::closeEditor(int id)
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
        return -1;

    const QString &path = QFileInfo(editor->filePath()).absoluteFilePath();
    if (editor->isModified())
    {
        int response = QMessageBox::question(this, tr("File modified"),
                                             tr("File '%1' has been modified, do you want to save it?").arg(editor->fileName()),
                                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort,
                                             QMessageBox::Yes);
        if (response == QMessageBox::Abort)
            return -2;
        if (response == QMessageBox::Yes)
            editor->saveFile();
    }
    _mapPathEditor.remove(path);
    _project->removeOpenedFiles(QSet<QString>()<<path);
    removeTab(id);
    delete editor;
    return 0;
}

int EditorTabWidget::closeAllEditors()
{
    while (count() > 0)
    {
        setCurrentIndex(0);
        if (closeEditor(0) < 0)
            return -1;
    }
    return 0;
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
            if(editor(i)->saveFile()==-1)
                return;
}

void EditorTabWidget::switchHeader()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;

    QString filePathPair;
    QString filePath = editor->filePath();
    if (filePath.endsWith(".h"))
    {
        filePathPair = filePath.replace(QRegExp("\\.h$"), ".c");
        if (QFile::exists(filePathPair))
            openFileEditor(filePathPair);
    }
    else if (filePath.endsWith(".c"))
    {
        filePathPair = filePath.replace(QRegExp("\\.c$"), ".h");
        if (QFile::exists(filePathPair))
            openFileEditor(filePathPair);
    }
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

void EditorTabWidget::undo()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;
    editor->undo();
}

void EditorTabWidget::redo()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;
    editor->redo();
}

void EditorTabWidget::cut()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;
    editor->cut();
}

void EditorTabWidget::copy()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;
    editor->copy();
}

void EditorTabWidget::paste()
{
    Editor *editor = currentEditor();
    if (!editor)
        return;
    editor->paste();
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

void EditorTabWidget::changeStatus(QString status)
{
    emit statusChanged(status);
}

void EditorTabWidget::undoUpdate(bool available)
{
    emit undoAvailable(available);
}

void EditorTabWidget::redoUpdate(bool available)
{
    emit redoAvailable(available);
}

void EditorTabWidget::copyUpdate(bool available)
{
    emit copyAvailable(available);
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
