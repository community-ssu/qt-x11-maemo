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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//! [0]
    QPushButton button1("Click here to open a new stacked window");
    button1.setAttribute(Qt::WA_Maemo5StackedWindow);
//! [0]

//! [1]
    QPushButton button2("Click here to open another stacked window", &button1);
    button2.setAttribute(Qt::WA_Maemo5StackedWindow);
    button2.setWindowFlags(button2.windowFlags() | Qt::Window);
//! [1]

//! [2]
    QLabel label("<p align=\"center\">This is the last stacked window. "
                 "Press the back arrow on the top-right corner to go back.",
                 &button2, Qt::Window);
    label.setAttribute(Qt::WA_Maemo5StackedWindow);
//! [2]
    label.setWordWrap(true);

//! [3]
    QObject::connect(&button1, SIGNAL(clicked()), &button2, SLOT(show()));
    QObject::connect(&button2, SIGNAL(clicked()), &label, SLOT(show()));
//! [3]

    button1.show();

    return app.exec();
}

