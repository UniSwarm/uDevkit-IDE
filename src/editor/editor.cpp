#include "editor.h"

#include "editors.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
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
    return saveFileData(_filePath);
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

void Editor::setFilePath(const QString &filePath)
{
    if (filePath != _filePath)
    {
        _filePath = QFileInfo(filePath).absoluteFilePath();
        emit filePathChanged(_filePath);
    }
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
        editor->openFileData(filePath);
    return editor;
}
