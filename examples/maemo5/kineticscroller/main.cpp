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

#include <QtGui>
#include <QtMaemo5>
#include "window.h"

/*! 
    \class ScrollerWindow
    \brief An example of how to use the kinetic scroller.
    \internal

    This window uses two kinetic scrollers. One on the right side to scroll the table view and
    one on the left to allow scrolling of the settings widgets.
    You can change the parameters of both by changing the settings.

    The meaning of these settings is quite complex and I recommend to leave the defaults for
    your own applications.
*/


/*! \internal
\brief Create a nice model to demonstrate the scroller. */
QAbstractItemModel* createTableModel( int rows, int columns )
{
    QStandardItemModel *tableModel = new QStandardItemModel(0, 0);

    for (int i = 0; i < rows; ++i) {
        QList<QStandardItem*> row;
        for (int j = 0; j < columns; ++j) {
            QStandardItem *item;
            item = new QStandardItem(QString("%1 %2").
                    arg(i, 2, 10, QChar('0')).
                    arg(j, 2, 10, QChar('0')));
            item->setBackground(QBrush(QColor((256*i/rows)<<16 | (256*j/columns)<<8 | 80 )));
            row.append(item);
        }
        tableModel->appendRow(row);
    }

    return tableModel;
}

void ScrollerWindow::setLowFrictionEnabled(bool value)
{
    scroller1->setLowFrictionEnabled(value);
    scroller2->setLowFrictionEnabled(value);
}

void ScrollerWindow::setOvershootWhenScrollable()
{
    scroller1->setOvershootPolicy(QAbstractKineticScroller::OvershootWhenScrollable);
    scroller2->setOvershootPolicy(QAbstractKineticScroller::OvershootWhenScrollable);
}

void ScrollerWindow::setOvershootAlwaysOn()
{
    scroller1->setOvershootPolicy(QAbstractKineticScroller::OvershootAlwaysOn);
    scroller2->setOvershootPolicy(QAbstractKineticScroller::OvershootAlwaysOn);
}

void ScrollerWindow::setOvershootAlwaysOff()
{
    scroller1->setOvershootPolicy(QAbstractKineticScroller::OvershootAlwaysOff);
    scroller2->setOvershootPolicy(QAbstractKineticScroller::OvershootAlwaysOff);
}

void ScrollerWindow::setAutoMode()
{
    scroller1->setMode(QAbstractKineticScroller::AutoMode);
    scroller2->setMode(QAbstractKineticScroller::AutoMode);
}

void ScrollerWindow::setPushMode()
{
    scroller1->setMode(QAbstractKineticScroller::PushMode);
    scroller2->setMode(QAbstractKineticScroller::PushMode);
}

void ScrollerWindow::setAccelerationMode()
{
    scroller1->setMode(QAbstractKineticScroller::AccelerationMode);
    scroller2->setMode(QAbstractKineticScroller::AccelerationMode);
}

void ScrollerWindow::setDragInertia(int value)
{
    scroller1->setDragInertia(qreal(value) / qreal(100.0));
    scroller2->setDragInertia(qreal(value) / qreal(100.0));
    updateNumberLabels();
}

void ScrollerWindow::setDirectionErrorMargin(int value)
{
    scroller1->setDirectionErrorMargin(value);
    scroller2->setDirectionErrorMargin(value);
    updateNumberLabels();
}

void ScrollerWindow::setPanningThreshold(int value)
{
    scroller1->setPanningThreshold(value);
    scroller2->setPanningThreshold(value);
    updateNumberLabels();
}

void ScrollerWindow::setDecelerationFactor(int value)
{
    scroller1->setDecelerationFactor(qreal(value) / qreal(100.0));
    scroller2->setDecelerationFactor(qreal(value) / qreal(100.0));
    updateNumberLabels();
}

void ScrollerWindow::setFastVelocityFactor(int value)
{
    scroller1->setFastVelocityFactor(qreal(value) / qreal(1000.0));
    scroller2->setFastVelocityFactor(qreal(value) / qreal(1000.0));
    updateNumberLabels();
}

