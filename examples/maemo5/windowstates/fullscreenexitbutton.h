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

#ifndef FULLSCREENEXITBUTTON_H
#define FULLSCREENEXITBUTTON_H

#include <QtGui/qtoolbutton.h>
#include <QtGui/qevent.h>

class FullScreenExitButton : public QToolButton
{
    Q_OBJECT
public:
    inline explicit FullScreenExitButton(QWidget *parent);

protected:
    inline bool eventFilter(QObject *obj, QEvent *ev);
};


FullScreenExitButton::FullScreenExitButton(QWidget *parent)
        : QToolButton(parent)
{
    Q_ASSERT(parent);

    // set the fullsize icon from Maemo's theme
    setIcon(QIcon::fromTheme(QLatin1String("general_fullsize")));

    // ensure that our size is fixed to our ideal size
    setFixedSize(sizeHint());

    // set the background to 0.5 alpha
    QPalette pal = palette();
    QColor backgroundColor = pal.color(backgroundRole());
    backgroundColor.setAlpha(128);
    pal.setColor(backgroundRole(), backgroundColor);
    setPalette(pal);

    // ensure that we're painting our background
    setAutoFillBackground(true);

    // when we're clicked, tell the parent to exit fullscreen
    connect(this, SIGNAL(clicked()), parent, SLOT(showNormal()));

    // install an event filter to listen for the parent's events
    parent->installEventFilter(this);
}

bool FullScreenExitButton::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != parent())
        return QToolButton::eventFilter(obj, ev);

    QWidget *parent = parentWidget();
    bool isFullScreen = parent->windowState() & Qt::WindowFullScreen;

    switch (ev->type()) {
    case QEvent::WindowStateChange:
        setVisible(isFullScreen);
        if (isFullScreen)
            raise();
        // fall through
    case QEvent::Resize:
        if (isVisible())
            move(parent->width() - width(),
                 parent->height() - height());
        break;
    default:
        break;
    }

    return QToolButton::eventFilter(obj, ev);
}

#endif
