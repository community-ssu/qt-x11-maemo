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

