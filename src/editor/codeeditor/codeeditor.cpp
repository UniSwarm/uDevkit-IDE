/**
 ** This file is part of the uDevkit-IDE project.
 ** Copyright 2017-2020 UniSwarm
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

#include "codeeditor.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSaveFile>
#include <QSplitter>

#include "edbee/edbee.h"
#include "edbee/io/textdocumentserializer.h"
#include "edbee/models/chardocument/chartextdocument.h"
#include "edbee/models/textdocument.h"
#include "edbee/models/textdocumentscopes.h"
#include "edbee/models/texteditorconfig.h"
#include "edbee/models/textgrammar.h"
#include "edbee/models/textrange.h"
#include "edbee/models/textsearcher.h"
#include "edbee/models/textundostack.h"
#include "edbee/texteditorcontroller.h"
#include "edbee/texteditorwidget.h"
#include "edbee/util/lineending.h"
#include "edbee/views/components/texteditorcomponent.h"
#include "edbee/views/texteditorscrollarea.h"
#include "edbee/views/textselection.h"
#include "edbee/views/texttheme.h"

#include "settings/settingsmanager.h"

bool CodeEditor::initialized = false;

CodeEditor::CodeEditor(Project *project, QWidget *parent)
    : Editor(project, parent)
{
    if (!initialized)
    {
        // get the edbee instance
        edbee::Edbee *tm = edbee::Edbee::instance();

        // configure your paths
        if (QFile::exists(QApplication::applicationDirPath() + "/../contrib/edbee-data/keymaps/"))
        {
            tm->setKeyMapPath(QApplication::applicationDirPath() + "/../contrib/edbee-data/keymaps/");
        }
        else
        {
            tm->setKeyMapPath(QApplication::applicationDirPath() + "/../data/keymaps/");
        }

        if (QFile::exists(QApplication::applicationDirPath() + "/../contrib/edbee-data/syntaxfiles/"))
        {
            tm->setGrammarPath(QApplication::applicationDirPath() + "/../contrib/edbee-data/syntaxfiles/");
        }
        else
        {
            tm->setGrammarPath(QApplication::applicationDirPath() + "/../data/syntaxfiles/");
        }

        tm->setThemePath(QApplication::applicationDirPath() + "/../data/themes/");

        // initialize the library
        tm->init();
        tm->autoShutDownOnAppExit();

        initialized = true;
    }

    _settingsClass = rtset()->registerClass("codeEditor");
    connect(_settingsClass, &SettingsClass::classModified, this, &CodeEditor::updateSettings);

    _editorWidget = new edbee::TextEditorWidget();
    _editorWidget->config()->setThemeName("udk-ide");
    //_editorWidget->config()->setShowWhitespaceMode(edbee::TextEditorConfig::ShowWhitespaces);
    _editorWidget->textDocument()->setLineEnding(edbee::LineEnding::unixType());

    _codeEditorScrollBar = new CodeEditorScrollBar();
    _codeEditorScrollBar->setTextRender(_editorWidget->textRenderer());
    _editorWidget->setVerticalScrollBar(_codeEditorScrollBar);
    _editorWidget->textMarginComponent()->init();
    _editorWidget->textScrollArea()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(_editorWidget->textDocument(), &edbee::TextDocument::persistedChanged, this, &CodeEditor::modificationAppend);
    connect(_editorWidget->controller(), &edbee::TextEditorController::updateStatusTextSignal, this, &CodeEditor::updatePos);

    QAction *action;
    action = new QAction(QString("help"), this);
    action->setShortcut(QKeySequence("F1"));
    addAction(action);
    connect(action, &QAction::triggered, this, &CodeEditor::help);

    _codeEditorMarginDelegate = new CodeEditorMarginDelegate();
    _editorWidget->textMarginComponent()->giveDelegate(_codeEditorMarginDelegate);

    updateSettings();
}

bool CodeEditor::isModified() const
{
    return !_editorWidget->textDocument()->isPersisted();
}

void CodeEditor::gotoLine(int x, int y)
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    if (y == -1)
    {
        y = 0;
    }
    controller->moveCaretTo(x - 1, y - 1, false);
    controller->scrollCaretVisible();
}

void CodeEditor::setText(const QString &text)
{
    _editorWidget->textDocument()->setText(text);
}

void CodeEditor::setGrammar(const QString &grammarName)
{
    edbee::TextGrammarManager *grammarManager = edbee::Edbee::instance()->grammarManager();
    edbee::TextGrammar *grammar = grammarManager->detectGrammarWithFilename("source." + grammarName.toLower());
    if (grammar)
    {
        _editorWidget->textDocument()->setLanguageGrammar(grammar);
    }
}

void CodeEditor::setSettingsClass(SettingsClass *settingsClass)
{
    QFont font = _editorWidget->config()->font();
    font.setFamily(settingsClass->setting("fontFamily", "monospace").toString());
    font.setStyleHint(QFont::Monospace);
    font.setPixelSize(settingsClass->setting("fontSize", 10).toInt());
    _editorWidget->config()->setFont(font);
}

int CodeEditor::openFileData(const QString &filePath)
{
    QFile file(filePath);
    QFileInfo info(file);
    if (!info.isReadable() || !info.isFile())
    {
        return -1;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return -1;
    }

    disconnect(_editorWidget->textDocument(), &edbee::TextDocument::textAboutToBeChanged, this, &CodeEditor::insertedText);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (_editorWidget->textDocument()->length() > 1)
    {
        edbee::CharTextDocument document;
        edbee::TextDocumentSerializer serializer(&document);
        if (!serializer.loadWithoutOpening(&file))
        {
            QApplication::restoreOverrideCursor();
            return -1;
        }

        _editorWidget->controller()->replace(0, _editorWidget->textDocument()->length(), document.text(), 0);
    }
    else
    {
        edbee::TextDocumentSerializer serializer(_editorWidget->textDocument());
        if (!serializer.loadWithoutOpening(&file))
        {
            QApplication::restoreOverrideCursor();
            return -1;
        }

        edbee::TextGrammarManager *grammarManager = edbee::Edbee::instance()->grammarManager();
        edbee::TextGrammar *grammar = grammarManager->detectGrammarWithFilename(filePath);
        _editorWidget->textDocument()->setLanguageGrammar(grammar);
    }
    QApplication::restoreOverrideCursor();

    _editorWidget->textDocument()->setPersisted(true);

    setFilePath(filePath);
    _project->versionControl()->requestFileModifications(_filePath);
    connect(_project->versionControl(), &AbstractVersionControl::fileModificationsAvailable, this, &CodeEditor::updateModifications);
    connect(_editorWidget->textDocument(), &edbee::TextDocument::textAboutToBeChanged, this, &CodeEditor::insertedText);
    emit modified(false);

    // TODO replace this section with .editorconfig
    if (info.suffix() == "mk" || info.fileName() == "Makefile")
    {
        _editorWidget->config()->setUseTabChar(true);
    }
    else
    {
        _editorWidget->config()->setUseTabChar(false);
    }

    return 0;
}

int CodeEditor::saveFileData(const QString &filePath)
{
    QString path = filePath.isEmpty() ? _filePath : filePath;
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        return -1;
    }

    edbee::TextDocumentSerializer serializer(_editorWidget->textDocument());
    if (!serializer.saveWithoutOpening(&file))
    {
        return -1;
    }

    if (!file.commit())
    {
        return -1;
    }

    _editorWidget->textDocument()->setPersisted(true);
    emit modified(false);
    _project->versionControl()->requestFileModifications(_filePath);

    return 0;
}

void CodeEditor::updateSettings()
{
    setSettingsClass(_settingsClass);
}

void CodeEditor::modificationAppend()
{
    emit modified(isModified());
}

void CodeEditor::help()
{
    if (_editorWidget->textSelection()->rangeCount() > 0)
    {
        edbee::TextSelection *wordUnderCursor = new edbee::TextSelection(*_editorWidget->textSelection());
        wordUnderCursor->expandToWords(_editorWidget->textDocument()->config()->whitespaces(), _editorWidget->textDocument()->config()->charGroups());

        QString word = _editorWidget->textDocument()->textPart(wordUnderCursor->range(0).anchor(), wordUnderCursor->range(0).length());
        qDebug() << wordUnderCursor->rangesAsString() << word;
        delete wordUnderCursor;

        emit helpRequest(word);
    }
}

void CodeEditor::updatePos()
{
    QString status;
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextDocument *textDocument = _editorWidget->textDocument();
    edbee::TextRange &range = _editorWidget->textSelection()->range(0);
    int caret = range.caret();
    int line = _editorWidget->textDocument()->lineFromOffset(caret);
    int col = _editorWidget->textDocument()->columnFromOffsetAndLine(caret, line) + 1;

    status.append(QString("l: %1 c: %2 ").arg(line + 1).arg(col));
    if (range.length() > 0)
    {
        status.append(QString("sel: %1 ").arg(range.length()));
        emit copyAvailable(true);
    }
    else
    {
        emit copyAvailable(false);
    }
    emit redoAvailable(textDocument->textUndoStack()->canRedo(controller));
    emit undoAvailable(textDocument->textUndoStack()->canUndo(controller));

    QVector<edbee::TextScope *> scopes = _editorWidget->textDocument()->scopes()->scopesAtOffset(caret);
    for (int i = 0, cnt = scopes.size(); i < cnt; ++i)
    {
        if (i != 0)
        {
            status.append(" > ");
        }
        edbee::TextScope *scope = scopes[i];
        status.append(scope->name());
    }
    emit statusChanged(status);
}

void CodeEditor::updateModifications(const QString &filePath)
{
    if (filePath == _filePath)
    {
        _localFileChange = _project->versionControl()->fileModifications(_filePath);
        notifyModificationChanged();
    }
}

void CodeEditor::notifyModificationChanged()
{
    _codeEditorMarginDelegate->setFileChange(&_localFileChange);
    _editorWidget->textMarginComponent()->update();
    _codeEditorScrollBar->setFileChange(&_localFileChange);
}

void CodeEditor::insertedText(edbee::TextBufferChange change)
{
    int startLine = _editorWidget->textDocument()->lineFromOffset(change.offset());
    int startCol = _editorWidget->textDocument()->columnFromOffsetAndLine(change.offset(), startLine) + 1;
    startLine++;

    /*qDebug()<<"insertedchange"<<change.length()<<change.lineCount()<<change.offset()
            <<change.newTextLength()<<change.newLineCount()
            <<startLine<<startCol;*/

    QString newData = QString(change.newText(), change.newTextLength());

    VersionChange vchange;
    vchange.setLineOld(startLine);
    vchange.setLineNew(startLine);
    for (int i = startLine; i < startLine + change.lineCount() + 1 && i < _editorWidget->textDocument()->lineCount(); i++)
    {
        vchange.addRemovedLine(_editorWidget->textDocument()->lineWithoutNewline(i - 1));
    }

    // added lines
    QStringList linesAdded = newData.split(QRegularExpression("\n|\r\n|\r"));
    int endCol = _editorWidget->textDocument()->columnFromOffsetAndLine(change.offset() + change.length(), startLine - 1 + change.lineCount());
    QString newLine;
    if (!vchange.removedLines().isEmpty())
    {
        newLine = vchange.removedLines().first().mid(0, startCol - 1);
    }
    newLine.append(linesAdded.first());
    if (change.newLineCount() > 0)
    {
        vchange.addAddedLine(newLine);
        for (int i = 1; i < linesAdded.count() - 1; i++)
        {
            vchange.addAddedLine(linesAdded.at(i));
        }
        newLine = linesAdded.last();
    }
    if (!vchange.removedLines().isEmpty())
    {
        newLine.append(vchange.removedLines().last().midRef(endCol));
    }
    vchange.addAddedLine(newLine);

    // qDebug()<<vchange.removedLines().count()<<vchange.addedLines().count();
    // qDebug()<<vchange.removedLines()<<vchange.addedLines();

    _localFileChange.insertChange(vchange);

    notifyModificationChanged();
}

