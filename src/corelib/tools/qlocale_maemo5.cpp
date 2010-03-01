/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qlocale.h"
#include "qdatetime.h"
#include "qlibrary.h"
#include "qdebug.h"
#include "qvariant.h"


// for dgettext
#include <libintl.h>
#include <time.h>
#include <locale.h>

QT_BEGIN_NAMESPACE

// in qcoreapplication.cpp
extern bool qt_locale_initialized;

static QVariant posix2QtFormat(const char *v, const QVariant &fixAmPm = QVariant())
{
    if (!v)
        return QVariant();

    QString f = QString::fromUtf8(v);
    if (!fixAmPm.isNull()) {
        // oh what a hack. Hildon hard codes "am" and "pm" in the translation
        // string, so in order to get a generic formatting string, we have to
        // replace the "pm" with '%P'.
        f.replace(fixAmPm.toString(), QLatin1String("%P"));
    }

    QString qtFormat;
    bool fmt = false;
    for (int i = 0; i < f.length(); ++i) {
        const QChar c = f.at(i);

        if (fmt) {
            fmt = false;
            switch (c.toLatin1()) {
            case 'a':
                qtFormat += QLatin1String("ddd");
                break;
            case 'A':
                qtFormat += QLatin1String("dddd");
                break;
            case 'b':
            case 'h':
                qtFormat += QLatin1String("MMM");
                break;
            case 'B':
                qtFormat += QLatin1String("MMMM");
                break;
            case 'd':
            case 'e': // 'e' is not 100% correct - date should have a leading space instead of '0'
                qtFormat += QLatin1String("dd");
                break;
            case 'D':
                qtFormat += QLatin1String("MM/dd/yy");
                break;
            case 'F':
                qtFormat += QLatin1String("yyyy-MM-dd");
                break;
            case 'H':
            case 'k': // 'k' is not 100% correct - time should have a leading space instead of '0'
                qtFormat += QLatin1String("HH");
                break;
            case 'I': // not 100% correct - this should always be 01 to 12, independent of am/pm
            case 'l': // 'l' is not 100% correct - time should have a leading space instead of '0'
                qtFormat += QLatin1String("hh");
                break;
            case 'm':
                qtFormat += QLatin1String("MM");
                break;
            case 'M':
                qtFormat += QLatin1String("mm");
                break;
            case 'n':
                qtFormat += QLatin1Char('\n');
                break;
            case 'p':
                qtFormat += QLatin1String("AP");
                break;
            case 'P':
                qtFormat += QLatin1String("ap");
                break;
            case 'R':
                qtFormat += QLatin1String("HH:mm");
                break;
            case 'S':
                qtFormat += QLatin1String("ss");
                break;
            case 't':
                qtFormat += QLatin1Char('\t');
                break;
            case 'T':
                qtFormat += QLatin1String("HH:mm:ss");
                break;
            case 'y':
                qtFormat += QLatin1String("yy");
                break;
            case 'Y':
                qtFormat += QLatin1String("yyyy");
                break;
            case '%':
                qtFormat += QLatin1Char('%');
                break;
            case 'E':
            case 'O':
                // alternative formats not supported, ignoring
                fmt = true;
                break;
            case 'c':
            case 'C':
            case 'G':
            case 'j':
            case 'r':
            case 's':
            case 'u':
            case 'U':
            case 'V':
            case 'w':
            case 'W':
            case 'x':
            case 'X':
            case 'z':
            case 'Z':
            case '+':
            case '\0':
            default:
                qWarning("Unsupported date format character at %d (%s)", i, qPrintable(f));
                return QVariant();
            }
        } else if (c == QLatin1Char('%')) {
            fmt = true;
        } else {
            // everything else just copy + escape literally
            if (c == QLatin1Char('\'')) {
                // special handling for single quotes
                qtFormat += QLatin1String("\'\'");
            } else {
                // quote everything else to prevent it being recognized as format string
                qtFormat += QLatin1Char('\'');
                qtFormat += c;
                qtFormat += QLatin1Char('\'');
            }
        }
    }
    return qtFormat;
}

static const char *getHildonTranslation(const char *str)
{
    const char *translatedString = ::dgettext("hildon-libs", str);
    if (qstrcmp(str, translatedString) == 0)
        return 0;
    return translatedString;
}

static QVariant formatHildonDate(const char *str, const QDateTime &in)
{
    if (!str)
        return QVariant();

    char buf[255];
    struct tm tm = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if (!in.date().isNull()) {
        tm.tm_wday = in.date().dayOfWeek() % 7;
        tm.tm_mday = in.date().day();
        tm.tm_mon = in.date().month() - 1;
        tm.tm_year = in.date().year() - 1900;
    }
    if (!in.time().isNull()) {
        tm.tm_sec = in.time().second();
        tm.tm_min = in.time().minute();
        tm.tm_hour = in.time().hour();
    }

    size_t res = ::strftime(buf, sizeof(buf), str, &tm);
    if (!res)
        return QVariant();
    return QString::fromUtf8(buf, res);
}

