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
#include "edbee/util/lineending.h"
#include "edbee/models/textsearcher.h"

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
    _editorWidget->config()->setUseTabChar(false);
    _editorWidget->textDocument()->setLineEnding(edbee::LineEnding::unixType());

    connect(_editorWidget->textDocument(), &edbee::TextDocument::persistedChanged, this, &CodeEditor::modificationAppend);

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
    emit modified(false);

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
    emit modified(false);

    return 0;
}

void CodeEditor::modificationAppend()
{
    emit modified(isModified());
}

Editor::SearchCaps CodeEditor::searchCap() const
{
    return Editor::SearchCaps(Editor::HasSearch | Editor::HasRegexp | Editor::HasReplace);
}

int CodeEditor::search(const QVariant &searchTerm, SearchFlags flags)
{
    edbee::TextEditorController* controller = _editorWidget->controller();
    edbee::TextSearcher* searcher = controller->textSearcher();

    if (flags.testFlag(RegExpMode))
        searcher->setSyntax(edbee::TextSearcher::SyntaxRegExp);
    else
        searcher->setSyntax(edbee::TextSearcher::SyntaxPlainString);

    searcher->setSearchTerm(searchTerm.toString());
    searcher->markAll(controller->borderedTextRanges());

    controller->update();

    return controller->borderedTextRanges()->rangeCount(); // return the number of occurences found
}
