/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2021 UniSwarm
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "editortabwidget.h"

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabBar>
#include <utility>

EditorTabWidget::EditorTabWidget(Project *project)
    : _project(project)
{
    setTabsClosable(true);
    setDocumentMode(true);
    setMovable(true);
    setUsesScrollButtons(true);
    setFocusPolicy(Qt::NoFocus);
    _idColor = 0;

    // remove bottom border, fixed tabTextColor overriden
    setStyleSheet("\
      EditorTabWidget::pane {\
          border: 0 solid #76797C;\
          padding: -3px 0px 0px 0px;\
      }\
      EditorTabWidget > QTabBar::tab:top {\
          color: palette(foreground);\
          border-top: 1px solid #76797C;\
          border-left: 1px solid #76797C;\
          border-right: 1px solid #76797C;\
          background-color: #232629;\
      }\
      EditorTabWidget > QTabBar::tab:top:!selected\
      {\
          color: palette(foreground);\
          background-color: #54575B;\
          border-top: 1px solid #76797C;\
          border-left: 1px solid #76797C;\
          border-right: 1px solid #76797C;\
      }\
    ");

    _iconProvider = new ProjectIconProvider();

    tabBar()->installEventFilter(this);
    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar(), &QTabBar::customContextMenuRequested, this, &EditorTabWidget::tabContextMenu);
    registerAction();

    _switchTabListWidget = new EditorTabSwitchWidget(this);
    _switchTabListWidget->hide();
    _switchTabListWidget->setFocusPolicy(Qt::ClickFocus);
    _switchTabActive = false;
    _switchTabListWidget->installEventFilter(this);

    connect(this, &QTabWidget::tabCloseRequested, this, &EditorTabWidget::closeEditor);
    connect(this, &QTabWidget::currentChanged, this, &EditorTabWidget::activeTab);
}

void EditorTabWidget::addEditor(Editor *editor)
{
    QFileInfo info(editor->filePath());
    const QString &path = info.absoluteFilePath();
    int index = count();
    QColor color;
    for (int i = count() - 1; i >= 0; i--)
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
    editor->editorWidget()->installEventFilter(this);
    if (color.isValid())
    {
        tabBar()->setTabTextColor(index, color);
    }
    setCurrentIndex(index);
    connect(editor, &Editor::filePathChanged, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::modified, this, &EditorTabWidget::updateTab);
    connect(editor, &Editor::statusChanged, this, &EditorTabWidget::changeStatus);
    connect(editor, &Editor::undoAvailable, this, &EditorTabWidget::undoUpdate);
    connect(editor, &Editor::redoAvailable, this, &EditorTabWidget::redoUpdate);
    connect(editor, &Editor::copyAvailable, this, &EditorTabWidget::copyUpdate);
    _mapPathEditor.insert(path, editor);
    _project->addOpenedFiles(QSet<QString>() << path);
}

Editor *EditorTabWidget::currentEditor() const
{
    return dynamic_cast<Editor *>(currentWidget());
}

QString EditorTabWidget::currentFilePath() const
{
    if (currentEditor() != nullptr)
    {
        return currentEditor()->filePath();
    }
    return QString();
}

Editor *EditorTabWidget::editor(int i) const
{
    return dynamic_cast<Editor *>(widget(i));
}

void EditorTabWidget::openFileEditor(const QString &url)
{
    QString filePath;
    QString line;
    QString column;
    QRegExp urlFilePath("(..[^:]*)(:[0-9]+)*(:[0-9]+)*");

    if (urlFilePath.indexIn(url) == -1)
    {
        return;
    }
    filePath = urlFilePath.cap(1);

    if (urlFilePath.captureCount() > 1)
    {
        line = urlFilePath.cap(2).mid(1);
    }
    if (urlFilePath.captureCount() > 2)
    {
        column = urlFilePath.cap(3).mid(1);
    }

    if (!QFile(filePath).exists())
    {
        filePath = _project->make()->resolveFilePath(filePath);
        if (!QFile(filePath).exists())
        {
            return;
        }
    }

    // find build
    /*MakeRule rule = _project->make()->buildRule(filePath);
    qDebug() << rule.target << rule.dependencies;

    qDebug() << "";
    qDebug() << "targets";
    QList<MakeRule> targets = _project->make()->targets();
    foreach(const MakeRule &arule, targets)
        qDebug() << arule.target << arule.dependencies;

    qDebug() << "";
    qDebug() << "includedInRules";
    QList<MakeRule> rules = _project->make()->includedInRules(filePath);
    foreach(const MakeRule &arule, rules)
        qDebug() << arule.target << arule.dependencies;*/

    Editor *editor = nullptr;
    if (!_mapPathEditor.contains(QFileInfo(filePath).absoluteFilePath()))
    {
        editor = Editor::createEditor(filePath, _project);
        if (editor == nullptr)
        {
            return;
        }
        addEditor(editor);
    }
    else
    {
        editor = _mapPathEditor.value(filePath);
        int id = indexOf(editor);
        if (id == -1)
        {
            return;
        }

        setCurrentIndex(id);
    }

    if (!line.isEmpty())
    {
        if (column.isEmpty())
        {
            editor->gotoLine(line.toInt());
        }
        else
        {
            editor->gotoLine(line.toInt(), column.toInt());
        }
    }
    editor->active();
}

