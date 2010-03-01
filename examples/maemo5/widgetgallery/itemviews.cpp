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

#include "itemviews.h"

#include <QtGui>

void populateModel(QStandardItemModel *model)
{
    for (int i = 0; i < 20; ++i) {
        QList<QStandardItem*> row;
        for (int j = 0; j < 10; ++j) {
            QStandardItem *item;
            item = new QStandardItem(QString("%1 %2").
                    arg(i, 2, 10, QChar('0')).
                    arg(j, 2, 10, QChar('0')));
            row.append(item);
        }
        model->appendRow(row);

        for (int z = 0; z < 10; ++z) {
            QStandardItem *child = new QStandardItem(QString("Child %1")
                            .arg(z, 2, 10, QChar('0')));
            row.at(0)->setChild(z, child);
            for (int zz = 0; zz < 10; ++zz) {
                child->setChild(zz, new QStandardItem(QString("Child %1 %2")
                        .arg(z, 2, 10, QChar('0'))
                        .arg(zz, 2, 10, QChar('0'))));

            }
        }
    }
}

const PreviewWidget itemViewWidgets[] =
{
    { "QListView",          WidgetCreator<QListView, ModelSetter<> >::createWidget, 0 },
    { "QTreeView",          WidgetCreator<QTreeView, ModelSetter<> >::createWidget, 0},
    { "QTableView",         WidgetCreator<QTableView, ModelSetter<> >::createWidget, 0 },
    { "QColumnView",        WidgetCreator<QColumnView, ModelSetter<> >::createWidget, 0 },
    { 0, 0, 0 }
};


