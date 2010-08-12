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
