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

#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include "codeeditor.h"

#ifndef NOWEBKIT
#if !defined(WEBENGINE) && !defined(WEBKIT)
#define NOWEBKIT
#endif
#endif

#ifdef WEBENGINE
class QWebEngineView;
#endif
#ifdef WEBKIT
class QWebView;
#endif
#ifdef NOWEBKIT
class QTextBrowser;
#endif

class HtmlEditor : public CodeEditor
{
    Q_OBJECT
public:
    HtmlEditor(Project *project, QWidget *parent = nullptr);

    // Editor interface
public:
    QWidget *previewWidget() const override;
    bool hasPreview() const override;

protected slots:
    virtual void updatePreview();
    int openFileData(const QString &filePath) override;

protected:
    void createPreview();

#ifdef WEBENGINE
    QWebEngineView *_htmlPreview;
#endif
#ifdef WEBKIT
    QWebView *_htmlPreview;
#endif
#ifdef NOWEBKIT
    QTextBrowser *_htmlPreview;
#endif
};

#endif // HTMLEDITOR_H