void CodeEditor::initialiseWidget()
{
    if (!layout())
    {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setMargin(0);
        if (hasPreview())
        {
            QSplitter *splitter = new QSplitter();
            splitter->addWidget(_editorWidget);
            splitter->addWidget(previewWidget());
            splitter->setSizes({100, 100});
            layout->addWidget(splitter);
        }
        else
        {
            layout->addWidget(_editorWidget);
        }
        setLayout(layout);
        repaint();
    }
    updatePos();
}

void CodeEditor::giveFocus()
{
    _editorWidget->textEditorComponent()->setFocus();
}

void CodeEditor::undoCommand()
{
    _editorWidget->controller()->executeCommand("undo");
}

void CodeEditor::redoCommand()
{
    _editorWidget->controller()->executeCommand("redo");
}

void CodeEditor::cutCommand()
{
    _editorWidget->controller()->executeCommand("cut");
}

void CodeEditor::copyCommand()
{
    _editorWidget->controller()->executeCommand("copy");
}

void CodeEditor::pasteCommand()
{
    _editorWidget->controller()->executeCommand("paste");
}

void CodeEditor::formatCommand()
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    _editorWidget->textDocument()->beginChanges(controller);
    edbee::TextRangeSet range(_editorWidget->textSelection());
    QString replaceString;

    range.clear();
    searcher->setSearchTerm("\t");
    replaceString = "    ";
    searcher->markAll(&range);
    _editorWidget->textDocument()->replaceRangeSet(range, replaceString);

    range.clear();
    searcher->setSyntax(edbee::TextSearcher::SyntaxRegExp);
    searcher->setSearchTerm(" +$");
    replaceString = "";
    searcher->markAll(&range);
    _editorWidget->textDocument()->replaceRangeSet(range, replaceString);

    _editorWidget->textDocument()->endChanges(0xFF02);

    controller->borderedTextRanges()->clear();
    controller->borderedTextRanges()->resetAnchors();
    controller->update();
}

