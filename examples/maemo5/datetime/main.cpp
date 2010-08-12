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

// ![3]
// for dgettext
#include <libintl.h>
// for strftime
#include <time.h>
// ![3]

static void appendToLayout(QGridLayout *layout, const QString &label, const QString &value)
{
    int row = layout->rowCount();
    layout->addWidget(new QLabel("<p align=\"right\">" + label), row, 0);
    layout->addWidget(new QLabel(value), row, 1);
}

// returns static character arrays, do not free/delete!
// ![4]
static const char *getHildonTranslation(const char *string)
{
    const char *translation = ::dgettext("hildon-libs", string);
    if (qstrcmp(string, translation) == 0)
        return 0;
    return translation;
}
// ![4]

// ![6]
static QString formatHildonDate(const QDateTime &dt, const char *format)
{
    if (!format)
        return QString();

    char buf[255];
    struct tm tm = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    if (!dt.date().isNull()) {
        tm.tm_wday = dt.date().dayOfWeek() % 7;
        tm.tm_mday = dt.date().day();
        tm.tm_mon = dt.date().month() - 1;
        tm.tm_year = dt.date().year() - 1900;
    }
    if (!dt.time().isNull()) {
        tm.tm_sec = dt.time().second();
        tm.tm_min = dt.time().minute();
        tm.tm_hour = dt.time().hour();
    }

    size_t resultSize = ::strftime(buf, sizeof(buf), format, &tm);
    if (!resultSize)
        return QString();

    return QString::fromUtf8(buf, resultSize);
}
// ![6]

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QScrollArea area;

    QWidget *w = new QWidget;
    QGridLayout *layout = new QGridLayout(w);
    layout->setHorizontalSpacing(20);

// ![1]
    QString amPmStrings = QLocale::system().amText() + " / " + QLocale::system().pmText();
// ![1]

// ![2]
    QString longDate = QLocale::system().toString(QDate::currentDate(), QLocale::LongFormat);
    QString shortDate = QLocale::system().toString(QDate::currentDate(), QLocale::ShortFormat);
// ![2]

// ![0]
    QString shortTime = QLocale::system().toString(QTime::currentTime(), QLocale::ShortFormat);
// ![0]

// ![5]
    const char *hildonDateDayNameShort = getHildonTranslation("wdgt_va_date_day_name_short");
    const char *hildon24hFormat = getHildonTranslation("wdgt_va_24h_time");
// ![5]

// ![7]
    QDateTime current = QDateTime::currentDateTime();
    QString dayNameShort = formatHildonDate(current, hildonDateDayNameShort);
    QString time24h = formatHildonDate(current, hildon24hFormat);
// ![7]

    appendToLayout(layout, "AM/PM Strings:", amPmStrings);
    appendToLayout(layout, "Long Date Format:", longDate);
    appendToLayout(layout, "Short Date Format:", shortDate);
    appendToLayout(layout, "Short Time Format:", shortTime);
    appendToLayout(layout, "Short With Day Name:", dayNameShort);
    appendToLayout(layout, "24h Time Format:", time24h);

    area.setWidget(w);
    area.setWidgetResizable(true);

    area.show();

    return app.exec();
}

