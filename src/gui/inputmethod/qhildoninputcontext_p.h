/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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
#ifndef QHILDONINPUTCONTEXT_P_H
#define QHILDONINPUTCONTEXT_P_H

#include <QtGui/QInputContext>
#include <QPointer>
#include <QWidget>
#include <private/qhildoninputmethodprotocol_p.h>
#include <private/qevent_p.h>

#ifdef Q_WS_MAEMO_5

QT_BEGIN_HEADER

QT_MODULE(Gui)

class QHIMProxyWidget : public QWidget {
    Q_OBJECT
public:
    QHIMProxyWidget(QWidget *widget);
    virtual ~QHIMProxyWidget();

    QWidget *widget() const;

    static QHIMProxyWidget *proxyFor(QWidget *w);

private Q_SLOTS:
    void widgetWasDestroyed();

private:
    static QMap<QWidget *, QHIMProxyWidget *> proxies;
    QWidget *w;
};

class QHildonInputContext : public QInputContext
{
    Q_OBJECT
public:
    explicit QHildonInputContext(QObject* parent = 0);
    ~QHildonInputContext();

    QString identifierName();
    QString language();
    void reset();
    bool isComposing() const;
    QWidget *focusWidget() const;
    void setFocusWidget(QWidget *w);
    bool filterEvent(const QEvent *event);
    void update();

protected:
    //Filters
    bool filterKey(QWidget *w, const QKeyEvent *ev, bool isLongPress);
    bool x11FilterEvent(QWidget *keywidget, XEvent *event);

private slots:
    void showSoftKeyboard();
    void longPressDetected();

private:
    void insertUtf8(int flag, const QString& text);
    void clearSelection();
    void cancelPreedit();

    void sendHildonCommand(HildonIMCommand cmd, QWidget *widget = 0);
    void sendX11Event(XEvent *event);

    //Context
    void checkSentenceStart();
    void commitPreeditBuffer();
    void sendSurrounding(bool sendAllContents = false);
    void sendInputMode();
    void setClientCursorLocation(bool offsetIsRelative, int cursorOffset);
    void setCommitMode(HildonIMCommitMode mode, bool clearPreEdit = true);

    void setMaskState(int *mask,
                             HildonIMInternalModifierMask lock_mask,
                             HildonIMInternalModifierMask sticky_mask,
                             bool was_press_and_release);
    void updateInputMethodHints();

    //Vars
    int mask;
    int options;
    HildonIMTrigger triggerMode;
    HildonIMCommitMode commitMode, lastCommitMode;
    int inputMode;
    QString preEditBuffer;
    int textCursorPosOnPress; //position of the cursor in the surrounding text at the last TabletPress event
    bool autoUpper;
    bool lastInternalChange;
    bool spaceAfterCommit;
    QTimer *longPressTimer;
    QScopedPointer<QKeyEventEx> longPressKeyEvent;
    QWidget *lastKeyWidget;
    int lastQtKeyCode;
    QChar combiningChar;      // Unicode representation of the dead key (combining)
    QChar plainCombiningChar; // Unicode representation of the dead key (plain)
    QString lastCommitString;

    QPointer<QWidget> realFocus; // the widget that really has the focus in case of a QGraphicsProxyWidget
    QPointer<QWidget> lastFocus; // the widget that last had the focus (workaround for HIM bug)
};

QT_END_HEADER

#endif // Q_WS_MAEMO_5

#endif //QHILDONINPUTCONTEXT_P_H
