#include "editor.h"

#include "editors.h"
#include "editortabwidget.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    _reloadWatcher = new QFileSystemWatcher();
    connect(_reloadWatcher, &QFileSystemWatcher::fileChanged, this, &Editor::reload);
    _extModifDetected = false;
}

Editor::~Editor()
{
    _reloadWatcher->deleteLater();
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

void Editor::reload()
{
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
    _reloadWatcher->addPath(_filePath);
}

void Editor::active()
{
    if (_extModifDetected)
        reload();
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

Editor *Editor::createEditor(Editor::Type type, QWidget *parent)
{
    switch (type)
    {
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
        editor->openFileData(filePath);
    return editor;
}
