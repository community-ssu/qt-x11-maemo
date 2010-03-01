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

#include <QtGui>
#include <QMaemo5ValueButton>
#include <QMaemo5ListPickSelector>

/*! The PickSelectorAction class is a QWidgetAction that displays a QMaemo5ValueButton with a QMaemo5ListPickSelector */
class PickSelectorAction : public QWidgetAction {
    Q_OBJECT

public:
    PickSelectorAction(QObject *parent)
        : QWidgetAction(parent)
        , coolnessFactor(0)
    {
        setText(tr("CoolnesSelector"));

        strings << tr("Mild") << tr("Cold") << tr("Very cool") << tr("Frosty");

        stringsModel = new QStringListModel(this);
        stringsModel->setStringList(strings);
    }

    /*! Create the WidgetAction widget
        Just remember that every time the menu is opened we need to create a new widget.
    */
    QWidget * createWidget(QWidget *parent)
    {
        QMaemo5ListPickSelector *selector = new QMaemo5ListPickSelector(parent);
        selector->setModel(stringsModel);
        selector->setCurrentIndex(coolnessFactor);

        QMaemo5ValueButton *valueButton = new QMaemo5ValueButton(tr("Coolness"), parent);
        valueButton->setValueLayout(QMaemo5ValueButton::ValueUnderTextCentered); // Maemo5 style guide wants the menu entries centered.
        valueButton->setPickSelector(selector);

        connect(selector, SIGNAL(selected(const QString &)), this, SLOT(selectionChanged(const QString &)));

        return valueButton;
    }

public Q_SLOTS:
    void selectionChanged(const QString&);

protected:
   int coolnessFactor;

   QStringList strings;
   QStringListModel *stringsModel;
};

/*! Store the selected picker value back to the action */
void PickSelectorAction::selectionChanged(const QString& value)
{
    coolnessFactor = strings.indexOf(value);
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow()
    {
        // add some radio/exclusive actions (which will be converted to filter actions in the application menu)
        // The Maemo 5 UI specification limits this to up to four actions in up to one group
// ![0]
        QActionGroup *filterGroup = new QActionGroup(this);
        filterGroup->setExclusive(true);

        QAction *actEnable = new QAction(tr("Enabled"), filterGroup);
        actEnable->setCheckable(true);
        actEnable->setChecked(true);

        QAction *actDisable = new QAction(tr("Disabled"), filterGroup);
        actDisable->setCheckable(true);

        QAction *actWhatever = new QAction(tr("Whatever"), filterGroup);
        actWhatever->setCheckable(true);

        menuBar()->addActions(filterGroup->actions());
// ![0]


// ![1]
        menuBar()->addAction(tr("Action 1"));
        menuBar()->addAction(tr("Action 2"));
        menuBar()->addAction(tr("Action 3"));
// ![1]

// ![2]
        QAction *checkAction = new QAction(tr("Checkable"), this);
        checkAction->setCheckable(true);
        menuBar()->addAction(checkAction);
// ![2]

// ![3]
        menuBar()->addAction(new PickSelectorAction(this));
// ![3]

        QLabel *central = new QLabel(tr("Use the menu"));
        setCentralWidget(central);

        setWindowTitle(tr("Menu example"));
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
