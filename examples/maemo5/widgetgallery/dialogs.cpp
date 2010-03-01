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

#include "dialogs.h"

#include <QtGui>

static const Properties textProperties = Properties()
            << Property("text", "Example Text");

static const Properties valueProperties = Properties()
            << Property("value", 42);

static const Properties inputDialogProperties = Properties()
            << Property("labelText", "Example Label Text")
            << Property("textValue", "Example Text Value");


static void errorMessageCreator(QMainWindow *parent, const Properties &)
{
    QErrorMessage *message = new QErrorMessage(parent);
    message->setAttribute(Qt::WA_DeleteOnClose);

    // use a unique error message every time to ensure that it doesn't
    // get suppressed by the "do not show again" checkbox.
    QString text = QString("Some random error message generated at %1").arg(QDateTime::currentDateTime().toString());

    message->showMessage(text);
}

static void staticFileDialogCreator(QMainWindow *parent, const Properties &)
{
    QFileDialog::getOpenFileName(parent, "Example Caption");
}

static void aboutQtCreator(QMainWindow *parent, const Properties &)
{
    QMessageBox::aboutQt(parent);
}

static void aboutCreator(QMainWindow *parent, const Properties &)
{
    QMessageBox::about(parent, "About this Widget Gallery", "This Applications shows a simple "
            "gallery of Qt widgets.");
}

static void splashScreenCreator(QMainWindow *parent, const Properties &)
{
    QSplashScreen *screen = new QSplashScreen(parent);
    screen->setAttribute(Qt::WA_DeleteOnClose);

    QImage img(600, 300, QImage::Format_ARGB32_Premultiplied);

    {
        QPainter p(&img);
        p.fillRect(img.rect(), QBrush(QLinearGradient(0, 0, 600, 300)));

        p.translate(img.rect().center());
        p.rotate(45);
        p.drawText(img.rect().translated(-img.rect().center()), Qt::AlignCenter, "SPLASH SCREEN :)");
    }

    screen->setPixmap(QPixmap::fromImage(img));
    screen->show();
}

const PreviewWidget dialogs[] =
{
    { "QColorDialog",       DialogCreator<QColorDialog>::createDialog, 0 },
    { "QErrorMessage",      errorMessageCreator, 0 },
    { "QFileDialog",        DialogCreator<QFileDialog>::createDialog, 0 },
    { "QFileDialog (static)", staticFileDialogCreator, 0 },
    { "QFontDialog",        DialogCreator<QFontDialog>::createDialog, 0 },
    { "QInputDialog",       DialogCreator<QInputDialog>::createDialog, &inputDialogProperties },
    { "QMessageBox",        DialogCreator<QMessageBox>::createDialog, &textProperties },
    { "QMessageBox::aboutQt", aboutQtCreator, 0 },
    { "QMessageBox::about", aboutCreator, 0 },
    { "QPageSetupDialog",   DialogCreator<QPageSetupDialog>::createDialog, 0 },
    { "QPrintDialog",       DialogCreator<QPrintDialog>::createDialog, 0 },
    { "QPrintPreviewDialog",DialogCreator<QPrintPreviewDialog>::createDialog, 0 },
    { "QProgressDialog",    DialogCreator<QProgressDialog>::createDialog, &valueProperties },
    { "QSplashScreen",      splashScreenCreator, 0 },
    { 0, 0, 0 }
};


