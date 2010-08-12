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

#ifndef COMMON_H
#define COMMON_H

#include <QtCore/qhash.h>
#include <QtCore/qpair.h>
#include <QtCore/qvariant.h>

#include <QtGui/qstandarditemmodel.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qboxlayout.h>

class QAbstractItemView;

// convenience typedef to specify a property
typedef QPair<QString, QVariant> Property;

// convenience hash for properties, with a convenience << operator
class Properties : public QHash<QString, QVariant>
{
public:
    inline Properties &operator<<(const Property &val)
    {
        insert(val.first, val.second);
        return *this;
    }
};

// the function that'll create a gallery widget
typedef void (*CreateWidgetFunc)(QMainWindow *parent, const Properties &properties);

// one single entry
struct PreviewWidget
{
    const char *text;
    CreateWidgetFunc func;
    const Properties *properties;
};

// convenience class that does absolutely nothing. Default if
// no extra initialization is required
template <typename T>
struct NoExtraInitialization
{
    static inline void init(T *) {}
};

extern QWidget *newToplevel(QWidget* parent, Qt::WindowFlags f = 0);
extern void addStatusWidget(QBoxLayout *layout, QWidget *child);
extern void setProperties(QObject *obj, const Properties &props);

// create a new top level widget for T, with optional extra initialization
template <typename T, class EXTRA = NoExtraInitialization<T> >
struct WidgetCreator
{
    static void createWidget(QMainWindow *parent, const Properties &properties)
    {
        QWidget *topLevel = newToplevel(parent);

        QVBoxLayout *layout = new QVBoxLayout(topLevel);

        T *t = new T;
        setProperties(t, properties);
        topLevel->setWindowTitle(t->metaObject()->className());
        EXTRA::init(t);
        layout->addWidget(t);

        addStatusWidget(layout, t);
        topLevel->show();
    }
};

// creates a new dialog
template <typename T, class EXTRA = NoExtraInitialization<T> >
struct DialogCreator
{
    static void createDialog(QMainWindow *parent, const Properties &properties)
    {
        T t(parent);
        setProperties(&t, properties);
        EXTRA::init(&t);
        t.exec();
    }
};

extern void populateModel(QStandardItemModel *model);

template <typename T = QAbstractItemView>
struct ModelSetter
{
    static void init(T *view)
    {
        QStandardItemModel *model = new QStandardItemModel(0, 0, view);
        populateModel(model);
        view->setModel(model);
    }
};

#endif