void EditorTabWidget::closeFileEditor(const QString &filePath)
{
    QFileInfo infoPath(filePath);
    QList<Editor *> editors;

    if (infoPath.isDir())
    {
        QDir dir(infoPath.absoluteFilePath());
        QMap<QString, Editor *>::const_iterator it;
        QMap<QString, Editor *>::const_iterator end;
        for (it = _mapPathEditor.cbegin(), end = _mapPathEditor.cend(); it != end; ++it)
        {
            QFileInfo editorInfo(it.key());
            if (editorInfo.absoluteDir() == dir)
            {
                editors.append(it.value());
            }
        }
        foreach (Editor *editor, editors)
        {
            int id = indexOf(editor);
            closeEditor(id);
        }
    }
    else
    {
        if (_mapPathEditor.contains(infoPath.absoluteFilePath()))
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
    {
        editor = dynamic_cast<Editor *>(widget(id));
    }
    if (editor == nullptr)
    {
        return -1;
    }

    const QString &path = QFileInfo(editor->filePath()).absoluteFilePath();
    if (editor->isModified())
    {
        int response = QMessageBox::question(this,
                                             tr("File modified"),
                                             tr("File '%1' has been modified, do you want to save it?").arg(editor->fileName()),
                                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort,
                                             QMessageBox::Yes);
        if (response == QMessageBox::Abort)
        {
            return -2;
        }
        if (response == QMessageBox::Yes)
        {
            editor->saveFile();
        }
    }
    _mapPathEditor.remove(path);
    _project->removeOpenedFiles(QSet<QString>() << path);
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
        {
            return -1;
        }
    }
    return 0;
}

void EditorTabWidget::saveCurrentEditor() const
{
    Editor *editor = currentEditor();
    if (editor != nullptr)
    {
        editor->saveFile();
    }
}

void EditorTabWidget::saveAllEditors()
{
    for (int i = 0; i < count(); i++)
    {
        if (editor(i)->isModified())
        {
            if (editor(i)->saveFile() == -1)
            {
                return;
            }
        }
    }
}

void EditorTabWidget::switchHeader()
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }

    QString filePathPair;
    QString filePath = editor->filePath();
    if (filePath.endsWith(".h"))
    {
        filePathPair = filePath;
        filePathPair.replace(QRegExp("\\.h$"), ".c");
        if (QFile::exists(filePathPair))
        {
            openFileEditor(filePathPair);
        }
        filePathPair = filePath;
        filePathPair.replace(QRegExp("\\.h$"), ".cpp");
        if (QFile::exists(filePathPair))
        {
            openFileEditor(filePathPair);
        }
    }
    else if (filePath.endsWith(".c") || filePath.endsWith(".cpp"))
    {
        filePathPair = filePath.replace(QRegExp("\\.c(pp)?$"), ".h");
        if (QFile::exists(filePathPair))
        {
            openFileEditor(filePathPair);
        }
    }
}

void EditorTabWidget::initiateSwitchTab(bool next)
{
    if (count() < 2)
    {
        return;
    }

    // fill list
    _switchTabListWidget->clear();
    foreach (int id, _activedTab)
    {
        Editor *editor = this->editor(id);
        if (editor == nullptr)
        {
            continue;
        }
        QFileInfo info(editor->filePath());
        QListWidgetItem *item = new QListWidgetItem(_iconProvider->icon(info), editor->fileName());
        item->setData(Qt::UserRole, id);
        QColor textColor = tabBar()->tabTextColor(id);
        if (!textColor.isValid())
        {
            textColor = palette().color(QPalette::Foreground);
        }
        item->setData(Qt::ForegroundRole, textColor);
        _switchTabListWidget->addItem(item);
    }
    _switchTabListWidget->setId(next ? 1 : count() - 1);

    // show list
    updateSwitchTab();
    _switchTabListWidget->show();
    _switchTabListWidget->update();
    _switchTabListWidget->setFocus(Qt::PopupFocusReason);
    _switchTabActive = true;
}

void EditorTabWidget::nextTab()
{
    if (_activedTab.count() < 2)
    {
        return;
    }
    setCurrentIndex(_activedTab[1]);
    // TODO improve me
}

void EditorTabWidget::previousTab()
{
    // TODO implement me
}

void EditorTabWidget::endSwitchTab()
{
    if (!_switchTabActive)
    {
        return;
    }
    _switchTabActive = false;
    _switchTabListWidget->hide();
    int id = _switchTabListWidget->id();
    if (id == 0)
    {
        currentEditor()->editorWidget()->setFocus();
    }
    else
    {
        int tabId = _switchTabListWidget->item(id)->data(Qt::UserRole).toInt();
        setCurrentIndex(tabId);
        currentEditor()->editorWidget()->setFocus();
    }
}

