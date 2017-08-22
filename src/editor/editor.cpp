#include "editor.h"

#include "editors.h"
#include "editortabwidget.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QTimer>

Editor::Editor(Project *project, QWidget *parent)
    : QWidget(parent), _project(project)
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
        return QString("new file");
    return QFileInfo(_filePath).fileName();
}

QString Editor::filePath() const
{
    return _filePath;
}

int Editor::saveFile(const QString &filePath)
{
    QString path = filePath.isEmpty() ? _filePath : filePath;
    if (path.isEmpty())
    {
        path = QFileDialog::getSaveFileName(this, tr("Save file"));
        if (path.isEmpty())
            return -1;
    }
    setFilePath(path);
    _reloadWatcher->removePath(_filePath);
    int ret = saveFileData(_filePath);
    _project->versionControl()->modifFile(QSet<QString>()<<_filePath);
    _reloadWatcher->addPath(_filePath);
    return ret;
}

bool Editor::isActiveEditor() const
{
    if (!parent())
        return false;
    if (!parent()->parent())
        return false;

    EditorTabWidget *tabWidget = static_cast<EditorTabWidget*>(parent()->parent());
    if (!tabWidget)
        return false;
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

void Editor::reload()
{
    QFileInfo info(_filePath);
    if (!info.exists() || info.size()==0)
    {
        _reloadWatcher->addPath(info.path());
        return;
    }

    bool needReload = true;
    _extModifDetected = true;

    if (!isActiveEditor())
        return;

    if (isModified())
    {
        int response = QMessageBox::question(this, tr("File externally modified"),
                                             tr("File '%1' has been externally modified, do you want to reload it and lost your modifications?").arg(fileName()),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::Yes);
        if (response == QMessageBox::No)
            needReload = false;
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
        reload();

    initialiseWidget();
    giveFocus();
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
            _reloadWatcher->removePath(_filePath);
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

Editor *Editor::createEditor(Editor::Type type, Project *project, QWidget *parent)
{
    switch (type)
    {
    case Editor::Code:
        return new CodeEditor(project, parent);
    case Editor::Hexa:
        return new HexEditor(project, parent);
    case Editor::HTML:
        return new HtmlEditor(project, parent);
    case Editor::Image:
        return new ImageEditor(project, parent);
    case Editor::ELF:
        break;
    }
    return nullptr;
}

Editor *Editor::createEditor(const QString &filePath, Project *project, QWidget *parent)
{
    QMimeDatabase db;
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QMimeType mime = db.mimeTypeForFileNameAndData(filePath, file.read(100));
    file.close();
    //qDebug()<<mime;

    Type type;
    if(mime.name() == "text/html")
        type = Editor::HTML;
    else if(mime.name().startsWith("text") || mime.name() == "application/xml" || mime.name() == "application/x-yaml")
        type = Editor::Code;
    else if(mime.name().startsWith("image"))
        type = Editor::Image;
    else
        type = Editor::Hexa;

    Editor *editor;
    editor = createEditor(type, project, parent);
    if (editor)
        editor->openFileData(filePath);
    return editor;
}

Editor::Type Editor::typeFromExt(const QString &ext)
{
    if (ext == "o")
        return Editor::Hexa;
    return Editor::Code;
}
