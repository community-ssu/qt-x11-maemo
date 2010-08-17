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
