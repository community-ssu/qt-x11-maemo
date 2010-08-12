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
#include <QtDBus>
#include <QtDebug>
#include <QMaemo5InformationBox>
#include <QMaemo5ValueButton>
#include <QMaemo5TimePickSelector>
#include <QMaemo5DatePickSelector>
#include <QMaemo5ListPickSelector>
#include <QMaemo5EditBar>

#include <mce/mode-names.h>
#include <mce/dbus-names.h>

#include <unistd.h>

#include "window.h"

class RotatedLabel : public QWidget {
public:
    RotatedLabel(const QString &str, int rotation)
        : QWidget(), m_rotation(rotation), m_text(str)
    {
        QFontMetrics fm = fontMetrics();
        int w = fm.width(m_text);
        setMinimumSize(w + 8, w + 8);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.translate(rect().center());
        p.rotate(m_rotation);
        p.drawText(rect().translated(-rect().center()), Qt::AlignCenter, m_text);
    }

private:
    int m_rotation;
    QString m_text;
};

class ProgressBarAction : public QWidgetAction {
public:
    ProgressBarAction(QObject *parent)
        : QWidgetAction(parent)
    {
        setText(tr("Progress"));
    }

    QWidget * createWidget(QWidget *parent)
    {
        return new QProgressBar(parent);
    }
};

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_Maemo5StackedWindow);
    setAttribute(Qt::WA_Maemo5AutoOrientation);

    m_fullscreen = new QAction(QIcon::fromTheme("general_fullsize"), QString(), this);
    m_fullscreen->setCheckable(true);
    QActionGroup *orientation = new QActionGroup(this);
    orientation->setExclusive(true);
    m_portrait = new QAction(QIcon::fromTheme("general_portrait", QIcon(QLatin1String(":/images/portrait"))), QString(), orientation);
    m_portrait->setCheckable(true);
    m_portrait->setChecked(false);
    m_landscape = new QAction(QIcon::fromTheme("general_landscape", QIcon(QLatin1String(":/images/landscape"))), QString(), orientation);
    m_landscape->setCheckable(true);
    m_landscape->setChecked(false);
    m_auto = new QAction(tr("Auto"), orientation);
    m_auto->setCheckable(true);
    m_auto->setChecked(true);

    connect(m_fullscreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));
    connect(orientation, SIGNAL(triggered(QAction *)), this, SLOT(orientationChanged(QAction *)));

    // add some radio/exclusive actions (which will be converted to filter actions in the application menu)
    // The Maemo 5 UI specification limits this to up to four actions in up to one group
    QActionGroup *filterGroup = new QActionGroup(this);
    filterGroup->setExclusive(true);
    m_enable_children = new QAction(tr("Enabled"), filterGroup);
    m_enable_children->setCheckable(true);
    m_enable_children->setChecked(true);
    m_disable_children = new QAction(tr("Disabled"), filterGroup);
    m_disable_children->setCheckable(true);
    QAction *whatever = new QAction(tr("Whatever"), filterGroup);
    whatever->setCheckable(true);

    connect(m_enable_children, SIGNAL(toggled(bool)), this, SLOT(enableChildren(bool)));

    menuBar()->addActions(filterGroup->actions());
}

Window::~Window()
{
}

void Window::orientationChanged(QAction *action)
{
    if (action == m_auto)
        setAttribute(Qt::WA_Maemo5AutoOrientation, true);
    else if (action == m_portrait)
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
    else
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);

    m_landscape->setChecked(action == m_landscape);
    m_portrait->setChecked(action == m_portrait);
    m_auto->setChecked(action == m_auto);

    relayout();
}

void Window::enableChildren(bool b)
{
    foreach (QWidget *w, findChildren<QWidget *>()) {
        if (!qobject_cast<QMenuBar *>(w))
            w->setEnabled(b);
    }
}

void Window::toggleFullScreen(bool b)
{
    setWindowState(b ? windowState() | Qt::WindowFullScreen
                     : windowState() & ~Qt::WindowFullScreen);
}

void Window::relayout()
{
}