void ScrollerWindow::setMinimumVelocity(int value)
{
    scroller1->setMinimumVelocity(value);
    scroller2->setMinimumVelocity(value);
    updateNumberLabels();
}

void ScrollerWindow::setMaximumVelocity(int value)
{
    scroller1->setMaximumVelocity(value);
    scroller2->setMaximumVelocity(value);
    updateNumberLabels();
}

void ScrollerWindow::setAxisLockThreshold(int value)
{
    scroller1->setAxisLockThreshold(qreal(value) / qreal(100.0));
    scroller2->setAxisLockThreshold(qreal(value) / qreal(100.0));
    updateNumberLabels();
}

void ScrollerWindow::setFPS(int value)
{
    scroller1->setScrollsPerSecond(value);
    scroller2->setScrollsPerSecond(value);
    updateNumberLabels();
}

void ScrollerWindow::scrollToRandom()
{
    QPoint newPos( qrand()%table->horizontalScrollBar()->maximum(),
            qrand()%table->verticalScrollBar()->maximum());
    scroller1->scrollTo(newPos);
}

void ScrollerWindow::updateNumberLabels()
{
    numberLabels[0]->setText(QString::number(scroller1->dragInertia(), 'f', 2));
    numberLabels[1]->setText(QString::number(scroller1->directionErrorMargin(), 'f', 0));
    numberLabels[2]->setText(QString::number(scroller1->panningThreshold(), 'f', 0));
    numberLabels[3]->setText(QString::number(scroller1->decelerationFactor(), 'f', 2));
    numberLabels[4]->setText(QString::number(scroller1->fastVelocityFactor(), 'f', 3));
    numberLabels[5]->setText(QString::number(scroller1->minimumVelocity(), 'f', 0));
    numberLabels[6]->setText(QString::number(scroller1->maximumVelocity(), 'f', 0));
    numberLabels[7]->setText(QString::number(scroller1->axisLockThreshold(), 'f', 2));
    numberLabels[8]->setText(QString::number(scroller1->scrollsPerSecond()));
}

void ScrollerWindow::setupSlider(const char* text, int min, int max, int value, const char* slot, int *row, QGridLayout *layout)
{
    QLabel *label;
    QSlider *slider;

    label = new QLabel(tr(text));
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(value);
    connect( slider, SIGNAL(valueChanged(int)), this, slot);

    layout->addWidget(label, (*row), 0);
    layout->addWidget(slider, (*row)++, 1);
}

