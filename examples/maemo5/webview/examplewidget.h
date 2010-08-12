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

#ifndef EXAMPLEWIDGET_H
#define EXAMPLEWIDGET_H

#include "qwebviewselectionsuppressor.h"

#include <QtGui/qwidget.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qabstractkineticscroller.h>
#include <QtGui/qlayout.h>
#include <QtWebKit/qwebview.h>

#include <QApplication>

class ExampleWidget : public QWidget
{
    Q_OBJECT
public:
    ExampleWidget()
    {
        const char exampleText[] =
            "<html>"
            "<body>"
            "<h1>WebKit Example</h1>"
            "<p>"
            "By default, a mouse move on the screen selects text. However, while "
            "finger scrolling, the view should just scroll, not select text. This example "
            "shows how to achieve the two different modes - either selecting text while "
            "swiping, or scrolling while swiping."
            "</p>"
            "<br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />"
            "Some dummy text."
            "<br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />"
            "End of page.";

        QVBoxLayout *layout = new QVBoxLayout(this);

        view = new QWebView;
//! [0]
        suppressor = new QWebViewSelectionSuppressor(view);
//! [0]
        QUrl url = QUrl::fromUserInput(QApplication::arguments().value(1));
        if (!url.isValid()) {
            view->setHtml(QLatin1String(exampleText));
        } else {
            view->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
            view->load(url);
        }

        layout->addWidget(view);

        scrollButton = new QRadioButton("Scrolling Mode");
        scrollButton->setChecked(true);
        textButton = new QRadioButton("Text Selection Mode");

        QButtonGroup *mode = new QButtonGroup(this);
        mode->addButton(scrollButton);
        mode->addButton(textButton);

        QHBoxLayout *modeLayout = new QHBoxLayout();
        modeLayout->setSpacing(0);
        modeLayout->addWidget(scrollButton);
        modeLayout->addWidget(textButton);
 
        layout->addLayout(modeLayout);

        connect(mode, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    }

private Q_SLOTS:
    void buttonClicked(QAbstractButton *button)
    {
        Q_ASSERT(view->property("kineticScroller").value<QAbstractKineticScroller *>());

        if (button == scrollButton) {
//! [1]
            suppressor->enable();
            view->property("kineticScroller").value<QAbstractKineticScroller *>()->setEnabled(true);
//! [1]
        }

        if (button == textButton) {
//! [2]
            suppressor->disable();
            view->property("kineticScroller").value<QAbstractKineticScroller *>()->setEnabled(false);
//! [2]
        }
    }

private:
    QWebView *view;
    QWebViewSelectionSuppressor *suppressor;
    QRadioButton *scrollButton;
    QRadioButton *textButton;
};

#endif