MainWindow::MainWindow()
    : Window(0)
{
    menuBar()->addAction(tr("Show a MessageBox 1"), this, SLOT(showOkCancelBox()));
    menuBar()->addAction(tr("Show a MessageBox 2"), this, SLOT(showYesNoBox()));
    menuBar()->addAction(tr("Show a Progress Dialog"), this, SLOT(showProgress()));
    menuBar()->addAction(tr("Show a Banner"), this, SLOT(showBanner()));
    menuBar()->addAction(tr("Show a Note"), this, SLOT(showNote()));
    menuBar()->addAction(tr("Show a Expose"), this, SLOT(showExpose()));
    menuBar()->addAction(tr("Show a Complex Note"), this, SLOT(showComplexNote()));

    // add a checkbox action
    QAction *checkAction = new QAction(tr("Busy"), this);
    connect(checkAction, SIGNAL(triggered(bool)), this, SLOT(beBusy(bool)));
    checkAction->setCheckable(true);
    menuBar()->addAction(checkAction);

    // add a widget action
    menuBar()->addAction(new ProgressBarAction(this));

    QToolBar *tb = new QToolBar(this);
    tb->addAction(m_fullscreen);
    tb->addAction(m_portrait);
    tb->addAction(m_landscape);
    tb->addAction(m_auto);
    addToolBar(tb);

    QWidget *central = new QWidget(this);
    QGridLayout *lay = new QGridLayout(central);

    QPushButton *pb;
    pb = new QPushButton(tr("Show various Widgets"));
    lay->addWidget(pb, 0, 0);
    connect(pb, SIGNAL(clicked()), this, SLOT(showWidgets1()));
    pb = new QPushButton(tr("Show an editable List"));
    lay->addWidget(pb, 0, 0);
    connect(pb, SIGNAL(clicked()), this, SLOT(showWidgets2()));

    setCentralWidget(central);
    relayout();

    // The Maemo5 window manager mis-calculates the length of the window title
    // and won't show the 'v' indicator in Portrait mode if the title is "Stacked"
    setWindowTitle(tr("Maemo"));
}

void MainWindow::relayout()
{
    QGridLayout *grid = static_cast<QGridLayout *>(centralWidget()->layout());
    bool isPortrait = m_portrait->isChecked();
    bool wasPortrait = (grid->columnCount() == 1);

    if (isPortrait != wasPortrait) {
        QList<QLayoutItem *> list;
        while (grid->count())
            list << grid->takeAt(0);
        int c = 0, r = 0;
        foreach (QLayoutItem *li, list) {
            grid->addItem(li, r, c);
            c++;
            if (c == (isPortrait ? 1 : 2)) {
                c = 0;
                r++;
            }
        }
    }
}


