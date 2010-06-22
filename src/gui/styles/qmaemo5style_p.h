/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QMAEMOS5STYLE_P_H
#define QMAEMOS5STYLE_P_H

#include <QMap>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef MAEMO_CHANGES
#define MAEMO_CHANGES
#endif
#include <gtk-2.0/gtk/gtkenums.h>

#include <private/qgtkstyle_p.h>

#include <hildon-fm-2/hildon/hildon-file-chooser-dialog.h>
#include <hildon-1/hildon/hildon-button.h>
#include <hildon-1/hildon/hildon-app-menu.h>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

#if defined(Q_WS_MAEMO_5)

typedef void (*Ptr_gtk_widget_show) (GtkWidget *);
typedef void (*Ptr_gtk_widget_set_name) (GtkWidget *, const gchar *);
typedef GtkWidget* (*Ptr_gtk_text_view_new)(void);
typedef void (*Ptr_gtk_widget_get_size_request)(GtkWidget *, gint *, gint *);
typedef void (*Ptr_gtk_button_set_label)(GtkButton *, const gchar *);
typedef gboolean (*Ptr_gtk_style_lookup_color)(GtkStyle *, const gchar *, GdkColor *);
typedef GtkWidget *(*Ptr_gtk_hbox_new)(gboolean, gint);
typedef void (*Ptr_gtk_box_pack_start)(GtkBox *, GtkWidget *, gboolean, gboolean, guint);
typedef void (*Ptr_gtk_box_pack_end)(GtkBox *, GtkWidget *, gboolean, gboolean, guint);
typedef void (*Ptr_gtk_widget_set_size_request)(GtkWidget *, gint, gint);

typedef void (*Ptr_hildon_init)(void);
typedef GtkWidget* (*Ptr_hildon_entry_new)(HildonSizeType);
typedef GtkWidget* (*Ptr_hildon_number_editor_new) (int,int);
typedef GtkWidget* (*Ptr_hildon_app_menu_new)(void);
typedef void (*Ptr_hildon_app_menu_add_filter)(HildonAppMenu*, GtkButton*);
typedef GtkWidget* (*Ptr_hildon_edit_toolbar_new_with_text)(const gchar *, const gchar *);
typedef GtkWidget* (*Ptr_hildon_button_new)(HildonSizeType, HildonButtonArrangement);
typedef GtkWidget* (*Ptr_hildon_check_button_new)(HildonSizeType);
typedef GtkWidget* (*Ptr_hildon_pannable_area_new)();
typedef void (*Ptr_hildon_gtk_widget_set_theme_size)(GtkWidget*, HildonSizeType);
typedef GtkWidget* (*Ptr_hildon_dialog_new_with_buttons)(const gchar*, GtkWindow*, GtkDialogFlags, const gchar*, ...);
typedef GtkWidget* (*Ptr_hildon_note_new_information)(GtkWindow *, const gchar *);

typedef GtkWidget* (*Ptr_hildon_file_chooser_dialog_new)(GtkWindow *parent, GtkFileChooserAction action);
typedef void (*Ptr_hildon_file_chooser_dialog_set_extension)(HildonFileChooserDialog *self, const gchar *extension);
typedef GtkWidget* (*Ptr_hildon_file_chooser_dialog_add_extensions_combo)(HildonFileChooserDialog *self, char **extensions, char **ext_names);

class QAbstractScrollArea;
class QTimer;
class QTimeLine;
class ScrollBarFader;

class QMaemo5StylePrivate : public QGtkStylePrivate
{
public:

    virtual void resolveGtk() const;
    virtual void initGtkMenu() const;
    virtual void initGtkWidgets() const;
    virtual void applyCustomPaletteHash();

    static void setupGtkFileChooser(GtkWidget* gtkFileChooser, QWidget *parent,
            const QString &dir, const QString &filter, QString *selectedFilter,
            QFileDialog::Options options, bool isSaveDialog = false,
            QMap<GtkFileFilter *, QString> *filterMap = 0);

    static QString openFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                QString *selectedFilter, QFileDialog::Options options);
    static QString saveFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                QString *selectedFilter, QFileDialog::Options options);
    static QString openDirectory(QWidget *parent, const QString &caption, const QString &dir,
                                QFileDialog::Options options);
    static QStringList openFilenames(QWidget *parent, const QString &caption, const QString &dir,
                                const QString &filter, QString *selectedFilter, QFileDialog::Options options);

    int getAppMenuMetric( const char *metricName, int defaultValue ) const;

    static Ptr_gtk_widget_show gtk_widget_show;
    static Ptr_gtk_widget_set_name gtk_widget_set_name;
    static Ptr_gtk_text_view_new gtk_text_view_new;
    static Ptr_gtk_widget_get_size_request gtk_widget_get_size_request;
    static Ptr_gtk_button_set_label gtk_button_set_label;
    static Ptr_gtk_style_lookup_color gtk_style_lookup_color;
    static Ptr_gtk_hbox_new gtk_hbox_new;
    static Ptr_gtk_box_pack_start gtk_box_pack_start;
    static Ptr_gtk_box_pack_end gtk_box_pack_end;
    static Ptr_gtk_widget_set_size_request gtk_widget_set_size_request;

    static Ptr_hildon_init hildon_init;
    static Ptr_hildon_entry_new hildon_entry_new;
    static Ptr_hildon_number_editor_new hildon_number_editor_new;
    static Ptr_hildon_app_menu_new hildon_app_menu_new;
    static Ptr_hildon_app_menu_add_filter hildon_app_menu_add_filter;
    static Ptr_hildon_edit_toolbar_new_with_text hildon_edit_toolbar_new_with_text;
    static Ptr_hildon_button_new hildon_button_new;
    static Ptr_hildon_check_button_new hildon_check_button_new;
    static Ptr_hildon_pannable_area_new hildon_pannable_area_new;
    static Ptr_hildon_gtk_widget_set_theme_size hildon_gtk_widget_set_theme_size;
    static Ptr_hildon_dialog_new_with_buttons hildon_dialog_new_with_buttons;
    static Ptr_hildon_note_new_information hildon_note_new_information;

    static Ptr_hildon_file_chooser_dialog_new hildon_file_chooser_dialog_new;
    static Ptr_hildon_file_chooser_dialog_set_extension hildon_file_chooser_dialog_set_extension;
    static Ptr_hildon_file_chooser_dialog_add_extensions_combo hildon_file_chooser_dialog_add_extensions_combo;

    static QMap<QAbstractScrollArea *, ScrollBarFader *> scrollBarFaders;
    static int scrollBarFadeDelay;
    static int scrollBarFadeDuration;
    static int scrollBarFadeUpdateInterval;

    static GtkWidget *radioButtonLeft;
    static GtkWidget *radioButtonMiddle;
    static GtkWidget *radioButtonRight;

private:
    virtual GtkWidget* getTextColorWidget() const;

};

class ScrollBarFader : public QObject {
    Q_OBJECT
public:
    ScrollBarFader(QAbstractScrollArea *area, int delay, int duration, int update_interval);
    ~ScrollBarFader();

    void show();

    inline qreal currentAlpha() const  { return m_current_alpha; }

private slots:
    void delayTimeout();
    void fade(qreal);

private:
    QAbstractScrollArea *m_area;
    QTimeLine *m_fade_timeline;
    QTimer *m_delay_timer;
    qreal m_current_alpha;
};

#endif //defined(Q_WS_MAEMO_5)

QT_END_NAMESPACE

QT_END_HEADER

#endif //QMAEMOS5STYLE_P_H
