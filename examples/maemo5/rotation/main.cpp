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
