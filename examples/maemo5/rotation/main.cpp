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

#include <QtGui>

class RotationWidget : public QWidget
{
    Q_OBJECT

public:
    RotationWidget()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        label = new QLabel;

        QHBoxLayout *blayout = new QHBoxLayout;
        blayout->setSpacing(0);

        QRadioButton *button1 = new QRadioButton("Portrait");
        QRadioButton *button2 = new QRadioButton("Auto");
        QRadioButton *button3 = new QRadioButton("Landscape");

        button2->toggle();

        connect(button1, SIGNAL(clicked()), this, SLOT(setPortrait()));
        connect(button2, SIGNAL(clicked()), this, SLOT(setAutoRotation()));
        connect(button3, SIGNAL(clicked()), this, SLOT(setLandscape()));

        blayout->addWidget(button1);
        blayout->addWidget(button2);
        blayout->addWidget(button3);

        layout->addWidget(label);
        layout->addLayout(blayout);

// ![3]
        connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(orientationChanged()));
// ![3]

        setAutoRotation();

        // initialize the label according to the current rotation
        orientationChanged();
    }

public slots:
    void setLandscape()
    {
// ![0]
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
// ![0]
    }

    void setPortrait()
    {
// ![1]
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
// ![1]
    }

    void setAutoRotation()
    {
// ![2]
        setAttribute(Qt::WA_Maemo5AutoOrientation, true);
// ![2]
    }

// ![4]
    void orientationChanged()
    {
        QRect screenGeometry = QApplication::desktop()->screenGeometry();
        if (screenGeometry.width() > screenGeometry.height())
            label->setText("<p align=\"center\">In Landscape Mode</p>");
        else
            label->setText("<p align=\"center\">In Portrait Mode</p>");
    }
// ![4]

private:
    QLabel *label;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RotationWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
