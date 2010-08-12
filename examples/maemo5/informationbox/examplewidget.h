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

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QMaemo5InformationBox>

class ExampleWidget : public QWidget
{
    Q_OBJECT
public:
    ExampleWidget()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QPushButton *bannerButton = new QPushButton("Show a Banner");
        QPushButton *noteButton = new QPushButton("Show a Note");
        QPushButton *complexNoteButton = new QPushButton("Show a complex Note");

        layout->addWidget(bannerButton);
        layout->addWidget(noteButton);
        layout->addWidget(complexNoteButton);

        connect(bannerButton, SIGNAL(clicked()), SLOT(showBanner()));
        connect(noteButton, SIGNAL(clicked()), SLOT(showNote()));
        connect(complexNoteButton, SIGNAL(clicked()), SLOT(showComplexNote()));
    }

public slots:
    void showBanner()
    {
//! [0]
        QMaemo5InformationBox::information(this, "This is a Maemo5 <b>banner</b>",
                                           QMaemo5InformationBox::DefaultTimeout);
//! [0]
    }

    void showNote()
    {
//! [1]
        QMaemo5InformationBox::information(this, "This is a Maemo5 <b>note</b>",
                                           QMaemo5InformationBox::NoTimeout);
//! [1]
    }

    void showComplexNote()
    {
//! [2]
        QMaemo5InformationBox *box = new QMaemo5InformationBox(this);
        QTextBrowser *textBrowser = new QTextBrowser(box);
        textBrowser->setText("This is a Maemo5 <b>note</b> containing custom widget");
        box->setWidget(textBrowser);
        box->setTimeout(QMaemo5InformationBox::NoTimeout);
        box->exec();
//! [2]
    }
};

