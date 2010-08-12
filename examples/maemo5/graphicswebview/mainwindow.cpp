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

#include <QtGui>
#include <QtWebKit>
#include "mainwindow.h"

#include <QtDebug>

//![0]
class ViewportItem : public QGraphicsWidget, public QAbstractKineticScroller
{
    Q_OBJECT
public:
    ViewportItem()
        : QGraphicsWidget(), QAbstractKineticScroller(), m_widget(0) 
    {
        setFlag(QGraphicsItem::ItemHasNoContents, true);
        setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
        setFlag(QGraphicsItem::ItemClipsToShape, true);
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        setFiltersChildEvents(true);
    }

//![0]

//![1]
    void setWidget(QGraphicsWidget *widget)
    {
        if (m_widget) {
            m_widget->setParentItem(0);
            delete m_widget;
        }
        m_widget = widget;
        if (m_widget) {
            m_widget->setParentItem(this);
            m_widget->setAttribute(Qt::WA_OpaquePaintEvent, true);

            if (qgraphicsitem_cast<QGraphicsWebView *>(m_widget)) {
                connect(m_widget, SIGNAL(loadFinished(bool)), this, SLOT(resizeWebViewToFrame()));
                resizeWebViewToFrame();
            }
        }
    }
//![1]

protected:
//![4]
    bool sceneEventFilter(QGraphicsItem *i, QEvent *e)
    {
        bool res = false;
        if (i && (i == m_widget) && m_widget->isEnabled()) {
            switch (e->type()) {
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseMove:
            case QEvent::GraphicsSceneMouseRelease:
            case QEvent::GraphicsSceneMouseDoubleClick: {
                res = handleMouseEvent(static_cast<QGraphicsSceneMouseEvent *>(e));
                break;
            }
            default:
                break;
            }
        }
//![6]
        // prevent text selection and image dragging
        if (e->type() == QEvent::GraphicsSceneMouseMove)
            return true;
        return res ? true : QGraphicsWidget::sceneEventFilter(i, e);
//![6]
    }
//![4]

//![3]
    QSize viewportSize() const
    {
        return size().toSize();
    }

    QPoint maximumScrollPosition() const
    {
        QSizeF s = m_widget ? m_widget->size() - size() : QSize(0, 0);
        return QPoint(qMax(0, int(s.width())), qMax(0, int(s.height())));
    }

    QPoint scrollPosition() const
    {
        return m_widget ? -m_widget->pos().toPoint() + m_overShoot : QPoint();
    }

    void setScrollPosition(const QPoint &p, const QPoint &overShoot)
    {
        m_overShoot = overShoot;
        if (m_widget)
            m_widget->setPos(-p + m_overShoot);
    }
//![3]

    void cancelLeftMouseButtonPress(const QPoint & /*globalPressPos*/)
    {
        //TODO: although my test have shown, that this seems like it isn't necessary
    }

private slots:
//![2]
    void resizeWebViewToFrame()
    {
        if (QGraphicsWebView *view = qgraphicsitem_cast<QGraphicsWebView *>(m_widget)) {
            if (view->page() && view->page()->mainFrame()) {
                QSizeF s = view->page()->mainFrame()->contentsSize();
                s = s.expandedTo(size());
                view->setGeometry(QRectF(view->geometry().topLeft(), s));
            }
        }
    }
//![2]

private:
    QGraphicsWidget *m_widget;
    QPoint m_overShoot;
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
//![7]
    GraphicsView()
        : QGraphicsView(new QGraphicsScene()), viewport(0)
    {
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setOptimizationFlags(QGraphicsView::DontSavePainterState);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setFrameShape(QFrame::NoFrame);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        viewport = new ViewportItem();
        scene()->addItem(viewport);
    }
//![7]

    ViewportItem *viewportItem() const
    {
        return viewport;
    }

protected:
//![8]
    void resizeEvent(QResizeEvent *e)
    {
        QGraphicsView::resizeEvent(e);
        setUpdatesEnabled(false);

        if (!viewport)
            return;

        QRectF rect(QPointF(0, 0), size());
        scene()->setSceneRect(rect);

        viewport->setGeometry(rect);
        setUpdatesEnabled(true);
        update();
    }
//![8]

private:
    ViewportItem *viewport;
};


//![10]
MainWindow::MainWindow()
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);

//![9]
    GraphicsView *gv = new GraphicsView();
    view = new QGraphicsWebView();
    gv->viewportItem()->setWidget(view);
//![9]

    view->setPalette(QApplication::palette("QWebView"));
    view->page()->setPalette(QApplication::palette("QWebView"));
    view->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setInputMethodHints(Qt::ImhUrlCharactersOnly | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase | Qt::ImhPreferLowercase);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    addToolBarAction(toolBar, QWebPage::Back, "general_back");
    addToolBarAction(toolBar, QWebPage::Forward, "general_forward");
    addToolBarAction(toolBar, QWebPage::Reload, "general_refresh");
    addToolBarAction(toolBar, QWebPage::Stop, "general_stop");
    toolBar->addWidget(locationEdit);

    setCentralWidget(gv);

    QString url = QApplication::arguments().value(1);
    if (url.isEmpty())
        url = QLatin1String("www.google.com");
    locationEdit->setText(url);
    changeLocation();
}

void MainWindow::addToolBarAction(QToolBar *toolBar, QWebPage::WebAction webaction, const char *iconname)
{
    if (QAction *a = view->pageAction(webaction)) {
        a->setIcon(QIcon::fromTheme(iconname));
        toolBar->addAction(a);
    }
}

void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    view->load(QUrl::fromUserInput(locationEdit->text()));
    view->setFocus();
}

void MainWindow::adjustTitle()
{
    setWindowTitle(view->title());
}

void MainWindow::setProgress(int p)
{
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, (p > 0 && p < 100));
}

void MainWindow::finishLoading(bool)
{
    setProgress(100);
}
//![10]

#include "mainwindow.moc"
