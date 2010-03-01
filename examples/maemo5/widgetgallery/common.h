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
