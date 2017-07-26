#include "codeeditor.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

#include "edbee/edbee.h"
#include "edbee/io/textdocumentserializer.h"
#include "edbee/models/textdocument.h"
#include "edbee/models/textgrammar.h"
#include "edbee/models/texteditorconfig.h"
#include "edbee/texteditorcontroller.h"

bool CodeEditor::initialized = false;

CodeEditor::CodeEditor(QWidget *parent) : Editor(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    if (!initialized)
    {
        // get the edbee instance
        edbee::Edbee* tm = edbee::Edbee::instance();

        // configure your paths
        tm->setKeyMapPath( QApplication::applicationDirPath()+"/../contrib/edbee-data/keymaps/");
        tm->setGrammarPath(  QApplication::applicationDirPath()+"/../contrib/edbee-data/syntaxfiles/" );
        tm->setThemePath( QApplication::applicationDirPath()+"/../contrib/edbee-data/themes/" );

        // initialize the library
        tm->init();
        tm->autoShutDownOnAppExit();

        initialized = true;
    }

    _editorWidget =  new edbee::TextEditorWidget();
    QFont font = _editorWidget->config()->font();
    font.setFamily("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPixelSize(13);
    _editorWidget->config()->setFont(font);
    _editorWidget->config()->setThemeName("RtIDE");
    //_editorWidget->config()->setThemeName("IDLE");

    layout->addWidget(_editorWidget);
    setLayout(layout);
}

bool CodeEditor::isModified() const
{
    return !_editorWidget->textDocument()->isPersisted();
}

int CodeEditor::openFileData(const QString &filePath)
{
    QFile file (filePath);
    QFileInfo info(file);
    if (!info.isReadable() || !info.isFile())
        return -1;
    if (!file.open(QIODevice::ReadOnly))
        return -1;
    _editorWidget->textDocument()->setText("");

    edbee::TextDocumentSerializer serializer( _editorWidget->textDocument() );
    if( !serializer.loadWithoutOpening( &file ) )
        return -1;

    edbee::TextGrammarManager* grammarManager = edbee::Edbee::instance()->grammarManager();
    edbee::TextGrammar* grammar = grammarManager->detectGrammarWithFilename( filePath );
    _editorWidget->textDocument()->setLanguageGrammar( grammar );

    _editorWidget->textDocument()->setPersisted(true);

    setFilePath(filePath);

    return 0;
}

int CodeEditor::saveFileData(const QString &filePath)
{
    QString path = filePath.isEmpty() ? _filePath : filePath;
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return -1;

    edbee::TextDocumentSerializer serializer( _editorWidget->textDocument() );
    if(!serializer.saveWithoutOpening(&file))
        return -1;

    if (!file.commit())
        return -1;

    _editorWidget->textDocument()->setPersisted(true);

    return 0;
}