Editor::SearchCaps CodeEditor::searchCap() const
{
    return Editor::SearchCaps(Editor::HasSearch | Editor::HasRegexp | Editor::HasReplace);
}

int CodeEditor::search(const QVariant &searchTerm, SearchFlags flags)
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    _searchTerm = searchTerm;

    // regexp mode
    if (flags.testFlag(RegExpMode))
    {
        _searchTerm = searchTerm;
        searcher->setSyntax(edbee::TextSearcher::SyntaxRegExp);
    }
    else
    {
        _searchTerm = QRegularExpression::escape(searchTerm.toString());
        searcher->setSyntax(edbee::TextSearcher::SyntaxPlainString);
    }
    searcher->setSearchTerm(searchTerm.toString());

    // case sensitivity
    searcher->setCaseSensitive(flags.testFlag(CaseSensitive));

    searcher->markAll(controller->borderedTextRanges());

    controller->update();

    return controller->borderedTextRanges()->rangeCount(); // return the number of occurences found
}

void CodeEditor::searchNext()
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    searcher->findNext(_editorWidget);

    controller->update();
}

void CodeEditor::searchPrev()
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    searcher->findPrev(_editorWidget);

    controller->update();
}

void CodeEditor::searchSelectAll()
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    searcher->selectAll(_editorWidget);

    controller->update();
}

