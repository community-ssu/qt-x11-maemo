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

#include "maemo5.h"

#include <QtGui>

#ifdef Q_WS_MAEMO_5

#include <QtMaemo5>

static const Properties textProperties = Properties()
            << Property("text", "Example Text");


template <typename T>
struct PickSelectorSetter
{
    static void init(QMaemo5ValueButton *valueButton)
    {
        valueButton->setPickSelector(new T);
    }
};

static void editBarCreator(QMainWindow *parent, const Properties &)
{
    QWidget *topLevel = newToplevel(parent);

    QVBoxLayout *layout = new QVBoxLayout(topLevel);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);

    QMaemo5EditBar *eb = new QMaemo5EditBar("This is a QMaemo5EditBar...");

    eb->addButton(new QPushButton("Dummy1"));
    eb->addButton(new QPushButton("Dummy2"));

    layout->addWidget(eb);
    layout->addWidget(new QTextEdit("Just a Text Edit"));

    topLevel->setWindowTitle("QMaemo5EditBar");
    topLevel->show();
}

static void noteCreator(QMainWindow *parent, const Properties &)
{
    QMaemo5InformationBox::information(parent, "This is a Maemo5 <b>note</b>",
                                       QMaemo5InformationBox::NoTimeout);
}

static void bannerCreator(QMainWindow *parent, const Properties &)
{
    QMaemo5InformationBox::information(parent, "This is a Maemo5 <b>banner</b>",
                                       QMaemo5InformationBox::DefaultTimeout);
}

// on Maemo 5, horizontal RadioButtons are rendered specially
static void horizontalRadioCreator(QMainWindow *parent, const Properties &properties)
{
    QWidget *topLevel = newToplevel(parent);

    QHBoxLayout *layout = new QHBoxLayout(topLevel);
    layout->setSpacing(0);

    QRadioButton *button1 = new QRadioButton;
    QRadioButton *button2 = new QRadioButton;
    QRadioButton *button3 = new QRadioButton;

    setProperties(button1, properties);
    setProperties(button2, properties);
    setProperties(button3, properties);

    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    topLevel->setWindowTitle("Horizontal Radio Buttons");
    topLevel->show();
}

const PreviewWidget maemo5Widgets[] =
{
    { "QRadioButton (horizontal)", horizontalRadioCreator, &textProperties },
    { "QMaemo5DatePickSelector", WidgetCreator<QMaemo5ValueButton, PickSelectorSetter<QMaemo5DatePickSelector> >::createWidget, 0 },
    { "QMaemo5TimePickSelector", WidgetCreator<QMaemo5ValueButton, PickSelectorSetter<QMaemo5TimePickSelector> >::createWidget, 0 },
    { "QMaemo5EditBar",     editBarCreator, 0 },
    { "QMaemo5InformationBox (banner)", bannerCreator, 0 },
    { "QMaemo5InformationBox (note)", noteCreator, 0 },
    { 0, 0, 0 }
};
#endif

