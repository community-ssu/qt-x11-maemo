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

#include <QtGui/QtGui>

#include "common.h"

#include "buttons.h"
#include "itemviews.h"
#include "container.h"
#include "input.h"
#include "dialogs.h"
#include "display.h"
#include "others.h"
#include "maemo5.h"

class WidgetPreviewButton : public QPushButton
{
    Q_OBJECT
public:
    WidgetPreviewButton(const QString &caption, QLayout *layout,
                        CreateWidgetFunc func,
                        QMainWindow *window,
                        const Properties &properties = Properties())
        : f(func), props(properties), w(window)
    {
        setText(caption);
        layout->addWidget(this);

        connect(this, SIGNAL(clicked()), this, SLOT(showPreviewWidget()));
    }

public slots:
    void showPreviewWidget()
    {
        f(w, props);
    }

private:
    CreateWidgetFunc f;
    Properties props;
    QMainWindow *w;
};

class Separator : public QWidget
{
    Q_OBJECT
public:
    Separator(const QString &text, QWidget *parent = 0)
        : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(0);

        QLabel *label = new QLabel(text);
        label->setAlignment(Qt::AlignHCenter);

        QFrame *frame = new QFrame;
        frame->setFrameShape(QFrame::HLine);

        layout->addWidget(label);
        layout->addWidget(frame);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void setPortrait(bool triggered)
    {
        if (!triggered)
            return;
#ifdef Q_WS_MAEMO_5
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#endif
    }

    void setLandscape(bool triggered)
    {
        if (!triggered)
            return;
#ifdef Q_WS_MAEMO_5
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
#endif
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Qt widget gallery");

#ifdef Q_WS_MAEMO_5
    window.setAttribute(Qt::WA_Maemo5StackedWindow);

    QActionGroup *orientation = new QActionGroup(&window);
    orientation->setExclusive(true);
    QAction *landscape = new QAction(QIcon::fromTheme("general_landscape", QIcon(QLatin1String(":/images/landscape"))), "Landscape", orientation);
    landscape->setCheckable(true);
    landscape->setChecked(true);
    QObject::connect(landscape, SIGNAL(triggered(bool)), &window, SLOT(setLandscape(bool)));
    QAction *portrait = new QAction(QIcon::fromTheme("general_portrait", QIcon(QLatin1String(":/images/portrait"))), "Portrait", orientation);
    portrait->setCheckable(true);
    portrait->setChecked(false);
    QObject::connect(portrait, SIGNAL(triggered(bool)), &window, SLOT(setPortrait(bool)));

    window.menuBar()->addActions(orientation->actions());
#endif

    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);

    // ### TODO:
    // QSystemTrayIcon ?
    // QUndoView, QWizard, QWorkSpace
    // QButtonBox?

    // a widget category
    struct PreviewCategory
    {
        const char *text;
        const PreviewWidget *widgets;
    };

    const PreviewCategory categories[] =
    {
        { "Buttons", buttonWidgets },
        { "Item Views", itemViewWidgets },
        { "Containers", containerWidgets },
        { "Input", inputWidgets },
        { "Display", displayWidgets },
        { "Dialogs", dialogs },
        { "Others", otherWidgets },
#ifdef Q_WS_MAEMO_5
        { "Maemo 5", maemo5Widgets },
#endif
        { 0, 0 }
    };

    // first command line argument
    const QString arg = app.arguments().value(1);

    const PreviewCategory *category = categories;
    while (category->text) {
        const PreviewWidget *previewWidget = category->widgets;
        layout->addWidget(new Separator(category->text));
        while (previewWidget->func) {
            WidgetPreviewButton *btn = new WidgetPreviewButton(QString::fromLatin1(previewWidget->text),
                    layout, previewWidget->func, &window,
                    previewWidget->properties ? *previewWidget->properties : Properties());
            if (arg == QLatin1String(previewWidget->text))
                QMetaObject::invokeMethod(btn, "animateClick", Qt::QueuedConnection);
            ++previewWidget;
        }
        ++category;
    }

    QScrollArea *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setWidget(central);
    window.setCentralWidget(area);

    window.show();

    return app.exec();
}

#include "main.moc"