ScrollerWindow::ScrollerWindow()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter);

    // -- the right side
    table = new QTableView();
    table->setModel(createTableModel(200, 20));
    table->setItemDelegate(new QItemDelegate(this));
    table->setSelectionMode(QAbstractItemView::NoSelection);

    scroller1 = table->property("kineticScroller").value<QAbstractKineticScroller *>();
    if (!scroller1)
        qFatal("This example only works with QMaemo5Style as the default style for all QAbstractScrollAreas");

    // -- the left side of the splitter
    QWidget *left = new QWidget();
    QGridLayout *layoutLeft = new QGridLayout(left);
    QRadioButton *radio;
    QLabel *label;
    int row = 0;
    left->setLayout(layoutLeft);

    label = new QLabel(tr("Scroll Mode"));
    layoutLeft->addWidget(label, row++, 0, 1, 3);

    QHBoxLayout *layoutMode = new QHBoxLayout();
    layoutMode->setSpacing(0);

    QButtonGroup *groupMode = new QButtonGroup(this);
    radio = new QRadioButton(tr("Auto"));
    radio->setChecked(true);
    connect(radio, SIGNAL(clicked()), this, SLOT(setAutoMode()) );
    groupMode->addButton(radio);
    layoutMode->addWidget(radio);
    radio = new QRadioButton(tr("Push"));
    connect(radio, SIGNAL(clicked()), this, SLOT(setPushMode()) );
    groupMode->addButton(radio);
    layoutMode->addWidget(radio);
    radio = new QRadioButton(tr("Accel."));
    connect(radio, SIGNAL(clicked()), this, SLOT(setAccelerationMode()) );
    groupMode->addButton(radio);
    layoutMode->addWidget(radio);

    layoutLeft->addLayout(layoutMode, row++, 0, 1, 3);

    label = new QLabel(tr("Overshoot Policy"));
    layoutLeft->addWidget(label, row++, 0, 1, 3);

    QHBoxLayout *layoutPolicy = new QHBoxLayout();
    layoutPolicy->setSpacing(0);

    QButtonGroup *groupPolicy = new QButtonGroup(this);
    radio = new QRadioButton(tr("Auto"));
    radio->setChecked(true);
    connect(radio, SIGNAL(clicked()), this, SLOT(setOvershootWhenScrollable()) );
    groupPolicy->addButton(radio);
    layoutPolicy->addWidget(radio);
    radio = new QRadioButton(tr("Always"));
    connect(radio, SIGNAL(clicked()), this, SLOT(setOvershootAlwaysOn()) );
    groupPolicy->addButton(radio);
    layoutPolicy->addWidget(radio);
    radio = new QRadioButton(tr("Never"));
    connect(radio, SIGNAL(clicked()), this, SLOT(setOvershootAlwaysOff()) );
    groupPolicy->addButton(radio);
    layoutPolicy->addWidget(radio);

    layoutLeft->addLayout(layoutPolicy, row++, 0, 1, 3);

    setupSlider( "Drag inertia", 10, 100, scroller1->dragInertia()*100.0, SLOT(setDragInertia(int)), &row, layoutLeft);
    setupSlider( "Dir.err. marg.", 1, 50, scroller1->directionErrorMargin(), SLOT(setDirectionErrorMargin(int)), &row, layoutLeft);
    setupSlider( "Pan. thres.", 1, 100, scroller1->panningThreshold(), SLOT(setPanningThreshold(int)), &row, layoutLeft);
    setupSlider( "Decel. fact.", 1, 100, scroller1->decelerationFactor()*100.0, SLOT(setDecelerationFactor(int)), &row, layoutLeft);
    setupSlider( "Fast vel. fact.", 1, 100, scroller1->fastVelocityFactor()*1000.0, SLOT(setFastVelocityFactor(int)), &row, layoutLeft);
    setupSlider( "Min vel.", 1, 1000, scroller1->minimumVelocity(), SLOT(setMinimumVelocity(int)), &row, layoutLeft);
    setupSlider( "Max vel.", 1, 10000, scroller1->maximumVelocity(), SLOT(setMaximumVelocity(int)), &row, layoutLeft);
    setupSlider( "Axis lock", 0, 100, scroller1->axisLockThreshold(), SLOT(setAxisLockThreshold(int)), &row, layoutLeft);
    setupSlider( "FPS", 10, 60, scroller1->scrollsPerSecond(), SLOT(setFPS(int)), &row, layoutLeft);

    // create the number labels
    for (int i=0; i<9; i++) {
        numberLabels[i] = new QLabel();
        layoutLeft->addWidget(numberLabels[i], i+4, 2);
    }
    updateNumberLabels();

    QPushButton *button = new QPushButton(tr("Scroll to random"));
    connect( button, SIGNAL(clicked()), this, SLOT(scrollToRandom()));
    layoutLeft->addWidget(button, row++, 0, 1, 3);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(left);
    scrollArea->setWidgetResizable(true);

    scroller2 = scrollArea->property("kineticScroller").value<QAbstractKineticScroller *>();
    if (!scroller2)
        qFatal("This example only works with QMaemo5Style as the default style for all QAbstractScrollAreas");

    splitter->addWidget(scrollArea);

    splitter->addWidget(table);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ScrollerWindow toplevel;
    toplevel.show();

    return app.exec();
}

