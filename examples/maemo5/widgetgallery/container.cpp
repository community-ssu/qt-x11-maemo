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

