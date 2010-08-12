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
