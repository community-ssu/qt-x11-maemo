/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
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