void CodeEditor::replace(const QVariant &replacePattern, Editor::SearchFlags flags, bool next)
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();
    edbee::TextSelection *textSelection = _editorWidget->textSelection();

    QString replaceString = replacePattern.toString();
    replaceString.replace("\\r", "\r");
    replaceString.replace("\\n", "\n");
    replaceString.replace("\\t", "\t");
    replaceString.replace("\\\\", "\\");

    bool find = false;
    bool match = false;

    if (textSelection->rangeCount() == 1)
    {
        if (textSelection->range(0).length() > 0)
        {
            QString selection = _editorWidget->textDocument()->textPart(textSelection->range(0).anchor(), textSelection->range(0).length());
            QRegularExpression regExp(_searchTerm.toString(), flags.testFlag(CaseSensitive) ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            if (regExp.match(selection).hasMatch())
            {
                match = true;
                find = true;
            }
        }
    }

    if (!match)
    {
        if (next)
        {
            find = searcher->findNext(_editorWidget);
        }
        else
        {
            find = searcher->findPrev(_editorWidget);
        }
    }

    if (find)
    {
        QString replaceText;
        edbee::TextRangeSet range(_editorWidget->textSelection());
        if (!flags.testFlag(RegExpMode))
        {
            replaceText = replaceString;
            _editorWidget->textDocument()->replaceRangeSet(range, replaceText);
        }
        else
        {
            QRegularExpression regExp(_searchTerm.toString(), flags.testFlag(CaseSensitive) ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            replaceText = range.getSelectedText();
            replaceText.replace(regExp, replaceString);
            _editorWidget->textDocument()->replaceRangeSet(range, replaceText);
        }
        textSelection->range(0).setLength(0);
        textSelection->range(0).moveCaret(_editorWidget->textDocument(), replaceText.length());

        int index = controller->borderedTextRanges()->rangeIndexAtOffset(textSelection->range(0).anchor() + replaceText.length());
        if (index >= 0)
        {
            controller->borderedTextRanges()->removeRange(index);
        }
    }
    controller->update();
}

void CodeEditor::replaceAll(const QVariant &replacePattern, SearchFlags flags)
{
    edbee::TextEditorController *controller = _editorWidget->controller();
    edbee::TextSearcher *searcher = controller->textSearcher();

    QString replaceString = replacePattern.toString();
    replaceString.replace("\\r", "\r");
    replaceString.replace("\\n", "\n");
    replaceString.replace("\\t", "\t");
    replaceString.replace("\\\\", "\\");

    edbee::TextRangeSet *rangeSet = controller->borderedTextRanges();
    searcher->setCaseSensitive(flags.testFlag(CaseSensitive));
    searcher->markAll(rangeSet);

    _editorWidget->textDocument()->beginChanges(controller);
    edbee::TextRangeSet range(_editorWidget->textDocument());

    for (int i = 0; i < rangeSet->rangeCount(); i++)
    {
        range.clear();
        range.addRange(rangeSet->range(i));

        if (!flags.testFlag(RegExpMode))
        {
            _editorWidget->textDocument()->replaceRangeSet(range, replaceString);
        }
        else
        {
            QRegularExpression regExp(_searchTerm.toString(), flags.testFlag(CaseSensitive) ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            QString replaceText = range.getSelectedText();
            replaceText.replace(regExp, replaceString);
            _editorWidget->textDocument()->replaceRangeSet(range, replaceText);
        }
    }
    _editorWidget->textDocument()->endChanges(0xFF01);

    controller->borderedTextRanges()->clear();
    controller->borderedTextRanges()->resetAnchors();
    controller->update();
}

QWidget *CodeEditor::editorWidget()
{
    return _editorWidget->textEditorComponent();
}
