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

#include "fingerswipegesture.h"
#include <QGesture>

#include <QWidget>
#include <QGraphicsObject>
#include <QEvent>
#include <QTouchEvent>
#include <QtDebug>


/*!
    \class FingerSwipeGestureRecognizer
    The gesture recognizer for the FingerSwipeGesture.
    \see FingerSwipeGesture
*/

FingerSwipeGestureRecognizer::FingerSwipeGestureRecognizer()
{
}

/*! \reimp
 */
QGesture *FingerSwipeGestureRecognizer::create(QObject *target)
{
    if (target && target->isWidgetType()) {
        QWidget *widget = static_cast<QWidget *>(target);
// ![0]
        widget->setAttribute(Qt::WA_AcceptTouchEvents);
// ![0]

    } else if (target) {
        QGraphicsObject *go = qobject_cast<QGraphicsObject*>(target);
        if (go) {
// ![1]
            go->setAcceptTouchEvents(true);
// ![1]
        }
    }
    return new FingerSwipeGesture();
}

/*! \reimp
 */
QGestureRecognizer::Result FingerSwipeGestureRecognizer::recognize(QGesture *state,
                                                            QObject *,
                                                            QEvent *event)
{
    FingerSwipeGesture *q = static_cast<FingerSwipeGesture *>(state);

    const QTouchEvent *ev = static_cast<const QTouchEvent *>(event); // only use ev after checking event->type()

    QGestureRecognizer::Result result = QGestureRecognizer::Ignore;

    switch (event->type()) {
    case QEvent::TouchBegin: {
        result = QGestureRecognizer::MayBeGesture;
        if (ev->touchPoints().size() >= 1) {
            QTouchEvent::TouchPoint p1 = ev->touchPoints().first();
            q->m_startPos = p1.startPos();
        }
        break;
    }
    case QEvent::TouchEnd: {
        if (q->m_cancelled) {
            result = QGestureRecognizer::Ignore;
        } else if (q->m_triggered) {
            result = QGestureRecognizer::FinishGesture;
        } else {
            result = QGestureRecognizer::CancelGesture;
        }
        break;
    }
    case QEvent::TouchUpdate: {
        if (!q->m_cancelled &&
            ev->touchPoints().size() >= 1) {

            QTouchEvent::TouchPoint p1 = ev->touchPoints().first();

            q->m_currentPos = p1.pos();

            // update the hot-spot to be in the middle between start and current point
            q->setHotSpot(QPointF(p1.screenPos().x() - (q->m_currentPos.x() - q->m_startPos.x()) / 2,
                                  p1.screenPos().y() - (q->m_currentPos.y() - q->m_startPos.y()) / 2));

            QPointF delta = p1.pos() - q->m_startPos;

            // -- check that the user has drawn a vertical line
            if (qAbs(delta.x()) > 25 &&
                (qAbs(delta.y()) < qAbs(delta.x()) * 0.2) ) {
                if (!q->m_triggered) {
                    q->m_triggered = true;
                    result = QGestureRecognizer::TriggerGesture;
                }
            } else {
                if (!q->m_triggered) {
                    result = QGestureRecognizer::MayBeGesture;
                } else {
                    q->m_cancelled = true;
                    result = QGestureRecognizer::CancelGesture;
                }
            }
            break;
        }
    }
    default:
        break;
    }
    return result;
}

/*! \reimp
 */
void FingerSwipeGestureRecognizer::reset(QGesture *state)
{
    FingerSwipeGesture *q = static_cast<FingerSwipeGesture *>(state);
    q->m_startPos = QPoint();
    q->m_lastPos = QPoint();
    q->m_currentPos = QPoint();
    q->m_triggered = false;
    q->m_cancelled = false;

    QGestureRecognizer::reset(state);
}

/*!
    \class FingerSwipeGesture
    This gesture detect a left to right or right to left swipe.
*/

/*!
    \internal
*/
FingerSwipeGesture::FingerSwipeGesture(QObject *parent)
    : QGesture(parent)
    , m_triggered(false)
    , m_cancelled(false)
{ }

FingerSwipeGesture::~FingerSwipeGesture()
{ }

bool FingerSwipeGesture::isLeftToRight() const
{
    return m_startPos.x() < m_currentPos.x();
}
