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