WidgetWindow::WidgetWindow(int page, MainWindow *parent)
    : Window(parent)
{
    QWidget *central = new QWidget();

    page = qBound(0, page, 3);
    QStandardItemModel *testmodel = new QStandardItemModel(this);
    for (int i = 0; i < 50; ++i)
        testmodel->appendRow(new QStandardItem(QIcon::fromTheme("chat_smiley_happy"), tr("Item number %1").arg(i+1)));

    if (page == 0) {
        setWindowTitle(tr("Widgets"));

        QScrollArea *scroll = new QScrollArea();
        QVBoxLayout *toplay = new QVBoxLayout(central);
        toplay->addWidget(scroll);

        QWidget *area = new QWidget();
        QVBoxLayout *lay = new QVBoxLayout(area);
        lay->setSpacing(0);
        lay->addWidget(new QLabel(tr("This is a Maemo5 Stacked Dialog - scroll down to see more.")));
        lay->addWidget(new QLineEdit(tr("LineEdit")));
        lay->addWidget(new QSpinBox());
        lay->addWidget(new QSlider(Qt::Horizontal));
        QProgressBar *prb = new QProgressBar();
        prb->setValue(50);
        lay->addWidget(prb);
        lay->addWidget(new QCheckBox(tr("CheckBox")));
        QButtonGroup *radios = new QButtonGroup(central);
        QHBoxLayout *radiolay = new QHBoxLayout();
        radiolay->setSpacing(0);
        foreach(const QString &str, QStringList() << tr("One") << tr("Two") << tr("Three")) {
            QRadioButton *radio = new QRadioButton(str);
            radiolay->addWidget(radio);
            if (radios->buttons().isEmpty())
                radio->setChecked(true);
            radios->addButton(radio);
        }
        lay->addLayout(radiolay);

        QButtonGroup *buttons = new QButtonGroup(central);
        QMaemo5ValueButton *v1 = new QMaemo5ValueButton(tr("Value beside text"));
        v1->setIcon(QIcon::fromTheme("chat_smiley_happy"));
        v1->setValueLayout(QMaemo5ValueButton::ValueBesideText);
        v1->setValueText(tr("ValueBesideText"));
        buttons->addButton(v1);
        lay->addWidget(v1);
        QMaemo5ValueButton *v2 = new QMaemo5ValueButton(tr("Value under text"));
        v2->setIcon(QIcon::fromTheme("chat_smiley_nerd"));
        v2->setValueLayout(QMaemo5ValueButton::ValueUnderText);
        QMaemo5ListPickSelector *listpicker = new QMaemo5ListPickSelector();
        listpicker->setModel(testmodel);
        v2->setPickSelector(listpicker);
        buttons->addButton(v2);
        lay->addWidget(v2);
        QMaemo5ValueButton *v3 = new QMaemo5ValueButton(tr("Time picker (left aligned)"));
        v3->setIcon(QIcon::fromTheme("chat_smiley_tongue"));
        v3->setValueLayout(QMaemo5ValueButton::ValueUnderText);
        v3->setPickSelector(new QMaemo5TimePickSelector());
        buttons->addButton(v3);
        lay->addWidget(v3);
        QMaemo5ValueButton *v4 = new QMaemo5ValueButton(tr("Date Picker (centered)"));
        v4->setIcon(QIcon::fromTheme("chat_smiley_surprised"));
        v4->setValueLayout(QMaemo5ValueButton::ValueUnderTextCentered);
        v4->setPickSelector(new QMaemo5DatePickSelector());
        buttons->addButton(v4);
        lay->addWidget(v4);

        QComboBox *com = new QComboBox();
        com->setEditable(false);
        com->setModel(testmodel);
        lay->addWidget(com);
        com = new QComboBox();
        com->setEditable(true);
        com->setModel(testmodel);
        lay->addWidget(com);
        QTextEdit *te = new QTextEdit(tr("Autoresizing TextEdit"));
        connect(te, SIGNAL(textChanged()), this, SLOT(textEditChanged()));
        connect(te, SIGNAL(cursorPositionChanged()), this, SLOT(textEditChanged()));
        lay->addWidget(te);
        updateEditWidget(te);

        QPlainTextEdit *pte = new QPlainTextEdit(tr("Autoresizing PlainTextEdit"));
        connect(pte, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
        connect(pte, SIGNAL(cursorPositionChanged()), this, SLOT(textEditChanged()));
        lay->addWidget(pte);
        updateEditWidget(pte);

        area->setLayout(lay);
        scroll->setWidget(area);
        scroll->setWidgetResizable(true);
    }
    setCentralWidget(central);
}

void WidgetWindow::textEditChanged()
{
    updateEditWidget(qobject_cast<QFrame *>(sender()));
}

void WidgetWindow::plainTextEditChanged()
{
    updateEditWidget(qobject_cast<QFrame *>(sender()));
}

void WidgetWindow::updateEditWidget(QFrame *widget)
{
    if (!widget)
        return;

    QTextEdit *te = qobject_cast<QTextEdit *>(widget);
    QPlainTextEdit *pte = qobject_cast<QPlainTextEdit *>(widget);

    if (!te && !pte)
        return;

    QTextDocument *doc = te ? te->document() : pte->document();
    QRect cursor = te ? te->cursorRect() : pte->cursorRect();

    QSize s = doc->size().toSize();
    if (pte)
        s.setHeight((s.height() + 1) * widget->fontMetrics().lineSpacing());

    QRect fr = widget->frameRect();
    QRect cr = widget->contentsRect();

    widget->setMinimumHeight(qMax(70, s.height() + (fr.height() - cr.height() - 1)));

    // make sure the cursor is visible in case we have a QAbstractScrollArea parent
    QPoint pos = widget->pos();
    QWidget *pw = widget->parentWidget();
    while (pw) {
        if (pw->parentWidget()) {
            if (QAbstractScrollArea *area = qobject_cast<QAbstractScrollArea *>(pw->parentWidget()->parentWidget())) {
                if (QAbstractKineticScroller *ks = area->property("kineticScroller").value<QAbstractKineticScroller *>()) {
                    ks->ensureVisible(pos + cursor.center(), 10 + cursor.width(), 2 * cursor.height());
                }
                break;
            }
        }
        pos = pw->mapToParent(pos);
        pw = pw->parentWidget();
    }
}

ListWindow::ListWindow(MainWindow *parent)
    : Window(parent)
{
    QWidget *central = new QWidget();

    setWindowTitle(tr("List"));

    QVBoxLayout *lay = new QVBoxLayout(central);
    lay->setSpacing(8);
    lay->setContentsMargins(0, 0, 0, 0);

    QMaemo5EditBar *eb = new QMaemo5EditBar(tr("Edit this list..."));
    lay->addWidget(eb);
    QPushButton *pb = new QPushButton(tr("Action"));
    eb->addButton(pb);
    connect(pb, SIGNAL(clicked()), this, SLOT(actionClicked()));

    QListWidget *list = new QListWidget();
    for (int i = 0; i < 100; ++i)
        list->addItem(tr("Item #%1").arg(i));
    lay->addWidget(list);

    setCentralWidget(central);
    toggleFullScreen(true);
}

void ListWindow::actionClicked()
{
    QMaemo5InformationBox::information(this, tr("Action was clicked"), QMaemo5InformationBox::DefaultTimeout);
}

void MainWindow::showWidgets1()
{
    WidgetWindow *w = new WidgetWindow(0, this);
    w->show();
}

void MainWindow::showWidgets2()
{
    ListWindow *w = new ListWindow(this);
    w->show();
}

void MainWindow::showOkCancelBox()
{
    QMessageBox::information(this, tr("A MessageBox"), tr("This is a Maemo5 Message Box that has a (hidden) cancel button. (You may also click on <a href=\"http://qt.nokia.com\">this link</a> to launch the web browser.)"), QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Apply);
}

void MainWindow::showYesNoBox()
{
    QMessageBox::information(this, tr("Another MessageBox"), tr("This is a Maemo5 Message Box without a cancel button."), QMessageBox::Yes | QMessageBox::No);
}

void MainWindow::showProgress()
{
    QProgressDialog p(tr("Label"), tr("Stop"), 0, 100, this);
    p.setMinimumDuration(0);
    p.setWindowModality(Qt::WindowModal);

    for (int i =0; i <= 100; i++) {
        p.setValue(i);
        if (p.wasCanceled())
            return;
        ::usleep(250*1000);
    }
}

void MainWindow::showBanner()
{
    QMaemo5InformationBox::information(this, tr("This is a <b>Maemo5</b> <i>banner</i>"), QMaemo5InformationBox::DefaultTimeout);
}

void MainWindow::showNote()
{
    QMaemo5InformationBox::information(this, tr("This<br>is<br>a<br><b>Maemo5</b><br><i>note</i>"), QMaemo5InformationBox::NoTimeout);
}

void MainWindow::showComplexNote()
{
    QMaemo5InformationBox *box = new QMaemo5InformationBox(this);
    box->setWidget(new RotatedLabel(tr("Qt for Maemo5"), 45));
    box->setTimeout(QMaemo5InformationBox::NoTimeout);
    box->exec();
}

void MainWindow::showExpose()
{
    QDBusConnection c = QDBusConnection::sessionBus();
    QDBusMessage m = QDBusMessage::createSignal("/","com.nokia.hildon_desktop","exit_app_view");
    c.send(m);
}

void MainWindow::beBusy(bool on)
{
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, on);
}

