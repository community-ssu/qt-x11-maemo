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

#include <QtCore>
#include <QtGui>
#include "fingerswipegesture.h"

/*!
    \class RectObject
    A QGraphicsObject that grabs and reacts on the swipe gesture.
    Note that it needs to be a QGraphicsObject or else the gestures will not work correctly.
*/
class RectObject :  public QGraphicsObject
{
    Q_OBJECT

public:

    RectObject( qreal x, qreal y, qreal width, qreal height, QBrush brush)
        : m_rect(x, y, width, height)
        , m_pen(brush.color().lighter(), 3.0)
        , m_brush(brush)
        , m_striken(false)
    {
// ![0]
        if (!fingerSwipeGestureType)
            fingerSwipeGestureType = QGestureRecognizer::registerRecognizer( new FingerSwipeGestureRecognizer() );

        grabGesture(fingerSwipeGestureType);
// ![0]
    }

    /*!
      \reimp
      */
    QRectF boundingRect() const
    {
        qreal halfpw = m_pen.widthF() / 2;
        QRectF rect = m_rect;
        if (halfpw > 0.0)
            rect.adjust(-halfpw, -halfpw, halfpw, halfpw);

        return rect;
    }

    bool sceneEvent(QEvent *e)
    {
// ![1]
        if (e->type() == QEvent::TouchBegin) {
            e->accept();
            return true;
        }
// ![1]
// ![2]
        if (e->type() == QEvent::Gesture) {
             QGestureEvent *ge = static_cast<QGestureEvent*>(e);

             if (QGesture *gesture = ge->gesture(fingerSwipeGestureType)) {
                 FingerSwipeGesture *swipe = static_cast<FingerSwipeGesture*>(gesture);
                 if (swipe->state() == Qt::GestureFinished) {
                     m_striken = swipe->isLeftToRight();
                     update();
                 }
                 ge->setAccepted(gesture, true);
                 return true;
             }
        }
// ![2]
        return QGraphicsObject::sceneEvent(e);
    }

    /*! \reimp
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setPen(m_pen);
        painter->setBrush(m_brush);
        painter->drawRect(m_rect);

        if (m_striken) {
            QPen strikePen(Qt::red, 10.0);
            painter->setPen(strikePen);
            painter->drawLine(m_rect.left() + m_rect.width() / 10,
                              m_rect.center().y(),
                              m_rect.right() - m_rect.width() / 10,
                              m_rect.center().y());
        }
    }

    static Qt::GestureType fingerSwipeGestureType;

    QRectF m_rect;

    QPen m_pen;
    QBrush m_brush;
    bool m_striken;
};

Qt::GestureType RectObject::fingerSwipeGestureType = (Qt::GestureType)0;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow()
    {
        QGraphicsScene *scene = new QGraphicsScene();

// ![3]
        RectObject *rect1 = new RectObject( 10, 10, 200, 100, QBrush( Qt::gray ) );
        RectObject *rect2 = new RectObject( 100, 200, 200, 200, QBrush( Qt::green ) );
        RectObject *rect3 = new RectObject( 400, 300, 300, 80, QBrush( Qt::yellow ) );

        scene->addItem(rect1);
        scene->addItem(rect2);
        scene->addItem(rect3);
// ![3]

        QGraphicsView *view = new QGraphicsView();
        view->setScene(scene);

        setCentralWidget(view);

        setWindowTitle(tr("Gesture example"));
  }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}

#include "main.moc"
