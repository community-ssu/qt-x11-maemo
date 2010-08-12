/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "fullscreenexitbutton.h"

#include <QtGui>
#include <QX11Info>
#include <QtDBus>

// ![4]
#include <X11/Xlib.h>
#include <X11/Xatom.h>
// ![4]

class ExampleWidget : public QWidget
{
    Q_OBJECT

public:
    ExampleWidget()
    {
        QPushButton *fsButton = new QPushButton("Toggle Fullscreen");
        connect(fsButton, SIGNAL(clicked()), SLOT(toggleFullScreen()));

        QCheckBox *busyCheckBox = new QCheckBox("Busy Indicator");
        connect(busyCheckBox, SIGNAL(stateChanged(int)), SLOT(toggleBusy(int)));

        QCheckBox *disturbCheckBox = new QCheckBox("Do Not Disturb");
        connect(disturbCheckBox, SIGNAL(stateChanged(int)), SLOT(toggleDisturb(int)));

        QPushButton *showExpose = new QPushButton("Show Expose");
        connect(showExpose, SIGNAL(clicked()), SLOT(showExpose()));

        QTextBrowser *browser = new QTextBrowser;
        browser->setText("Just some dummy example text.");

// ![3]
        new FullScreenExitButton(this);
// ![3]

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(fsButton);
        layout->addWidget(busyCheckBox);
        layout->addWidget(disturbCheckBox);
        layout->addWidget(showExpose);
        layout->addWidget(browser);

        defaultMargins = layout->contentsMargins();
    }

public Q_SLOTS:
// ![0]
    void toggleFullScreen()
    {
        bool isFullScreen = windowState() & Qt::WindowFullScreen;
        if (isFullScreen)
            showNormal();
        else
            showFullScreen();
    }
// ![0]

// ![1]
    void toggleBusy(int checkState)
    {
        setAttribute(Qt::WA_Maemo5ShowProgressIndicator, checkState == Qt::Checked);
    }
// ![1]

// ![5]
    void toggleDisturb(int checkState)
    {
        Atom atom = XInternAtom(QX11Info::display(), "_HILDON_DO_NOT_DISTURB", False);
        if (!atom) {
            qWarning("Unable to obtain _HILDON_DO_NOT_DISTURB. This example will only work "
                     "on a Maemo 5 device!");
            return;
        }

        if (checkState == Qt::Checked) {
            long state = 1;
            XChangeProperty(
                    QX11Info::display(),
                    winId(),
                    atom,
                    XA_INTEGER,
                    32,
                    PropModeReplace,
                    (unsigned char *) &state,
                    1);
        } else {
            XDeleteProperty(QX11Info::display(), winId(), atom);
        }
    }
// ![5]

// ![2]
    void showExpose()
    {
        QDBusConnection c = QDBusConnection::sessionBus();
        QDBusMessage m = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
        c.send(m);
    }
// ![2]

protected:
    void changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::WindowStateChange) {

            // set the content margins to 0 in full screen mode,
            // so our fullScreenButton will be nicely visible
            const bool isFullScreen = windowState() & Qt::WindowFullScreen;
            if (isFullScreen)
                layout()->setContentsMargins(0, 0, 0, 0);
            else
                layout()->setContentsMargins(defaultMargins);
        }

        QWidget::changeEvent(event);
    }

private:
    QMargins defaultMargins;
};

#endif

