/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

