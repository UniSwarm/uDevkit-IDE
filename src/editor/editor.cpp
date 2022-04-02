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

#include "editor.h"

#include "editors.h"
#include "editortabwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QTimer>

Editor::Editor(Project *project, QWidget *parent)
    : QWidget(parent)
    , _project(project)
{
    _reloadWatcher = new QFileSystemWatcher();
    connect(_reloadWatcher, &QFileSystemWatcher::fileChanged, this, &Editor::prepareReload);
    connect(_reloadWatcher, &QFileSystemWatcher::directoryChanged, this, &Editor::prepareReload);
    _extModifDetected = false;
}

Editor::~Editor()
{
    delete _reloadWatcher;
    disconnect();
}

QString Editor::fileName() const
{
    if (_filePath.isEmpty())
    {
        return QString(tr("new file"));
    }
    return QFileInfo(_filePath).fileName();
}

QString Editor::filePath() const
{
    return _filePath;
}

int Editor::openFile(const QString &filePath)
{
    setFilePath(filePath);
    openFileData(filePath);
    return 0;
}

int Editor::saveFile(const QString &filePath)
{
    QString path = filePath.isEmpty() ? _filePath : filePath;
    if (path.isEmpty())
    {
        path = QFileDialog::getSaveFileName(this, tr("Save file"));
        if (path.isEmpty())
        {
            return -1;
        }
    }
    setFilePath(path);
    _reloadWatcher->removePath(_filePath);
    int ret = saveFileData(_filePath);
    _project->versionControl()->modifFile(QSet<QString>() << _filePath);
    _reloadWatcher->addPath(_filePath);
    return ret;
}

bool Editor::isActiveEditor() const
{
    if (parent() == nullptr)
    {
        return false;
    }
    if (parent()->parent() == nullptr)
    {
        return false;
    }

    EditorTabWidget *tabWidget = dynamic_cast<EditorTabWidget *>(parent()->parent());
    if (tabWidget == nullptr)
    {
        return false;
    }
    return (tabWidget->currentEditor() == this);
}

void Editor::gotoLine(int x, int y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
}

Editor::SearchCaps Editor::searchCap() const
{
    return NoSearch;
}

bool Editor::hasResearch() const
{
    return searchCap().testFlag(HasSearch);
}

bool Editor::hasRegExp() const
{
    return searchCap().testFlag(HasRegexp);
}

bool Editor::hasReplace() const
{
    return searchCap().testFlag(HasReplace);
}

int Editor::search(const QVariant &search, SearchFlags flags)
{
    Q_UNUSED(search)
    Q_UNUSED(flags)
    return 0;
}

void Editor::searchNext()
{
}

void Editor::searchPrev()
{
}

void Editor::searchSelectAll()
{
}

void Editor::replace(const QVariant &replacePattern, SearchFlags flags, bool next)
{
    Q_UNUSED(replacePattern)
    Q_UNUSED(flags)
    Q_UNUSED(next)
}

void Editor::replaceAll(const QVariant &replacePattern, SearchFlags flags)
{
    Q_UNUSED(replacePattern)
    Q_UNUSED(flags)
}

QWidget *Editor::previewWidget() const
{
    return nullptr;
}

bool Editor::hasPreview() const
{
    return false;
}

QWidget *Editor::editorWidget()
{
    return this;
}

void Editor::reload()
{
    QFileInfo info(_filePath);
    if (!info.exists() || info.size() == 0)
    {
        _reloadWatcher->addPath(info.path());
        return;
    }

    bool needReload = true;
    _extModifDetected = true;

    if (!isActiveEditor())
    {
        return;
    }

    if (isModified())
    {
        int response = QMessageBox::question(this,
                                             tr("File externally modified"),
                                             tr("File '%1' has been externally modified, do you want to reload it and lost your modifications?").arg(fileName()),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::Yes);
        if (response == QMessageBox::No)
        {
            needReload = false;
        }
    }

    if (needReload)
    {
        openFileData(_filePath);
        _extModifDetected = false;
    }
    _reloadWatcher->removePath(info.path());
    _reloadWatcher->addPath(_filePath);
}

void Editor::active()
{
    if (_extModifDetected)
    {
        reload();
    }

    initialiseWidget();
    giveFocus();
}

void Editor::undo()
{
    undoCommand();
}

void Editor::redo()
{
    redoCommand();
}

void Editor::cut()
{
    cutCommand();
}

void Editor::copy()
{
    copyCommand();
}

void Editor::paste()
{
    pasteCommand();
}

void Editor::format()
{
    formatCommand();
}

void Editor::prepareReload()
{
    QTimer::singleShot(100, this, SLOT(reload()));
}

void Editor::setFilePath(const QString &filePath)
{
    if (filePath != _filePath)
    {
        if (!_filePath.isEmpty())
        {
            _reloadWatcher->removePath(_filePath);
        }
        _filePath = QFileInfo(filePath).absoluteFilePath();
        emit filePathChanged(_filePath);
        _reloadWatcher->addPath(_filePath);
    }
}

void Editor::initialiseWidget()
{
}

void Editor::giveFocus()
{
    setFocus();
}

void Editor::undoCommand()
{
}

void Editor::redoCommand()
{
}

void Editor::cutCommand()
{
}

void Editor::copyCommand()
{
}

void Editor::pasteCommand()
{
}

void Editor::formatCommand()
{
}

Editor *Editor::createEditor(Editor::Type type, Project *project, QWidget *parent)
{
    switch (type)
    {
    case Editor::Code:
        return new CodeEditor(project, parent);

    case Editor::HexFile:
        return new HexFileEditor(project, parent);

    case Editor::Hexa:
        return new HexEditor(project, parent);

    case Editor::HTML:
        return new HtmlEditor(project, parent);

    case Editor::Markdown:
        return new MarkdownEditor(project, parent);

    case Editor::Image:
        return new ImageEditor(project, parent);

    case Editor::ELF:
        break;
    }
    return nullptr;
}

Editor *Editor::createEditor(const QString &filePath, Project *project, QWidget *parent)
{
    Type type = typeFromPath(filePath);

    Editor *editor;
    editor = createEditor(type, project, parent);
    if (editor != nullptr)
    {
        editor->openFileData(filePath);
    }
    return editor;
}

Editor::Type Editor::typeFromExt(const QString &ext)
{
    if (ext == "o")
    {
        return Editor::Hexa;
    }
    return Editor::Code;
}

Editor::Type Editor::typeFromPath(const QString &filePath)
{
    QMimeDatabase db;
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QMimeType mime = db.mimeTypeForFileNameAndData(filePath, file.read(100));
    file.close();
    // qDebug() << mime;

    Type type;
    if (mime.name() == "text/x-hex" || filePath.endsWith(".hex"))
    {
        type = Editor::HexFile;
    }
    else if (mime.name() == "text/html")
    {
        type = Editor::HTML;
    }
    else if (mime.name().startsWith("text/markdown"))
    {
        type = Editor::Markdown;
    }
    else if (mime.name().startsWith("text") || mime.name() == "application/xml" || mime.name() == "application/x-yaml" || mime.name() == "application/x-shellscript" ||
             mime.name() == "application/javascript" || mime.name() == "application/json" || mime.name() == "application/x-php" || filePath.endsWith(".eds") || file.size() == 0)
    {
        type = Editor::Code;
    }
    else if (mime.name().startsWith("image"))
    {
        type = Editor::Image;
    }
    else
    {
        type = Editor::Hexa;
    }

    return type;
}
