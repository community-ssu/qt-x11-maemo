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

