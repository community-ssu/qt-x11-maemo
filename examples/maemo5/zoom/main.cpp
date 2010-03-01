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
