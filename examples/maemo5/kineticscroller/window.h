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

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QAbstractKineticScroller;
class QTableView;
class QLabel;
class QGridLayout;


class ScrollerWindow : public QWidget
{
    Q_OBJECT

public:
    ScrollerWindow();

protected slots:
    void setLowFrictionEnabled(bool);

    void setAutoMode();
    void setPushMode();
    void setAccelerationMode();

    void setDragInertia(int);
    void setDirectionErrorMargin(int);
    void setPanningThreshold(int);
    void setDecelerationFactor(int);
    void setFastVelocityFactor(int);
    void setMinimumVelocity(int);
    void setMaximumVelocity(int);
    void setOvershootWhenScrollable();
    void setOvershootAlwaysOn();
    void setOvershootAlwaysOff();
    void setAxisLockThreshold(int);
    void setFPS(int);

    void scrollToRandom();

protected:

    void updateNumberLabels();
    void setupSlider( const char* text, int min, int max, int value, const char* slot, int *row, QGridLayout *layout);

    QTableView *table;
    QAbstractKineticScroller *scroller1;
    QAbstractKineticScroller *scroller2;

    QLabel *numberLabels[9];
};


#endif
