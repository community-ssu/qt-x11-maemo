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

#include <QtGui>

// ![0]
#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
// ![0]

class ZoomWidget : public QWidget
{
    Q_OBJECT

public:
    ZoomWidget()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        progressBar = new QProgressBar;
        progressBar->setValue(50);

        layout->addWidget(new QLabel("<p align=\"center\">Please press the zoom keys</p>"));
        layout->addWidget(progressBar);

        grabZoomKeys(true);
    }

// ![1]
    void grabZoomKeys(bool grab)
    {
        if (!winId()) {
            qWarning("Can't grab keys unless we have a window id");
            return;
        }

        unsigned long val = (grab) ? 1 : 0;
        Atom atom = XInternAtom(QX11Info::display(), "_HILDON_ZOOM_KEY_ATOM", False);
        if (!atom) {
            qWarning("Unable to obtain _HILDON_ZOOM_KEY_ATOM. This example will only work "
                     "on a Maemo 5 device!");
            return;
        }

        XChangeProperty (QX11Info::display(),
                winId(),
                atom,
                XA_INTEGER,
                32,
                PropModeReplace,
                reinterpret_cast<unsigned char *>(&val),
                1);
    }
// ![1]

protected:
// ![2]
    void keyPressEvent(QKeyEvent* event)
    {
        switch (event->key()) {
        case Qt::Key_F7:
            progressBar->setValue(progressBar->value() + 1);
            event->accept();
            break;

        case Qt::Key_F8:
            progressBar->setValue(progressBar->value() - 1);
            event->accept();
            break;
        }
        QWidget::keyPressEvent(event);
    }
// ![2]

private:
    QProgressBar *progressBar;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ZoomWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
