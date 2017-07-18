#include "mainwindow.h"

#include <QBoxLayout>
#include <QFile>
#include <QDebug>

#include "edbee/edbee.h"
#include "edbee/io/textdocumentserializer.h"
#include "edbee/models/textdocument.h"
#include "edbee/models/textgrammar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // get the edbee instance
    edbee::Edbee* tm = edbee::Edbee::instance();

    // configure your paths
    tm->setKeyMapPath( "../edbee-data/keymaps/");
    tm->setGrammarPath(  "../edbee-data/syntaxfiles/" );
    tm->setThemePath( "../edbee-data/themes/" );

    // initialize the library
    tm->init();
    tm->autoShutDownOnAppExit();

    _editorWidget =  new edbee::TextEditorWidget();

    setCentralWidget(_editorWidget);
    setFixedSize(QSize(800,600));
}

MainWindow::~MainWindow()
{
}

void MainWindow::openFile(const QString &fileName)
{
    QFile file (fileName);

    edbee::TextDocumentSerializer serializer( _editorWidget->textDocument() );
    if( !serializer.load( &file ) )
        qDebug() << QString("Error opening file!\n%1").arg(serializer.errorString());

    edbee::TextGrammarManager* grammarManager = edbee::Edbee::instance()->grammarManager();
    edbee::TextGrammar* grammar = grammarManager->detectGrammarWithFilename( fileName );
    _editorWidget->textDocument()->setLanguageGrammar( grammar );

}
