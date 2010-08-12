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