// returns whether the system is set to am/pm or 24h clock.
// on error, default to 24h clock
static bool maemo5_is24h()
{
    struct GError;
    struct GConfClient;

    typedef void (*GObjectUnrefPtr)(void *);
    typedef GConfClient *(*ClientGetDefaultPtr)();
    typedef bool (*ClientGetBoolPtr)(GConfClient *client, const char *key, GError **err);
    typedef void (*TypeInitPtr)();


    TypeInitPtr initTypes = (TypeInitPtr)QLibrary::resolve(
            QLatin1String("gobject-2.0"), 0, "g_type_init");
    GObjectUnrefPtr gObjectUnref = (GObjectUnrefPtr)QLibrary::resolve(
            QLatin1String("gobject-2.0"), 0, "g_object_unref");
    ClientGetDefaultPtr getDefaultClient = (ClientGetDefaultPtr)QLibrary::resolve(
            QLatin1String("gconf-2"), 4, "gconf_client_get_default");
    ClientGetBoolPtr getBool = (ClientGetBoolPtr)QLibrary::resolve(
            QLatin1String("gconf-2"), 4, "gconf_client_get_bool");

    if (!initTypes || !gObjectUnref || !getDefaultClient || !getBool) {
        qWarning("Unable to use libgconf-2.so.4!");
        return true;
    }

    initTypes();
    GConfClient *defaultClient = getDefaultClient();
    if (!defaultClient) {
        qWarning("Unable to get GConf default client");
        return true;
    }

    bool is24 = getBool(defaultClient, "/apps/clock/time-format", 0);

    gObjectUnref(defaultClient);

    return is24;
}

struct QHildonSystemLocaleHelper
{
    QHildonSystemLocaleHelper()
    {
        if (!qt_locale_initialized) {
            setlocale(LC_ALL, "");
            qt_locale_initialized = true;
        }

        is24h = maemo5_is24h();

        const char *translation = getHildonTranslation("wdgt_va_am");
        if (translation)
            amString = QString::fromUtf8(translation);
        translation = getHildonTranslation("wdgt_va_pm");
        if (translation)
            pmString = QString::fromUtf8(translation);

        hildonTimeFormat24h = getHildonTranslation("wdgt_va_24h_time");
        timeFormat24h = posix2QtFormat(hildonTimeFormat24h);

        hildonTimeFormat12hAm = getHildonTranslation("wdgt_va_12h_time_am");
        hildonTimeFormat12hPm = getHildonTranslation("wdgt_va_12h_time_pm");
        timeFormat12h = posix2QtFormat(hildonTimeFormat12hPm, pmString);

        timeFormatShort = is24h ? timeFormat24h : timeFormat12h;

        hildonDateFormatLong = getHildonTranslation("wdgt_va_date_long");
        dateFormatLong = posix2QtFormat(hildonDateFormatLong);

        hildonDateFormatShort = getHildonTranslation("wdgt_va_date_short");
        dateFormatShort = posix2QtFormat(hildonDateFormatShort);

        if (!timeFormatShort.isNull() && !dateFormatShort.isNull())
            dateTimeFormatShort = dateFormatShort.toString()
                                  + QLatin1Char(' ')
                                  + timeFormatShort.toString();
    }

    const char *getTimeFormatString(const QTime &time)
    {
        if (is24h)
            return hildonTimeFormat24h;
        else if (time.hour() >= 12)
            return hildonTimeFormat12hPm;
        else
            return hildonTimeFormat12hAm;
    }

    bool is24h;
    QVariant amString;
    QVariant pmString;
    QVariant timeFormat24h;
    QVariant timeFormat12h;
    QVariant timeFormatShort;
    QVariant dateFormatLong;
    QVariant dateFormatShort;
    QVariant dateTimeFormatShort;

    const char *hildonTimeFormat24h;
    const char *hildonTimeFormat12hPm;
    const char *hildonTimeFormat12hAm;
    const char *hildonDateFormatLong;
    const char *hildonDateFormatShort;
};

Q_GLOBAL_STATIC(QHildonSystemLocaleHelper, hildonSystemLocaleHelper)

QVariant querySystemLocale_maemo5(QSystemLocale::QueryType type, const QVariant &in)
{
    QHildonSystemLocaleHelper *helper = hildonSystemLocaleHelper();
    if (!helper)
        return QVariant();

    switch (type) {
    case QSystemLocale::AMText:
        return helper->amString;
    case QSystemLocale::PMText:
        return helper->pmString;
    case QSystemLocale::TimeFormatShort:
        return helper->timeFormatShort;
    case QSystemLocale::DateFormatLong:
        return helper->dateFormatLong;
    case QSystemLocale::DateFormatShort:
        return helper->dateFormatShort;
    case QSystemLocale::DateTimeFormatShort:
        return helper->dateTimeFormatShort;

    case QSystemLocale::TimeToStringShort:
        return formatHildonDate(helper->getTimeFormatString(in.toTime()),
                                QDateTime(QDate(), in.toTime()));
    case QSystemLocale::DateToStringLong:
        return formatHildonDate(helper->hildonDateFormatLong, QDateTime(in.toDate()));
    case QSystemLocale::DateToStringShort:
        return formatHildonDate(helper->hildonDateFormatShort, QDateTime(in.toDate()));
    case QSystemLocale::DateTimeToStringShort: {
        QByteArray ba = helper->hildonDateFormatShort;
        ba += ' ';
        ba += helper->getTimeFormatString(in.toDateTime().time());
        if (ba.length() == 1) // Just the space between time and date
            return QVariant();
        return formatHildonDate(ba.constData(), in.toDateTime()); }

    default:
        return QVariant();
    }
}

QT_END_NAMESPACE

