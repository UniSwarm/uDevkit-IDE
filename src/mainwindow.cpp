#include "mainwindow.h"

#include <QBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

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
    tm->setKeyMapPath( QApplication::applicationDirPath()+"/../edbee-data/keymaps/");
    tm->setGrammarPath(  QApplication::applicationDirPath()+"/../edbee-data/syntaxfiles/" );
    tm->setThemePath( QApplication::applicationDirPath()+"/../edbee-data/themes/" );

    // initialize the library
    tm->init();
    tm->autoShutDownOnAppExit();

    _fileItemModel = new QFileSystemModel();
    _fileItemModel->setRootPath(QApplication::applicationDirPath()+"/../../rtprog/");

    QSplitter *spitter = new QSplitter(this);
    _fileView = new QTreeView(this);
    _fileView->setModel(_fileItemModel);
    _fileView->setRootIndex(_fileItemModel->index(QApplication::applicationDirPath()+"/../../rtprog/"));
    for (int i = 1; i < _fileItemModel->columnCount(); ++i)
        _fileView->hideColumn(i);
    _fileView->header()->close();
    _editorWidget =  new edbee::TextEditorWidget();
    spitter->addWidget(_fileView);
    spitter->addWidget(_editorWidget);
    spitter->setSizes(QList<int>()<<100<<500);
    connect(_fileView, &QTreeView::doubleClicked, this, &MainWindow::openIndex);

    setCentralWidget(spitter);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

MainWindow::~MainWindow()
{
}

int MainWindow::openFile(const QString &fileName)
{
    QFile file (fileName);
    QFileInfo info(file);
    if (!info.isReadable() || !info.isFile())
        return -1;
    _editorWidget->textDocument()->setText("");

    edbee::TextDocumentSerializer serializer( _editorWidget->textDocument() );
    if( !serializer.load( &file ) )
        qDebug() << QString("Error opening file!\n%1").arg(serializer.errorString());

    edbee::TextGrammarManager* grammarManager = edbee::Edbee::instance()->grammarManager();
    edbee::TextGrammar* grammar = grammarManager->detectGrammarWithFilename( fileName );
    _editorWidget->textDocument()->setLanguageGrammar( grammar );

}

void MainWindow::openIndex(const QModelIndex &index)
{
    openFile(_fileItemModel->filePath(index));
}
