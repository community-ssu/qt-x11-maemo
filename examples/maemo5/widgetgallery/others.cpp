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

#include "others.h"

#include <QtGui>

struct StatusBarInitializer
{
    static void init(QMainWindow *window)
    {
        window->statusBar()->addWidget(new QLabel("Some Text"));
        window->statusBar()->addWidget(new QProgressBar);
        window->statusBar()->addWidget(new QLabel("Some Text"));
    }
};

struct ToolBarInitializer
{
    static void init(QMainWindow *window)
    {
        QToolBar *bar = window->addToolBar("ToolBar");
        bar->addAction("Action 1");
        bar->addAction("Action 2");
        bar->addSeparator();
        bar->addAction("Action 3");
    }
};

static void splitterCreator(QMainWindow *parent, const Properties &)
{
    QWidget *topLevel = newToplevel(parent);

    QVBoxLayout *layout = new QVBoxLayout(topLevel);

    QSplitter *hsplitter = new QSplitter(Qt::Horizontal);
    hsplitter->addWidget(new QTextEdit("Widget 1"));
    hsplitter->addWidget(new QTextEdit("Widget 2"));

    QSplitter *vsplitter = new QSplitter(Qt::Vertical);
    vsplitter->addWidget(hsplitter);
    vsplitter->addWidget(new QTextEdit("Widget 3"));

    layout->addWidget(vsplitter);

    topLevel->setWindowTitle("QSplitter");
    topLevel->show();
}

const PreviewWidget otherWidgets[] =
{
    { "QSplitter",          splitterCreator, 0 },
    { "QStatusBar",         WidgetCreator<QMainWindow, StatusBarInitializer>::createWidget, 0 },
    { "QToolBar",           WidgetCreator<QMainWindow, ToolBarInitializer>::createWidget, 0 },
    { 0, 0, 0 }
};

