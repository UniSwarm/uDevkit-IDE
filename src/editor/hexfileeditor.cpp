#include "hexfileeditor.h"

#include <QApplication>

#include "qhexedit.h"
#include "hexfile.h"

HexFileEditor::HexFileEditor(Project *project, QWidget *parent)
    : HexEditor(project, parent)
{

}

HexFileEditor::~HexFileEditor()
{

}

int HexFileEditor::openFileData(const QString &filePath)
{
    _file.setFileName(filePath);
    class HexFile hexFile(filePath);
    hexFile.read();
    _hexEditor->setData(hexFile.prog());
    _modified = false;
    emit modified(isModified());
    setFilePath(filePath);
    return 0;
}

int HexFileEditor::saveFileData(const QString &filePath)
{
    /*QString path = filePath.isEmpty() ? _filePath : filePath;
    QString tmpFileName = path + ".~tmp";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile file(tmpFileName);
    bool ok = _hexEditor->write(file);
    if (QFile::exists(path))
        ok = QFile::remove(path);
    if (ok)
    {
        file.setFileName(tmpFileName);
        ok = file.copy(path);
        if (ok)
            ok = QFile::remove(tmpFileName);
    }
    QApplication::restoreOverrideCursor();
    _modified = false;
    emit modified(false);

    return ok ? 0 : -1;*/
    return 0;
}
