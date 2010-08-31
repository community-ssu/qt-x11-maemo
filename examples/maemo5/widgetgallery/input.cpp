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

#include "input.h"

#include <QtGui>

extern const char lotsOfText[];

static const Properties editableProperties = Properties()
            << Property("editable", true);

static const Properties passwordEcho = Properties()
            << Property("echoMode", QLineEdit::PasswordEchoOnEdit);

static const Properties plainTextProperties = Properties()
            << Property("plainText", QLatin1String(lotsOfText));

static const Properties valueProperties = Properties()
            << Property("value", 42);

static const Properties doubleProperties = Properties()
            << Property("value", 42.42);

static const Properties sliderProperties = Properties()
            << Property("orientation", Qt::Horizontal)
            << Property("value", 42);

class InputHintsCreator : public QObject {
    Q_OBJECT
public:
    static void createWidget(QMainWindow *parent, const Properties &)
    {
        QWidget *topLevel = newToplevel(parent);

        QVBoxLayout *layout = new QVBoxLayout(topLevel);

        QListWidget *list = new QListWidget;
        QLineEdit *le = new QLineEdit;

        list->setFocusPolicy(Qt::NoFocus);
        new InputHintsCreator(list, le);

        insertItem(list, "Hidden Text", Qt::ImhHiddenText);
        insertItem(list, "No Auto Uppercase", Qt::ImhNoAutoUppercase);
        insertItem(list, "Prefer Numbers", Qt::ImhPreferNumbers);
        insertItem(list, "Prefer Uppercase", Qt::ImhPreferUppercase);
        insertItem(list, "Prefer Lowercase", Qt::ImhPreferLowercase);
        insertItem(list, "No Predicitive Text", Qt::ImhNoPredictiveText);
        insertItem(list, "Digits only", Qt::ImhDigitsOnly);
        insertItem(list, "Numbers only", Qt::ImhFormattedNumbersOnly);
        insertItem(list, "Uppercase only", Qt::ImhUppercaseOnly);
        insertItem(list, "Lowercase only", Qt::ImhLowercaseOnly);
        insertItem(list, "Dialable only", Qt::ImhDialableCharactersOnly);
        insertItem(list, "eMail only", Qt::ImhEmailCharactersOnly);
        insertItem(list, "URL only", Qt::ImhUrlCharactersOnly);

        layout->addWidget(list);
        layout->addWidget(le);

        topLevel->setWindowTitle("QLineEdit with InputMethodHints");
        topLevel->show();
    }
    static void insertItem(QListWidget *list, const char *text, int hints)
    {
        QListWidgetItem *lwi = new QListWidgetItem(QLatin1String(text), list);
        lwi->setData(Qt::UserRole, hints);
        lwi->setFlags(lwi->flags() | Qt::ItemIsUserCheckable);
        lwi->setCheckState(Qt::Unchecked);
    }

    InputHintsCreator(QListWidget *list, QLineEdit *le)
        : m_listwidget(list), m_lineedit(le)
    {
        connect(list, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(itemChanged(QListWidgetItem *)));
    }

private slots:
    void itemChanged(QListWidgetItem *item)
    {
        if (item) {
            Qt::InputMethodHints hints = m_lineedit->inputMethodHints();
            Qt::InputMethodHint flag = static_cast<Qt::InputMethodHint>(item->data(Qt::UserRole).toInt());
            if (item->checkState() == Qt::Checked)
                hints |= flag;
            else
                hints &= ~flag;
            m_lineedit->setInputMethodHints(hints);
        }
    }
private:
    QListWidget *m_listwidget;
    QLineEdit *m_lineedit;
};

const PreviewWidget inputWidgets[] =
{
    { "QComboBox (r/o)",    WidgetCreator<QComboBox, ModelSetter<QComboBox> >::createWidget, 0 },
    { "QComboBox (r/w)",    WidgetCreator<QComboBox, ModelSetter<QComboBox> >::createWidget, &editableProperties },
    { "QFontComboBox",      WidgetCreator<QFontComboBox>::createWidget, 0 },
    { "QLineEdit",          WidgetCreator<QLineEdit>::createWidget, 0 },
    { "QLineEdit (Password)", WidgetCreator<QLineEdit>::createWidget, &passwordEcho },
    { "QLineEdit with Input Hints", InputHintsCreator::createWidget, 0 },
    { "QTextEdit",          WidgetCreator<QTextEdit>::createWidget, &plainTextProperties },
    { "QPlainTextEdit",     WidgetCreator<QPlainTextEdit>::createWidget, &plainTextProperties },
    { "QSpinBox",           WidgetCreator<QSpinBox>::createWidget, &valueProperties },
    { "QDoubleSpinBox",      WidgetCreator<QDoubleSpinBox>::createWidget, &doubleProperties },
    // time & date only
    { "QDateTimeEdit",      WidgetCreator<QDateTimeEdit>::createWidget, 0 },
    { "QDial",              WidgetCreator<QDial>::createWidget, 0 },
    { "QScrollBar",         WidgetCreator<QScrollBar>::createWidget, &valueProperties },
    { "QSlider",            WidgetCreator<QSlider>::createWidget, &sliderProperties },
    { 0, 0, 0 }
};

#include "input.moc"
