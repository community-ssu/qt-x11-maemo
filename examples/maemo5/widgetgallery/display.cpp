/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "display.h"

#include <QtGui>

#ifndef QT_NO_WEBKIT
#  include <QtWebKit>
#endif

extern const char lotsOfText[];

static const Properties textProperties = Properties()
            << Property("text", "Example Text");

static const Properties urlTextProperties = Properties()
            << Property("text", "Go to <a href=\"http://qt.nokia.com\">qt.nokia.com</a>")
            << Property("openExternalLinks", true);

static const Properties plainTextProperties = Properties()
            << Property("plainText", QLatin1String(lotsOfText));

static const Properties intValueProperties = Properties()
            << Property("intValue", 42);

static const Properties urlProperties = Properties()
            << Property("url", QUrl("http://www.google.com/"));

static const Properties valueProperties = Properties()
            << Property("value", 42);

struct QGVProxyInitializer
{
    static void init(QGraphicsView *view)
    {
        QGroupBox *groupBox = new QGroupBox("Contact Details");
        QLabel *numberLabel = new QLabel("Telephone number");
        QLineEdit *numberEdit = new QLineEdit;

        QFormLayout *layout = new QFormLayout;
        layout->addRow(numberLabel, numberEdit);
        groupBox->setLayout(layout);

        QGraphicsScene *scene = new QGraphicsScene;
        scene->addWidget(groupBox);

        view->setScene(scene);
    }
};

struct QGVTextItemInitializer
{
    static void init(QGraphicsView *view)
    {
        QGraphicsScene *scene = new QGraphicsScene;

        QGraphicsTextItem *textItem = new QGraphicsTextItem("Edit me");
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

        scene->addItem(textItem);

        view->setScene(scene);
    }
};

#ifndef QT_NO_WEBKIT
struct QWebViewMouseFilterInit : public QObject
{
    static void init(QWebView *view)
    {
        view->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

        // a crude workaround for the lack of WebKit API to disable text selection
        // and image dragging
        new QWebViewMouseFilterInit(view);
    }

    QWebViewMouseFilterInit(QWebView *view)
        : QObject(view)
    {
        view->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *o, QEvent *e)
    {
        return (e->type() == QEvent::MouseMove);
    }
};
#endif

const PreviewWidget displayWidgets[] =
{
    { "QLabel",             WidgetCreator<QLabel>::createWidget, &textProperties },
    { "QLabel (url)",       WidgetCreator<QLabel>::createWidget, &urlTextProperties },
    { "QTextBrowser",       WidgetCreator<QTextBrowser>::createWidget, &plainTextProperties },
    { "QGraphicsView (Proxy Widgets)", WidgetCreator<QGraphicsView, QGVProxyInitializer>::createWidget, 0 },
    { "QGraphicsView (Text Item)", WidgetCreator<QGraphicsView, QGVTextItemInitializer>::createWidget, 0 },
    // ### more QGraphicsView
    { "QCalendarWidget",    WidgetCreator<QCalendarWidget>::createWidget, 0 },
    { "QLCDNumber",         WidgetCreator<QLCDNumber>::createWidget, &intValueProperties },
    { "QProgressBar",       WidgetCreator<QProgressBar>::createWidget, &valueProperties },
#ifndef QT_NO_WEBKIT
    { "QWebView", WidgetCreator<QWebView, QWebViewMouseFilterInit>::createWidget, &urlProperties },
#endif
    { 0, 0, 0 }
};

