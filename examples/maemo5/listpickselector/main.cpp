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

static void populateListModel(QStandardItemModel *model)
{
    enum { ItemCount = 50 };

    for (int i = 0; i < ItemCount; ++i) {
        QStandardItem *item = new QStandardItem(QString("%1 coins").arg(i*12+5));
        item->setTextAlignment(Qt::AlignCenter); // the Maemo 5 design spec recommends this.
        item->setEditable(false); // prevent editing of the item
        model->appendRow(item);
    }
}

static void populateTableModel(QStandardItemModel *model)
{
    enum { ItemCount = 50 };

    const char *itemNames[] = { "potion", "ring", "amulet", "wand", "figurine" };
    const char *itemColors[] = { "black", "white", "red", "mauve", "blue", "green",
                                 "yellow", "ultramarine", "pink", "purple" };

    for (int i = 0; i < ItemCount; ++i) {
        QList<QStandardItem*> row;
        QStandardItem *item;
        item = new QStandardItem((i % 10 == 0) ? QString(itemNames[i / 10]) : QString());
        item->setEditable(false);
        row.append(item);
        item = new QStandardItem(QString("%1 %2").arg(QString(itemColors[i % 10])).
                    arg(QString(itemNames[i / 10])));
        item->setEditable(false);
        row.append(item);
        item = new QStandardItem(QString("%1 gold coins").arg(i * 10 + (i % 10) * 15 + 13));
        item->setTextAlignment(Qt::AlignCenter); // the Maemo 5 design spec recommends this.
        item->setEditable(false);
        row.append(item);
        model->appendRow(row);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

// ![0]
    QStandardItemModel model(0, 1);
    populateListModel(&model);
// ![0]

    QWidget toplevel;
    QVBoxLayout *layout = new QVBoxLayout(&toplevel);

// ![1]
    QMaemo5ValueButton *button1 = new QMaemo5ValueButton("Value besides text");
    button1->setValueLayout(QMaemo5ValueButton::ValueBesideText);
    QMaemo5ListPickSelector *selector1 = new QMaemo5ListPickSelector;
    selector1->setModel(&model);
    // not setting the current index means that the value is empty.
    button1->setPickSelector(selector1);
// ![1]

// ![2]
    QStandardItemModel tableModel(0, 0);
    populateTableModel(&tableModel);
// ![2]

// ![3]
    QMaemo5ValueButton *button2 = new QMaemo5ValueButton("Value under text");
    button2->setValueLayout(QMaemo5ValueButton::ValueUnderText);
    QMaemo5ListPickSelector *selector2 = new QMaemo5ListPickSelector;
    selector2->setModel(&tableModel);
    selector2->setModelColumn(2);
    selector2->setCurrentIndex(5);
    button2->setPickSelector(selector2);
// ![3]

    // create a custom view we want a table view instead of a list
    QTableView *view = new QTableView();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // try to get a sensible column width and row height
    view->setModel(&tableModel); // set the model in order to get correct column widths
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    view->horizontalHeader()->setStretchLastSection(true);

    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setVisible(false);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    // five rows should be visible as a default
    if (view->verticalHeader()->sectionSize(0)>0)
        view->setMinimumHeight(view->verticalHeader()->sectionSize(0) * 5);

    QMaemo5ValueButton *button3 = new QMaemo5ValueButton("Value centered under text");
    button3->setValueLayout(QMaemo5ValueButton::ValueUnderTextCentered);
    QMaemo5ListPickSelector *selector3 = new QMaemo5ListPickSelector;
    selector3->setModel(&tableModel);
    selector3->setModelColumn(2);
    selector3->setView(view); // set our new custom view
    selector3->setCurrentIndex(0);
    button3->setPickSelector(selector3);

    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    toplevel.show();

    return app.exec();
}