void EditorTabWidget::undo() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->undo();
}

void EditorTabWidget::redo() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->redo();
}

void EditorTabWidget::cut() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->cut();
}

void EditorTabWidget::copy() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->copy();
}

void EditorTabWidget::paste() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->paste();
}

void EditorTabWidget::format() const
{
    Editor *editor = currentEditor();
    if (editor == nullptr)
    {
        return;
    }
    editor->format();
}

void EditorTabWidget::updateTab()
{
    Editor *editor = qobject_cast<Editor *>(sender());
    if (editor == nullptr)
    {
        return;
    }

    int id = indexOf(editor);
    if (id == -1)
    {
        return;
    }

    QString tabText = editor->fileName();
    if (editor->isModified())
    {
        tabText.append(" *");
    }
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
    {
        _activedTab.removeAt(pos);
    }
    _activedTab.prepend(id);
    // qDebug()<<_activedTab;

    Editor *editor = this->editor(id);
    emit editorChange(editor);
    if (editor != nullptr)
    {
        currentFileChanged(editor->filePath());
        editor->active();
    }
}

void EditorTabWidget::tabContextMenu(const QPoint &pos)
{
    int index = tabBar()->tabAt(pos);
    if (index == -1)
    {
        return;
    }

    QString path = editor(index)->filePath();
    setCurrentIndex(index);

    QMenu menu;
    QAction *reopen;
    Editor::Type type;
    if (currentEditor()->type() != Editor::Hexa)
    {
        type = Editor::Hexa;
        reopen = menu.addAction(tr("Open in hex editor"));
    }
    else
    {
        type = Editor::typeFromPath(path);
        reopen = menu.addAction(tr("Open in default editor"));
    }
    QAction *close = menu.addAction(tr("Close"));

    QAction *action = menu.exec(tabBar()->mapToGlobal(pos));
    if (action == close)
    {
        closeEditor(index);
    }
    else if (action == reopen)
    {
        QColor tabColor = tabBar()->tabTextColor(index);
        closeEditor(index);
        Editor *editor = Editor::createEditor(type, _project, this);
        editor->openFile(path);
        insertTab(index, editor, editor->fileName());
        editor->editorWidget()->installEventFilter(this);
        tabBar()->setTabTextColor(index, tabColor);
        setCurrentIndex(index);
    }
}

void EditorTabWidget::changeStatus(const QString &status)
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
    if (e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(e);
        if (!_switchTabActive && e->type() == QEvent::KeyPress && ((keyEvent->modifiers() & Qt::ControlModifier) != 0u)
            && (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab))
        {
            initiateSwitchTab(!(keyEvent->modifiers() & Qt::ShiftModifier));
        }
        else if (_switchTabActive && e->type() == QEvent::KeyRelease && keyEvent->modifiers() == Qt::NoModifier)
        {
            endSwitchTab();
        }
        return QObject::eventFilter(o, e);
    }
    if (o == tabBar() && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(e);
        if (mouseEvent->button() == Qt::MiddleButton)
        {
            closeEditor(tabBar()->tabAt(mouseEvent->pos()));
            return true;
        }
    }
    else if (o == _switchTabListWidget)
    {
        if (e->type() == QEvent::FocusOut)
        {
            endSwitchTab();
        }

        return QObject::eventFilter(o, e);
    }
    return QTabWidget::eventFilter(o, e);
}

void EditorTabWidget::registerAction()
{
    QAction *action;

    action = new QAction(tr("save"), this);
    action->setShortcut(QKeySequence::Save);
    connect(action, &QAction::triggered, this, &EditorTabWidget::saveCurrentEditor);
    addAction(action);

    action = new QAction(tr("save all"), this);
    action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(action, &QAction::triggered, this, &EditorTabWidget::saveAllEditors);
    addAction(action);

    action = new QAction(tr("close tab"), this);
    action->setShortcut(QKeySequence::Close);
    connect(action, &QAction::triggered, this, &EditorTabWidget::closeEditor);
    addAction(action);
}

void EditorTabWidget::updateSwitchTab()
{
    QRect mgeometry = _switchTabListWidget->geometry();
    mgeometry.setWidth(qMin(200, width() - 20));
    mgeometry.setHeight(qMin(30 * 8, height() - 20));
    mgeometry.moveLeft((size().width() - mgeometry.width()) / 2);
    mgeometry.moveTop((size().height() - mgeometry.height()) / 2);
    _switchTabListWidget->setGeometry(mgeometry);
}

void EditorTabWidget::resizeEvent(QResizeEvent *event)
{
    if (_switchTabActive)
    {
        updateSwitchTab();
    }
    QTabWidget::resizeEvent(event);
}

void EditorTabWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);  // disable QTabWidget Ctrl + tab
}
