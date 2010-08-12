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

#include "container.h"

#include <QtGui>

static const Properties titleProperties = Properties()
    << Property("title", "Example Title");

struct ToolBoxInitializer
{
    static void init(QToolBox *box)
    {
        box->addItem(new QLabel("Some Text 1"), "First");
        box->addItem(new QLabel("Some Text 2"), "Second");
        box->addItem(new QLabel("Some Text 3"), "Third");
        box->addItem(new QLabel("Some Text 4"), "Fourth");
    }
};

struct TabBarInit
{
    static void init(QTabBar *bar)
    {
        bar->addTab("Example Tab 1");
        bar->addTab("Example Tab 2");
        bar->addTab("Example Tab 3");
        bar->addTab("Example Tab 4");
    }
};

struct TabWidgetInit
{
    static void init(QTabWidget *widget)
    {
        widget->addTab(new QTextEdit, "Example Tab 1");
        widget->addTab(new QTextEdit, "Example Tab 2");
        widget->addTab(new QTextEdit, "Example Tab 3");
    }
};

const PreviewWidget containerWidgets[] =
{
    { "QGroupBox",          WidgetCreator<QGroupBox>::createWidget, &titleProperties },
    { "QToolBox",           WidgetCreator<QToolBox, ToolBoxInitializer>::createWidget, 0 },
    { "QTabBar",            WidgetCreator<QTabBar, TabBarInit>::createWidget, 0 },
    { "QTabWidget",         WidgetCreator<QTabWidget, TabWidgetInit>::createWidget, 0 },
    { "QFrame",             WidgetCreator<QFrame>::createWidget, 0 },
    // QMdiArea?
    // QDockWidget?
    { 0, 0, 0 }
};

