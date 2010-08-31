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
#include "qdebug.h"
#include "qhildoninputcontext_p.h"
#include "qpointer.h"
#include "qapplication.h"
#include "qclipboard.h"
#include "qplaintextedit.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qtextbrowser.h"
#include "qinputcontext.h"
#include "qtextcodec.h"
#include "qtextboundaryfinder.h"
#include <private/qkeymapper_p.h>

#include "qgraphicsview.h"
#include "qtimer.h"
#ifndef QT_NO_GRAPHICSVIEW
#include "qgraphicsitem.h"
#include "qgraphicsproxywidget.h"
#include "qgraphicsscene.h"
#include "qgraphicswidget.h"
#endif

#ifdef Q_WS_MAEMO_5

#define DEFAULT_LONG_PRESS_TIMEOUT 600

#define GDK_ISO_ENTER  0xfe34
#define COMPOSE_KEY    Qt::Key_Multi_key   // "Ch" key
#define LEVEL_KEY      Qt::Key_AltGr       //"Fn" key

#define STATE_LEVEL_MASK    1 << 7
#define STATE_CONTROL_MASK  1 << 2
#define STATE_SHIFT_MASK    1 << 0

//Keyboard layout levels
#define BASE_LEVEL 0
#define NUMERIC_LEVEL 2
#define LOCKABLE_LEVEL 4


extern bool qt_sendSpontaneousEvent(QObject*, QEvent*); //qapplication_x11.cpp

#ifdef QT_BUILD_INTERNAL
#  define HIM_DEBUG
#endif

#ifdef HIM_DEBUG
static inline bool qHimDebugEnabled()
{
    static const bool debug = !qgetenv("QT_HIM_DEBUG").isEmpty();
    return debug;
}
#  define qHimDebug  if (!qHimDebugEnabled()) {} else qDebug
#else
#  define qHimDebug  while (false) qDebug
#endif

static const char *debugNameForCommunicationId(HildonIMCommunication id)
{
#ifdef HIM_DEBUG
    static const char *const mapping[] = {
        "HILDON_IM_CONTEXT_HANDLE_ENTER",
        "HILDON_IM_CONTEXT_HANDLE_TAB",
        "HILDON_IM_CONTEXT_HANDLE_BACKSPACE",
        "HILDON_IM_CONTEXT_HANDLE_SPACE",
        "HILDON_IM_CONTEXT_CONFIRM_SENTENCE_START",
        "HILDON_IM_CONTEXT_FLUSH_PREEDIT",
        "HILDON_IM_CONTEXT_CANCEL_PREEDIT",
        "HILDON_IM_CONTEXT_BUFFERED_MODE",
        "HILDON_IM_CONTEXT_DIRECT_MODE",
        "HILDON_IM_CONTEXT_REDIRECT_MODE",
        "HILDON_IM_CONTEXT_SURROUNDING_MODE",
        "HILDON_IM_CONTEXT_PREEDIT_MODE",
        "HILDON_IM_CONTEXT_CLIPBOARD_COPY",
        "HILDON_IM_CONTEXT_CLIPBOARD_CUT",
        "HILDON_IM_CONTEXT_CLIPBOARD_PASTE",
        "HILDON_IM_CONTEXT_CLIPBOARD_SELECTION_QUERY",
        "HILDON_IM_CONTEXT_REQUEST_SURROUNDING",
        "HILDON_IM_CONTEXT_REQUEST_SURROUNDING_FULL",
        "HILDON_IM_CONTEXT_WIDGET_CHANGED",
        "HILDON_IM_CONTEXT_OPTION_CHANGED",
        "HILDON_IM_CONTEXT_ENTER_ON_FOCUS",
        "HILDON_IM_CONTEXT_SPACE_AFTER_COMMIT",
        "HILDON_IM_CONTEXT_NO_SPACE_AFTER_COMMIT",
        "HILDON_IM_CONTEXT_SHIFT_LOCKED",
        "HILDON_IM_CONTEXT_SHIFT_UNLOCKED",
        "HILDON_IM_CONTEXT_LEVEL_LOCKED",
        "HILDON_IM_CONTEXT_LEVEL_UNLOCKED",
        "HILDON_IM_CONTEXT_SHIFT_UNSTICKY",
        "HILDON_IM_CONTEXT_LEVEL_UNSTICKY"
    };

    if (unsigned(id) < (sizeof(mapping) / sizeof(mapping[0]))) {
        return mapping[id];
    } else {
        static char name[] = "ID 00";
        name[3] = '0' + (id / 10);
        name[4] = '0' + (id % 10);
        return name;
    }
#endif
    return 0;
}

static QGraphicsObject *qDeclarativeTextEdit_cast(QWidget *w)
{
    if (QGraphicsView *view = qobject_cast<QGraphicsView *>(w)) {
        if (QGraphicsObject *item = qgraphicsitem_cast<QGraphicsObject *>(view->scene()->focusItem())) {
            if (item->inherits("QDeclarativeTextEdit"))
                return item;
        }
    }
    return 0;
}


#define LOGMESSAGE1(x)       qHimDebug() << x;
#define LOGMESSAGE2(x, y)    qHimDebug() << x << "(" << y << ")";
#define LOGMESSAGE3(x, y, z) qHimDebug() << x << "(" << y << " " << z << ")";


QMap<QWidget *, QHIMProxyWidget *> QHIMProxyWidget::proxies;

QHIMProxyWidget::QHIMProxyWidget(QWidget *widget)
    : QWidget(0), w(widget)
{
    setAttribute(Qt::WA_InputMethodEnabled);
    setAttribute(Qt::WA_NativeWindow);
    createWinId();
    connect(w, SIGNAL(destroyed()), this, SLOT(widgetWasDestroyed()));
}

QHIMProxyWidget::~QHIMProxyWidget()
{
}

QWidget *QHIMProxyWidget::widget() const
{
    return w;
}

QHIMProxyWidget *QHIMProxyWidget::proxyFor(QWidget *w)
{
    QHIMProxyWidget *proxy = qobject_cast<QHIMProxyWidget *>(w);

    if (!proxy)
        proxy = proxies.value(w);

    if (!proxy) {
        proxy = new QHIMProxyWidget(w);
        proxies.insert(w, proxy);
    }
    //qWarning() << "Using HIM Proxy widget" << proxy << "for widget" << w << "isnative: " << proxy->testAttribute(Qt::WA_NativeWindow) << " / " << w->testAttribute(Qt::WA_NativeWindow);

    return proxy;
}

void QHIMProxyWidget::widgetWasDestroyed()
{
    proxies.remove(w);
    delete this;
}


/*! XkbLookupKeySym ( X11->display, event->nativeScanCode(), HILDON_IM_SHIFT_STICKY_MASK, &mods_rtrn, sym_rtrn)
 */
static QString translateKeycodeAndState(KeyCode key, uint state, KeySym &keysym){
    uint mods;
    KeySym *ks = &keysym;
    if ( XkbLookupKeySym ( X11->display, key, state, &mods, ks) )
        return QKeyMapperPrivate::maemo5TranslateKeySym(*ks);
    else
        return QString();
}

static Window findHildonIm()
{
    union
    {
        Window *win;
        unsigned char *val;
    } value;

    Window result = 0;
    ulong n = 0;
    ulong extra = 0;
    int format = 0;
    Atom realType;

    int status = XGetWindowProperty(X11->display, QX11Info::appRootWindow(),
                    ATOM(_HILDON_IM_WINDOW), 0L, 1L, 0,
                    XA_WINDOW, &realType, &format,
                    &n, &extra, (unsigned char **) &value.val);

    if (status == Success && realType == XA_WINDOW
          && format == HILDON_IM_WINDOW_ID_FORMAT && n == 1 && value.win != 0) {
        result = value.win[0];
        XFree(value.val);
    } else {
        qWarning("QHildonInputContext: Unable to get the Hildon IM window id");
    }

    return result;
}



/*! Send a key event to the IM, which makes it available to the plugins
 */
static void sendKeyEvent(QWidget *widget, QEvent::Type type, uint state, uint keyval, quint16 keycode)
{
    int gdkEventType;
    Window w = findHildonIm();

    if (!w)
        return;

    //Translate QEvent::Type in GDK_Event
    switch (type){
        case QEvent::KeyPress:
            gdkEventType = 8;
        break;
        case QEvent::KeyRelease:
            gdkEventType = 9;
        break;
        default:
            qWarning("QHildonInputContext: Event type not allowed");
            return;
    }

    XEvent ev;
    memset(&ev, 0, sizeof(XEvent));

    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = ATOM(_HILDON_IM_KEY_EVENT);
    ev.xclient.format = HILDON_IM_KEY_EVENT_FORMAT;

    HildonIMKeyEventMessage *msg = reinterpret_cast<HildonIMKeyEventMessage *>(&ev.xclient.data);
    msg->input_window = QHIMProxyWidget::proxyFor(widget)->winId();

    msg->type = gdkEventType;
    msg->state = state;
    msg->keyval = keyval;
    msg->hardware_keycode = keycode;

    XSendEvent(X11->display, w, false, 0, &ev);
    XSync( X11->display, false );
}

static void deadKeyToUnicodeCombiningChar(int qtkeycode, QChar &combiningChar, QChar &plainChar)
{
    plainChar = combiningChar = QChar();

    switch (qtkeycode) {
        case Qt::Key_Dead_Grave:            plainChar = QChar(0x0060); combiningChar = QChar(0x0300); break;
        case Qt::Key_Dead_Acute:            plainChar = QChar(0x00b4); combiningChar = QChar(0x0301); break;
        case Qt::Key_Dead_Circumflex:       plainChar = QChar(0x005e); combiningChar = QChar(0x0302); break;
        case Qt::Key_Dead_Tilde:            plainChar = QChar(0x007e); combiningChar = QChar(0x0303); break;
        case Qt::Key_Dead_Macron:           plainChar = QChar(0x00af); combiningChar = QChar(0x0304); break;
        case Qt::Key_Dead_Breve:            plainChar = QChar(0x02d8); combiningChar = QChar(0x0306); break;
        case Qt::Key_Dead_Abovedot:         plainChar = QChar(0x02d9); combiningChar = QChar(0x0307); break;
        case Qt::Key_Dead_Diaeresis:        plainChar = QChar(0x00a8); combiningChar = QChar(0x0308); break;
        case Qt::Key_Dead_Abovering:        plainChar = QChar(0x00b0); combiningChar = QChar(0x030a); break;
        case Qt::Key_Dead_Doubleacute:      plainChar = QChar(0x0022); combiningChar = QChar(0x030b); break;
        case Qt::Key_Dead_Caron:            plainChar = QChar(0x02c7); combiningChar = QChar(0x030c); break;
        case Qt::Key_Dead_Cedilla:          plainChar = QChar(0x00b8); combiningChar = QChar(0x0327); break;
        case Qt::Key_Dead_Ogonek:           plainChar = QChar(0x02db); combiningChar = QChar(0x0328); break;
        case Qt::Key_Dead_Iota:             break; // Cannot be combined
        case Qt::Key_Dead_Voiced_Sound:     break; // Cannot be combined
        case Qt::Key_Dead_Semivoiced_Sound: break; // Cannot be combined
        case Qt::Key_Dead_Belowdot:         plainChar = QChar(0x02d4); combiningChar = QChar(0x0323); break;
        case Qt::Key_Dead_Hook:             plainChar = QChar(0x02c0); combiningChar = QChar(0x0309); break;
        case Qt::Key_Dead_Horn:             combiningChar = QChar(0x031b); break; // no plain char
        default:                            break; // unknown dead key
    }
}

/*! Sends the key as a spontaneous event.
 */
static void sendKey(QWidget *keywidget, int qtCode)
{
    QPointer<QWidget> guard = keywidget;

    KeySym keysym = NoSymbol;
    int keycode;

    switch (qtCode) {
    case Qt::Key_Enter:
        keycode = 36;
        break;
    case Qt::Key_Tab:
        keycode = 66;
        break;
    case Qt::Key_Backspace:
        keycode = 22;
        break;
    case Qt::Key_Left:
        keycode = 116;
        break;
    case Qt::Key_Right:
        keycode = 114;
        break;
    default:
        qHimDebug("HIM: sendKey() keycode %d not allowed", qtCode);
        return;
    }

    keysym = XKeycodeToKeysym(X11->display, keycode, 0);

    QKeyEventEx click(QEvent::KeyPress, qtCode, Qt::NoModifier , QString(), false, 1, keycode, keysym, 0);
    qt_sendSpontaneousEvent(keywidget, &click);

    // in case the widget was destroyed when the key went down
    if (guard.isNull())
        return;

    QKeyEventEx release(QEvent::KeyRelease, qtCode, Qt::NoModifier , QString(), false, 1, keycode, keysym, 0);
    qt_sendSpontaneousEvent(keywidget, &release);
}

/*!
 */
static void answerClipboardSelectionQuery(QWidget *widget)
{
    bool hasSelection = !widget->inputMethodQuery(Qt::ImCurrentSelection).toString().isEmpty();

    XEvent xev;
    Window w = findHildonIm();

    memset(&xev, 0, sizeof(xev));
    xev.xclient.type = ClientMessage;
    xev.xclient.window = w;
    xev.xclient.message_type = ATOM(_HILDON_IM_CLIPBOARD_SELECTION_REPLY);
    xev.xclient.format = HILDON_IM_CLIPBOARD_SELECTION_REPLY_FORMAT;
    xev.xclient.data.l[0] = hasSelection;

    XSendEvent(X11->display, w, false, 0, &xev);
}


KeySym getKeySymForLevel(int keycode, int level ){
    XkbDescPtr xkbDesc = XkbGetMap(X11->display, XkbAllClientInfoMask, XkbUseCoreKbd);
    if (!xkbDesc)
        return NoSymbol;

    KeySym keySym = XkbKeySymEntry(xkbDesc, keycode, level, 0);

    //Check for a not repated keysym
    KeySym keySymTest = XkbKeySymEntry(xkbDesc, keycode, 0, 1);
    if (keySym == keySymTest)
        return NoSymbol;

    return keySym;
}

QHildonInputContext::QHildonInputContext(QObject* parent)
    : QInputContext(parent),
      mask(0), options(0),
      triggerMode(HILDON_IM_TRIGGER_NONE),
      commitMode(HILDON_IM_COMMIT_REDIRECT),
      lastCommitMode(HILDON_IM_COMMIT_REDIRECT),
      inputMode(HILDON_GTK_INPUT_MODE_FULL),
      textCursorPosOnPress(0), autoUpper(false),
      lastInternalChange(false), spaceAfterCommit(false),
      lastKeyWidget(0), lastQtKeyCode(Qt::Key_unknown)
{
    longPressTimer = new QTimer(this);
    longPressTimer->setInterval(DEFAULT_LONG_PRESS_TIMEOUT);
    longPressTimer->setSingleShot(true);
    connect(longPressTimer, SIGNAL(timeout()), this, SLOT(longPressDetected()));
}

QHildonInputContext::~QHildonInputContext()
{
    sendHildonCommand(HILDON_IM_HIDE);
}

QString QHildonInputContext::identifierName()
{
    return QLatin1String("hildon");
}

QString QHildonInputContext::language()
{
    //TODO GConf /apps/osso/inputmethod/hildon-im-languages
    return QString();
}

/*! \internal
 *  Resolves the focus for a widget inside a QGraphicsProxyWidget.
 *  Returns that widget (holding the focus) or 0 otherwise.
 */
QWidget *resolveFocusWidget(QWidget *testw)
{
    QWidget *w = testw;

#ifndef QT_NO_GRAPHICSVIEW
    while (QGraphicsView *view = qobject_cast<QGraphicsView *>(w)) {

        QGraphicsScene *scene = view->scene();
        if (scene) {
            QGraphicsItem *item = scene->focusItem();
            QGraphicsProxyWidget *proxy = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
            if (proxy && proxy->widget() && proxy->widget()->focusWidget() ) {
                w = proxy->widget()->focusWidget();
            } else {
                break;
            }
        } else {
            break;
        }
    }
#endif

    return (w == testw) ? 0 : w;
}

QWidget *QHildonInputContext::focusWidget() const
{
    return QInputContext::focusWidget() ? QInputContext::focusWidget() : lastFocus.data();
}

/*!\internal
reset the UI state
 */
void QHildonInputContext::reset()
{
    qHimDebug() << "HIM: reset()";

    if (QInputContext::focusWidget())
        sendHildonCommand(HILDON_IM_CLEAR, QInputContext::focusWidget());

    cancelPreedit();
    longPressKeyEvent.reset(0);
    longPressTimer->stop();
    updateInputMethodHints();

    //Reset internals
    mask = 0;
    lastInternalChange = false;
}

bool QHildonInputContext::isComposing() const
{
    return false;
}

void QHildonInputContext::setFocusWidget(QWidget *w)
{
    // As soon as the virtual keyboard is mapped by the X11 server,
    // it is also activated, which essentially steals our focus.
    // The same happens for the symbol picker.
    // This is a bug in the HIM, that we try to work around here.
    lastFocus = QInputContext::focusWidget();

    // Another work around for the GraphicsView.
    // In case of a Widget inside a GraphicsViewProxyWidget we need to remember
    // that it had the focus
    realFocus = resolveFocusWidget(w);

    QInputContext::setFocusWidget(w);

    updateInputMethodHints();
    if (w)
        sendHildonCommand(HILDON_IM_SETCLIENT, w);

    qHimDebug() << "HIM: setFocusWidget: " << w << " (real: " << realFocus << " / last: " << lastFocus << ")";
}

void QHildonInputContext::updateInputMethodHints()
{
    if (QInputContext::focusWidget()) {
        Qt::InputMethodHints hints = QInputContext::focusWidget()->inputMethodHints();

        // restrictions
        if ((hints & Qt::ImhExclusiveInputMask) == Qt::ImhDialableCharactersOnly) {
            inputMode = HILDON_GTK_INPUT_MODE_TELE;
        } else if (((hints & Qt::ImhExclusiveInputMask) == (Qt::ImhDigitsOnly | Qt::ImhUppercaseOnly)) ||
                   ((hints & Qt::ImhExclusiveInputMask) == (Qt::ImhDigitsOnly | Qt::ImhLowercaseOnly))) {
            inputMode = HILDON_GTK_INPUT_MODE_ALPHA;
        } else if ((hints & Qt::ImhExclusiveInputMask) == Qt::ImhDigitsOnly) {
            inputMode = HILDON_GTK_INPUT_MODE_NUMERIC;
        } else if (((hints & Qt::ImhExclusiveInputMask) == Qt::ImhFormattedNumbersOnly) ||
                   ((hints & Qt::ImhExclusiveInputMask) == (Qt::ImhFormattedNumbersOnly | Qt::ImhDigitsOnly))) {
            inputMode = HILDON_GTK_INPUT_MODE_NUMERIC | HILDON_GTK_INPUT_MODE_SPECIAL;
        } else {
            inputMode = HILDON_GTK_INPUT_MODE_FULL;
        }

        bool isAutoCapable = (hints & (Qt::ImhExclusiveInputMask |
                                       Qt::ImhNoAutoUppercase)) == 0;
        bool isPredictive = (hints & (Qt::ImhDigitsOnly |
                                      Qt::ImhFormattedNumbersOnly |
                                      Qt::ImhUppercaseOnly |
                                      Qt::ImhLowercaseOnly |
                                      Qt::ImhDialableCharactersOnly |
                                      Qt::ImhNoPredictiveText)) == 0;

        // behavior flags
        if (hints & Qt::ImhHiddenText) {
            inputMode |= HILDON_GTK_INPUT_MODE_INVISIBLE;
        } else {
            // no auto upper case or predictive text for passwords
            if (isAutoCapable)
                inputMode |= HILDON_GTK_INPUT_MODE_AUTOCAP;
            if (isPredictive)
                inputMode |= HILDON_GTK_INPUT_MODE_DICTIONARY;
        }

        // multi-line support
        // TODO: this really needs to fixed in Qt
        QWidget *testmulti = realFocus ? realFocus.data() : QInputContext::focusWidget();

        if (qobject_cast<QTextEdit *>(testmulti)
                || qobject_cast<QPlainTextEdit *>(testmulti)
                || qDeclarativeTextEdit_cast(testmulti)) {
            inputMode |= HILDON_GTK_INPUT_MODE_MULTILINE;
        }

        qHimDebug("HIM: Mapped hint: 0x%x to mode: 0x%x", int(hints), int(inputMode));
    } else {
        inputMode = 0;
    }
}

bool QHildonInputContext::filterEvent(const QEvent *event)
{
    QWidget *w = QInputContext::focusWidget();
    if (!w)
        return false;

    switch (event->type()){
    case QEvent::RequestSoftwareInputPanel:{
        //On the device, these events are sent at the same time of the TabletRelease ones
        triggerMode = HILDON_IM_TRIGGER_FINGER;

        // workaround for a very weird interaction between QLineEdit (which
        // changes its internal editing:yes/no state on focus out) and the
        // Hildon fullscreen keyboard, whiccheckh steals the focus from the
        // application (NOT when it shows up, but as only soon as the user
        // clicks on any button within the keyboard)
        if (QLineEdit *le = qobject_cast<QLineEdit *>(w)) {
            if (le->echoMode() == QLineEdit::PasswordEchoOnEdit)
                le->clear();
        }

        QTimer::singleShot(0, this, SLOT(showSoftKeyboard()));
        return true;
    }
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        triggerMode = HILDON_IM_TRIGGER_KEYBOARD;
        return filterKey(w, static_cast<const QKeyEvent *>(event), false);

    default:
        break;
    }
    return QInputContext::filterEvent(event);
}

void QHildonInputContext::showSoftKeyboard()
{
    // Hacky fix for the misbehaving QGraphicsProxyWidget:
    // we do not get any notification if the input focus changes to another
    // widget inside a single QGraphicsProxyWidget
    QWidget *newFocus = resolveFocusWidget(QInputContext::focusWidget());
    if (realFocus && (newFocus != realFocus)) {
        cancelPreedit();
        longPressKeyEvent.reset(0);
        longPressTimer->stop();
        realFocus = newFocus;
        updateInputMethodHints();
    }
    sendHildonCommand(HILDON_IM_SETNSHOW, QInputContext::focusWidget());
}

void QHildonInputContext::update()
{
    qHimDebug() << "HIM: update(): lastInternalChange =" << lastInternalChange;
    updateInputMethodHints();
    sendInputMode();

    if (lastInternalChange) {
        //Autocase update
        checkSentenceStart();
        lastInternalChange = false;
    }
}

// an evil hack to quickly make a KeyRelease from a KeyPress event
class MyKeyEventEx : public QKeyEventEx {
public:
    void release()
    {
        t = QEvent::KeyRelease;
    }
};

void QHildonInputContext::longPressDetected()
{
    qHimDebug("HIM: longPressDetected");

    if (longPressKeyEvent.isNull() || !lastKeyWidget)
        return;

    int oldMask = mask;

    if (mask & HILDON_IM_LEVEL_LOCK_MASK)
        mask &= ~(HILDON_IM_LEVEL_LOCK_MASK | HILDON_IM_LEVEL_STICKY_MASK);
    else
        mask |= HILDON_IM_LEVEL_STICKY_MASK;

    cancelPreedit();

    MyKeyEventEx *ke = static_cast<MyKeyEventEx *>(longPressKeyEvent.data());
    filterKey(lastKeyWidget, ke, true);
    ke->release();
    filterKey(lastKeyWidget, ke, true);

    qHimDebug() << "HIM: lastCommitString.lenght() == " << lastCommitString.length();
    if (!lastCommitString.isEmpty()) {
        qHimDebug() << "HIM: sending Left/Backspace/Right";
        sendKey(lastKeyWidget, Qt::Key_Left);
        sendKey(lastKeyWidget, Qt::Key_Backspace);
        sendKey(lastKeyWidget, Qt::Key_Right);
    }

    longPressKeyEvent.reset(0);
    mask = oldMask;
}

/*! \internal
 * Filters spontaneous keyevents then elaborates them and updates the Hildon Main UI
 * via XMessages. In some cases it creates and posts a new keyevent
 * as no spontaneous event.
 */
bool QHildonInputContext::filterKey(QWidget *keywidget, const QKeyEvent *event, bool isLongPress)
{
    // Ignore non-extended events, since we can't handle those below.
    if (!event->hasExtendedInfo())
        return false;

    const quint32 state = event->nativeModifiers();
    const quint32 keycode = event->nativeScanCode();
    KeySym keysym = static_cast<KeySym>(event->nativeVirtualKey());
    const int qtkeycode = event->key();
    Qt::InputMethodHints hints = QInputContext::focusWidget()->inputMethodHints();
    Qt::KeyboardModifiers modifiers = event->modifiers();
    bool isAutoRepeat = false;
    QString commitString;

    if (event->type() == QEvent::KeyPress)
        lastCommitString.clear();

    // All QKeyEvents sent to QInputContext::filterEvent() are always
    // non-auto-repeated (even though the REAL QKeyEvents that get sent to
    // the widgets afterwards may have the auto-repeat flag correctly set). 
    // The workaround is to check if a X11 Press event for the same key code
    // is already queued when a Release is received.  (Qt has a much better
    // version of this algorithm in qkeymapper_x11.cpp, but this code here
    // is sufficient for the N900's X11 server)
    if (!isLongPress) {
        static quint32 lastRepeatedKeycode = 0;

        if (event->type() == QEvent::KeyRelease) {
            XEvent xevent;
            lastRepeatedKeycode = 0;

            if (XCheckTypedWindowEvent(X11->display, keywidget->effectiveWinId(), XKeyPress, &xevent)) {
                if (xevent.xkey.keycode == keycode) {
                    isAutoRepeat = true;
                    lastRepeatedKeycode = keycode;
                }
                XPutBackEvent(X11->display, &xevent);
            }
        } else if (event->type() == QEvent::KeyPress) {
            if (lastRepeatedKeycode && (lastRepeatedKeycode == keycode))
                isAutoRepeat = true;
        }
    }
    qHimDebug("HIM: filterKey (%s) Auto: %d, Mask: %x state: %x options: %x keycode: %d keysym: %x QtKey: %x Text: \"%s\" (%d)",
              (event->type() == QEvent::KeyPress ? "press  " : "release"), isAutoRepeat, mask, state, options,
              keycode, (quint32) keysym, qtkeycode, qPrintable(event->text()), event->text().length());

    // Reset static vars when the widget changes
    if (keywidget != lastKeyWidget){
        mask = 0;
        lastKeyWidget = keywidget;
        lastQtKeyCode = 0;
        combiningChar = plainCombiningChar = QChar();
    }

    if (!isAutoRepeat && !isLongPress) {
        longPressTimer->stop();
        longPressKeyEvent.reset(0);
    }

    // Drop auto repeated keys for COMPOSE_KEY
    if (qtkeycode == COMPOSE_KEY && isAutoRepeat)
        return true;
    if (!qtkeycode)
        return true;

    //1. A dead key will not be immediately commited, but combined with the next key
    if ((qtkeycode >= Qt::Key_Dead_Grave && qtkeycode <= Qt::Key_Dead_Horn) && (event->type() == QEvent::KeyPress))
        mask |= HILDON_IM_DEAD_KEY_MASK;
    else
        mask &= ~HILDON_IM_DEAD_KEY_MASK;

    if (mask & HILDON_IM_DEAD_KEY_MASK && combiningChar.isNull())
    {
        deadKeyToUnicodeCombiningChar(qtkeycode, combiningChar, plainCombiningChar);
        return true;
    }

    /*2. Pressing any key while the compose key is pressed will keep that
     *   character from being directly submitted to the application. This
     *   allows the IM process to override the interpretation of the key
     */
    if (qtkeycode == COMPOSE_KEY)
    {
        if (event->type() == QEvent::KeyPress)
            mask |= HILDON_IM_COMPOSE_MASK;
        else
            mask &= ~HILDON_IM_COMPOSE_MASK;
    }

    // 3 Sticky and locking keys initialization
    if (event->type() == QEvent::KeyRelease)
    {
        if (qtkeycode == LEVEL_KEY){
            setMaskState(&mask,
                         HILDON_IM_LEVEL_LOCK_MASK,
                         HILDON_IM_LEVEL_STICKY_MASK,
                         lastQtKeyCode == LEVEL_KEY);
        }
        else if (qtkeycode == Qt::Key_Shift ){
            setMaskState(&mask,
                         HILDON_IM_SHIFT_LOCK_MASK,
                         HILDON_IM_SHIFT_STICKY_MASK,
                         lastQtKeyCode == Qt::Key_Shift);
        }
    }

    lastQtKeyCode = qtkeycode;

    if (qtkeycode == Qt::Key_Tab){
        commitString = QLatin1String("\t");
    }

    // Invert the level key when in tele or special mode
    bool invertLevelKey = false;
    if ((inputMode & HILDON_GTK_INPUT_MODE_FULL) != HILDON_GTK_INPUT_MODE_FULL)
        invertLevelKey = (inputMode & HILDON_GTK_INPUT_MODE_ALPHA) == 0  &&
                         (inputMode & HILDON_GTK_INPUT_MODE_HEXA)  == 0  &&
                         ((inputMode & HILDON_GTK_INPUT_MODE_TELE) ||
                         (inputMode & HILDON_GTK_INPUT_MODE_SPECIAL));
    bool isShifted = (mask & (HILDON_IM_SHIFT_STICKY_MASK | HILDON_IM_SHIFT_LOCK_MASK)) || (state & STATE_SHIFT_MASK);
    bool isLeveled = (mask & (HILDON_IM_LEVEL_STICKY_MASK | HILDON_IM_LEVEL_LOCK_MASK)) || (state & STATE_LEVEL_MASK);

    /* 5. When the level key is in sticky or locked state, translate the
     *    keyboard state as if that level key was being held down.
     */
    /* If the input mode is strictly numeric and the digits are level
     *  shifted on the layout, it's not necessary for the level key to
     *  be pressed at all.
     */
    if (invertLevelKey || ((options & HILDON_IM_AUTOLEVEL_NUMERIC) &&
        ((inputMode & HILDON_GTK_INPUT_MODE_FULL) == HILDON_GTK_INPUT_MODE_NUMERIC))) {

        /* the level key is inverted
        when level or shift key is pressed use normal level
        otherwise use numeric level*/
        if (!isLeveled) {
            KeySym ks = getKeySymForLevel(keycode, NUMERIC_LEVEL);
            QString string = QKeyMapperPrivate::maemo5TranslateKeySym(ks);
            if (!string.isEmpty()) {
                keysym = ks;
                commitString = string;
            }
        } else {
            KeySym ks = getKeySymForLevel(keycode, BASE_LEVEL);
            QString string = QKeyMapperPrivate::maemo5TranslateKeySym(ks);
            if (!string.isEmpty()) {
                keysym = ks;
                commitString = string;
            }
        }
    }
    /* The input is forced to a predetermined level
     */
    else if (isLeveled) {
        commitString = translateKeycodeAndState(keycode, STATE_LEVEL_MASK, keysym);
    }
    else if (options & HILDON_IM_LOCK_LEVEL)
    {
        KeySym ks = getKeySymForLevel(keycode, LOCKABLE_LEVEL);
        QString string = QKeyMapperPrivate::maemo5TranslateKeySym(ks);

        qHimDebug("HIM: LOCK_LEVEL: mapped to KeySym: %x Text (%d): \"%s\" (%04x)", int(ks), string.length(), string.toUtf8().constData(), string.length() ? string[0].unicode() : 0xffff);

        if (ks && !string.isEmpty() && string.at(0).unicode()) {
            KeySym lower = NoSymbol;
            KeySym upper = NoSymbol;
            XConvertCase(ks, &lower, &upper);

            if (string.at(0).isPrint()) {
                if (isShifted) {
                    commitString = string.toUpper();
                    keysym = upper;
                } else {
                    commitString = string.toLower();
                    keysym = lower;
                }
            }
        }
    }
    // 6. Shift lock or holding the shift down forces uppercase, ignoring autocap
    else if (isShifted) {
        commitString = translateKeycodeAndState(keycode, STATE_SHIFT_MASK, keysym);
    }

    /* Hardware keyboard autocapitalization  */
    if (autoUpper && (inputMode & HILDON_GTK_INPUT_MODE_AUTOCAP))
    {
        qHimDebug() << "HIM: Auto-cap";
        QChar currentChar;
        KeySym lower = NoSymbol;
        KeySym upper = NoSymbol;

        if (commitString.isEmpty()) {
            QString ks = QKeyMapperPrivate::maemo5TranslateKeySym(keysym);
            if (!ks.isEmpty())
                currentChar = ks.at(0);
        } else {
            currentChar = commitString.at(0);
        }

        XConvertCase(keysym, &lower, &upper);

        if (currentChar.isPrint()) {
            if (state & STATE_SHIFT_MASK) {
                currentChar = currentChar.toLower();
                keysym = lower;
            } else {
                currentChar = currentChar.toUpper();
                keysym = upper;
            }
            commitString = QString(currentChar); //sent to the widget
        }
    }
    //F. word completion manipulation (for fremantle)
    if (event->type() == QEvent::KeyPress &&
        lastCommitMode == HILDON_IM_COMMIT_PREEDIT &&
        !preEditBuffer.isNull())
    {
        switch (qtkeycode){
            case Qt::Key_Right:{
                commitPreeditBuffer();
                return true;
            }
            case Qt::Key_Backspace:
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:{
                cancelPreedit();
                return true;
           }

           case Qt::Key_Return:
           case Qt::Key_Enter: {
                cancelPreedit();
                break;
           }
           default: {
               if (keysym == GDK_ISO_ENTER)
                   cancelPreedit();
               break;
           }
        }
    }

    //7. Sticky and lock state reset
    if (event->type() == QEvent::KeyRelease)
    {
        if (qtkeycode != Qt::Key_Shift )
        {
            /* If not locked, pressing any character resets shift state */
            if ((mask & HILDON_IM_SHIFT_LOCK_MASK) == 0)
            {
                mask &= ~HILDON_IM_SHIFT_STICKY_MASK;
            }
        }
        if (qtkeycode != LEVEL_KEY)
        {
            /* If not locked, pressing any character resets level state */
            if ((mask & HILDON_IM_LEVEL_LOCK_MASK) == 0)
            {
                mask &= ~HILDON_IM_LEVEL_STICKY_MASK;
            }
        }
    }

    if (event->type() == QEvent::KeyRelease || state & STATE_CONTROL_MASK)
    {
        //Prevent Symbol Picker for following hints
        if ((hints & (Qt::ImhFormattedNumbersOnly | Qt::ImhDigitsOnly | Qt::ImhDialableCharactersOnly)))
            return false;
        sendKeyEvent(keywidget, event->type(), state, keysym, keycode);
        return false;
    }


    /* 8. Pressing a dead key twice, or if followed by a space, inputs
     *    the dead key's character representation
     */
    if ((mask & HILDON_IM_DEAD_KEY_MASK || qtkeycode == Qt::Key_Space) && !combiningChar.isNull())
    {
        QChar thisChar, dummy;
        deadKeyToUnicodeCombiningChar(qtkeycode, thisChar, dummy);
        if ((thisChar == combiningChar) || qtkeycode == Qt::Key_Space)
            commitString = QString(plainCombiningChar);
        else
            commitString = QString::fromUtf8(XKeysymToString(keysym));
        combiningChar = plainCombiningChar = QChar();
    } else {
        /* Regular keypress */
        if (mask & HILDON_IM_COMPOSE_MASK) {
            sendKeyEvent(keywidget, event->type(),state, keysym, keycode);
            return true;
        } else if (commitString.isEmpty() && qtkeycode != Qt::Key_Backspace) {
            commitString = event->text();
        }
    }

    /* Control keys should not produce commitString, if they do it's a bug
       on keymap side and we have to workaround this here */
    if (qtkeycode == Qt::Key_Return || qtkeycode == Qt::Key_Enter ||
        keysym == GDK_ISO_ENTER || qtkeycode == Qt::Key_Backspace) {
        commitString = QString();
        lastInternalChange = true;
    } else if (qtkeycode == Qt::Key_Shift || qtkeycode == Qt::Key_AltGr ||
               qtkeycode == Qt::Key_Control) {
        commitString = QString();
    }

    // Shift-Backspace is translated to Shift-Delete, which does not work...
    if (qtkeycode == Qt::Key_Delete && (modifiers & Qt::ShiftModifier)) {
        modifiers &= ~Qt::ShiftModifier;
        commitString.clear();
    }

    // sanity check
    if (commitString.length() == 1 && commitString.at(0) == QChar(0))
        commitString.clear();

    // check for input mode restrictions
    if (!commitString.isEmpty() && (hints & Qt::ImhExclusiveInputMask)) {
        for (int i = 0; i < commitString.length(); ++i) {
            QChar c = commitString.at(i);
            bool ok = false;

            if (hints & Qt::ImhDigitsOnly)
                ok |= c.isDigit() || c == QLatin1Char('-');
            if (hints & Qt::ImhFormattedNumbersOnly)
                ok |= c.isDigit() || QString::fromLatin1("-.,").contains(c, Qt::CaseSensitive);
            if (hints & Qt::ImhUppercaseOnly)
                ok |= c.isLetter() && c.isUpper();
            if (hints & Qt::ImhLowercaseOnly)
                ok |= c.isLetter() && c.isLower();
            if (hints & Qt::ImhDialableCharactersOnly)
                ok |= c.isDigit() || QString::fromLatin1("#*+pP").contains(c, Qt::CaseSensitive);
            if (hints & Qt::ImhEmailCharactersOnly)
                ok = c.isPrint();
            if (hints & Qt::ImhUrlCharactersOnly)
                ok = c.isPrint();

            if (!ok) {
                cancelPreedit();
                return true;
            }
        }
    }

    if (!commitString.isEmpty()) {
        //entering a new character cleans the preedit buffer
        cancelPreedit();

        /* Pressing a dead key followed by a regular key combines to form
         * an accented character
         */
        if (!combiningChar.isNull()) {
            commitString.append(combiningChar);//This will be sent to the widget
            commitString = commitString.normalized(QString::NormalizationForm_C);
            keysym = XStringToKeysym(qPrintable(commitString)); //This will be sent to the IM
        }

        if (!commitString.isEmpty() && isAutoRepeat)
            return true;

        // Create the new event with the elaborate information,
        // then it adds the event to the events queue

        qHimDebug() << "HIM: Commiting: \"" << qPrintable(commitString) << "\" (" << commitString.length() << ") ... first: " << (commitString.isEmpty() ? 0 : commitString.at(0).unicode());
        lastCommitString = commitString;

        QKeyEventEx *ke = new QKeyEventEx(event->type(), qtkeycode, modifiers, commitString, false, commitString.size(), keycode, keysym, state);
        if (isLongPress) {
            QCoreApplication::sendEvent(keywidget, ke);
            delete ke;
        } else {
            QCoreApplication::postEvent(keywidget, ke);
        }

        // start long-press timer
        if ((event->type() == QEvent::KeyPress) && qtkeycode && !isLongPress) {
            qHimDebug() << "HIM: starting long-press timer";
            longPressKeyEvent.reset(new QKeyEventEx(event->type(), event->key(), modifiers, event->text(), false, event->text().length(), keycode, event->nativeVirtualKey(), state));
            longPressTimer->start();
        }

        //Send the new keysym
        sendKeyEvent(keywidget, event->type(), state, keysym, keycode);

        /* Non-printable characters invalidate any previous dead keys */
        if (qtkeycode != Qt::Key_Shift)
            combiningChar = plainCombiningChar = QChar();

        lastInternalChange = true;
        return true;
    } else {
        //Send the new keysym
        sendKeyEvent(keywidget, event->type(), state, keysym, keycode);
        return false;
    }
}

void QHildonInputContext::setCommitMode(HildonIMCommitMode mode, bool clearPreEdit)
{
    if (commitMode != mode) {
        if (clearPreEdit)
            preEditBuffer.clear();
        lastCommitMode = commitMode;
    }
    commitMode = mode;
}



/*! \internal
Filters the XClientMessages sent by QApplication_x11
 */
bool QHildonInputContext::x11FilterEvent(QWidget *keywidget, XEvent *event)
{
    if (QHIMProxyWidget *proxy = qobject_cast<QHIMProxyWidget *>(keywidget))
        keywidget = proxy->widget();

    if (event->xclient.message_type == ATOM(_HILDON_IM_INSERT_UTF8) &&
        event->xclient.format == HILDON_IM_INSERT_UTF8_FORMAT) {
        qHimDebug() << "HIM: x11FilterEvent( HILDON_IM_INSERT_UTF8_FORMAT )";

        HildonIMInsertUtf8Message *msg = reinterpret_cast<HildonIMInsertUtf8Message *>(&event->xclient.data);
        insertUtf8(msg->msg_flag, QString::fromUtf8(msg->utf8_str));
        return true;
    } else if (event->xclient.message_type == ATOM(_HILDON_IM_COM)) {
        HildonIMComMessage *msg = (HildonIMComMessage *)&event->xclient.data;
        options = msg->options;

        qHimDebug() << "HIM: x11FilterEvent( _HILDON_IM_COM /" << debugNameForCommunicationId(msg->type) << ")";

        switch (msg->type) {
        //Handle Keys msgs
        case HILDON_IM_CONTEXT_HANDLE_ENTER:
            sendKey(keywidget, Qt::Key_Enter);
            return true;
        case HILDON_IM_CONTEXT_HANDLE_TAB:
            sendKey(keywidget, Qt::Key_Tab);
            return true;
        case HILDON_IM_CONTEXT_HANDLE_BACKSPACE:
            sendKey(keywidget, Qt::Key_Backspace);
            return true;
        case HILDON_IM_CONTEXT_HANDLE_SPACE:
            insertUtf8(HILDON_IM_MSG_CONTINUE, QChar(Qt::Key_Space));
            commitPreeditBuffer();
            return true;

        //Handle Clipboard msgs
        case HILDON_IM_CONTEXT_CLIPBOARD_SELECTION_QUERY:
            answerClipboardSelectionQuery(keywidget);
            return true;
        case HILDON_IM_CONTEXT_CLIPBOARD_PASTE:
            if (QClipboard *clipboard = QApplication::clipboard()) {
                QInputMethodEvent e;
                e.setCommitString(clipboard->text());
                QApplication::sendEvent(keywidget, &e);
            }
            return true;
        case HILDON_IM_CONTEXT_CLIPBOARD_COPY:
            if (QClipboard *clipboard = QApplication::clipboard())
                clipboard->setText(keywidget->inputMethodQuery(Qt::ImCurrentSelection).toString());
            return true;
        case HILDON_IM_CONTEXT_CLIPBOARD_CUT:
            if (QClipboard *clipboard = QApplication::clipboard()) {
                clipboard->setText(keywidget->inputMethodQuery(Qt::ImCurrentSelection).toString());
                QInputMethodEvent ev;
                QApplication::sendEvent(keywidget, &ev);
            }
            return true;

        //Handle commit mode msgs
        case HILDON_IM_CONTEXT_DIRECT_MODE:
            setCommitMode(HILDON_IM_COMMIT_DIRECT);
            return true;
        case HILDON_IM_CONTEXT_BUFFERED_MODE:
            setCommitMode(HILDON_IM_COMMIT_BUFFERED);
            return true;
        case HILDON_IM_CONTEXT_REDIRECT_MODE:
            setCommitMode(HILDON_IM_COMMIT_REDIRECT);
            clearSelection();
            return true;
        case HILDON_IM_CONTEXT_SURROUNDING_MODE:
            setCommitMode(HILDON_IM_COMMIT_SURROUNDING);
            return true;
        case HILDON_IM_CONTEXT_PREEDIT_MODE:
            setCommitMode(HILDON_IM_COMMIT_PREEDIT);
            return true;

        //Handle context
        case HILDON_IM_CONTEXT_CONFIRM_SENTENCE_START:
            checkSentenceStart();
            return true;
        case HILDON_IM_CONTEXT_FLUSH_PREEDIT:
            commitPreeditBuffer();
            return true;
        case HILDON_IM_CONTEXT_REQUEST_SURROUNDING:
            sendSurrounding(false);
            return true;
        case HILDON_IM_CONTEXT_LEVEL_UNSTICKY:
            mask &= ~(HILDON_IM_LEVEL_STICKY_MASK | HILDON_IM_LEVEL_LOCK_MASK);
            return true;
        case HILDON_IM_CONTEXT_SHIFT_UNSTICKY:
            mask &= ~(HILDON_IM_SHIFT_STICKY_MASK | HILDON_IM_SHIFT_LOCK_MASK);
            return true;
        case HILDON_IM_CONTEXT_CANCEL_PREEDIT:
            cancelPreedit();
            return true;
        case HILDON_IM_CONTEXT_REQUEST_SURROUNDING_FULL:
            sendSurrounding(true);
            return true;
        case HILDON_IM_CONTEXT_SPACE_AFTER_COMMIT:
        case HILDON_IM_CONTEXT_NO_SPACE_AFTER_COMMIT:
            spaceAfterCommit = (msg->type == HILDON_IM_CONTEXT_SPACE_AFTER_COMMIT);
            return true;

        case HILDON_IM_CONTEXT_WIDGET_CHANGED:
        case HILDON_IM_CONTEXT_ENTER_ON_FOCUS:
        case HILDON_IM_CONTEXT_SHIFT_LOCKED:
        case HILDON_IM_CONTEXT_SHIFT_UNLOCKED:
        case HILDON_IM_CONTEXT_LEVEL_LOCKED:
        case HILDON_IM_CONTEXT_LEVEL_UNLOCKED:
            // ignore
            return true;

        default:
            qWarning() << "HIM: x11FilterEvent( _HILDON_IM_COM /" << debugNameForCommunicationId(msg->type) << ") was not handled.";
            break;
        }
    } else if (event->xclient.message_type == ATOM(_HILDON_IM_SURROUNDING_CONTENT) &&
               event->xclient.format == HILDON_IM_SURROUNDING_CONTENT_FORMAT) {
        qWarning() << "HIM: x11FilterEvent( _HILDON_IM_SURROUNDING_CONTENT ) is not supported";
    } else if (event->xclient.message_type == ATOM(_HILDON_IM_SURROUNDING) &&
               event->xclient.format == HILDON_IM_SURROUNDING_FORMAT) {
        qHimDebug() << "HIM: x11FilterEvent( _HILDON_IM_SURROUNDING )";

        HildonIMSurroundingMessage *msg = reinterpret_cast<HildonIMSurroundingMessage*>(&event->xclient.data);
        setClientCursorLocation(msg->offset_is_relative, msg->cursor_offset );
        return true;
    } else if (event->xclient.message_type == ATOM(_HILDON_IM_LONG_PRESS_SETTINGS)) {
        qHimDebug() << "HIM: x11FilterEvent( _HILDON_IM_LONG_PRESS_SETTINGS )";

        HildonIMLongPressSettingsMessage *msg = reinterpret_cast<HildonIMLongPressSettingsMessage *>(&event->xclient.data);
        if (msg->enable_long_press) {
            longPressTimer->setInterval((msg->long_press_timeout > 0) ? msg->long_press_timeout : DEFAULT_LONG_PRESS_TIMEOUT);
        } else {
            longPressTimer->stop();
            longPressTimer->setInterval(0);
        }
        return true;
    }
    return false;
}

/*! \internal
Ask the client widget to insert the specified text at the cursor
 *  position, by triggering the commit signal on the context
 */
void QHildonInputContext::insertUtf8(int flag, const QString& text)
{
    qHimDebug() << "HIM: insertUtf8(" << flag << ", " << text << ")";

    QWidget *w = focusWidget();
    if (!w)
        return;

    QString cleanText = text;
    if (mask & HILDON_IM_SHIFT_LOCK_MASK)
        cleanText = cleanText.toUpper();

    lastInternalChange = true;

    //TODO HILDON_IM_AUTOCORRECT is used by the hadwriting plugin
    //Writing CiAo in the plugin add Ciao in the widget.
    if (options & HILDON_IM_AUTOCORRECT){
        qWarning() << "HILDON_IM_AUTOCORRECT Not Implemented Yet";
    }

    //Delete suroundings when we are using the preeditbuffer.
    // Eg: For the HandWriting plugin
    if (!preEditBuffer.isNull()) {
        //Updates preEditBuffer
        if (flag != HILDON_IM_MSG_START) {
            preEditBuffer.append(cleanText);
            cleanText = preEditBuffer;
        }
     }

    if (commitMode == HILDON_IM_COMMIT_PREEDIT) {
        if (preEditBuffer.isNull())
            preEditBuffer = cleanText;

        //Creating attribute list
        QList<QInputMethodEvent::Attribute> attributes;
        QTextCharFormat textCharFormat;
        textCharFormat.setFontUnderline(true);
        textCharFormat.setBackground(w->palette().highlight());
        textCharFormat.setForeground(w->palette().base());
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, cleanText.length(), textCharFormat);
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Cursor, 0, 1, QVariant());

        QInputMethodEvent e(cleanText, attributes);
        QApplication::sendEvent(w, &e);

        //Reset commit mode
        if (flag == HILDON_IM_MSG_END)
            setCommitMode(lastCommitMode, false);
    } else { // commitMode != HILDON_IM_COMMIT_PREEDIT
        QInputMethodEvent e;
        e.setCommitString(cleanText);
        QApplication::sendEvent(w, &e);
    }
}

void QHildonInputContext::clearSelection()
{
    qHimDebug() << "HIM: clearSelection()";

    QWidget *w = focusWidget();
    if (!w)
        return;

    int textCursorPos = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
    QString selection = w->inputMethodQuery(Qt::ImCurrentSelection).toString();

    if (selection.isEmpty())
        return;

    //Remove the selection
    QInputMethodEvent e;
    e.setCommitString(selection);
    QApplication::sendEvent(w, &e);

    //Move the cursor backward if the text has been selected from right to left
    if (textCursorPos < textCursorPosOnPress){
        QInputMethodEvent e;
        e.setCommitString(QString(), -selection.length(),0);
        QApplication::sendEvent(w, &e);
    }
}

void QHildonInputContext::cancelPreedit()
{
    qHimDebug() << "HIM: cancelPreedit()";

    QWidget *w = focusWidget();
    if (!w)
        return;

    if (preEditBuffer.isEmpty())
        return;
    preEditBuffer.clear();

    QInputMethodEvent e;
    QApplication::sendEvent(w, &e);
    if (realFocus)
        QApplication::sendEvent(realFocus, &e);
}

void QHildonInputContext::sendHildonCommand(HildonIMCommand cmd, QWidget *widget)
{
    qHimDebug() << "HIM: sendHildonCommand(" << cmd << "," << widget << ")";

    Window w = findHildonIm();
    if (!w)
        return;

    XEvent ev;
    memset(&ev, 0, sizeof(XEvent));

    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = ATOM(_HILDON_IM_ACTIVATE);
    ev.xclient.format = HILDON_IM_ACTIVATE_FORMAT;

    HildonIMActivateMessage *msg = reinterpret_cast<HildonIMActivateMessage *>(&ev.xclient.data);

    if (widget) {
        msg->input_window = QHIMProxyWidget::proxyFor(widget)->winId();
        msg->app_window = widget->window()->winId();
    } else if (cmd != HILDON_IM_HIDE) {
        qWarning() << "Invalid Hildon Command:" << cmd;
        return;
    }

    if (cmd == HILDON_IM_SETCLIENT || cmd == HILDON_IM_SETNSHOW)
        sendInputMode();

    msg->cmd = cmd;
    msg->trigger = triggerMode;

    XSendEvent(X11->display, w, false, 0, &ev);
    XSync(X11->display, False);
}


/*!
\internal
 */
void QHildonInputContext::sendX11Event(XEvent *event)
{
    if (Window w = findHildonIm()) {
        event->xclient.type = ClientMessage;
        event->xclient.window = w;

        XSendEvent(X11->display, w, false, 0, event);
        XSync(X11->display, False);
    }
}

//CONTEXT
/*! \internal
Updates the IM with the autocap state at the active cursor position
 */
void QHildonInputContext::checkSentenceStart()
{
    qHimDebug() << "HIM: checkSentenceStart()";

    if (!(options & HILDON_IM_AUTOCASE))
        return;

    QWidget *w = QInputContext::focusWidget();
    if (!w)
        return;

    if ((inputMode & (HILDON_GTK_INPUT_MODE_ALPHA | HILDON_GTK_INPUT_MODE_AUTOCAP)) !=
            (HILDON_GTK_INPUT_MODE_ALPHA | HILDON_GTK_INPUT_MODE_AUTOCAP)) {
        // If autocap is off, but the mode contains alpha, send autocap message.
        // The important part is that when entering a numerical entry the autocap
        // is not defined, and the plugin sets the mode appropriate for the language */
        if (inputMode & HILDON_GTK_INPUT_MODE_ALPHA) {
            autoUpper = false;
            sendHildonCommand(HILDON_IM_SHIFT_UNSTICKY, w);
        }
        return;
    } else if (inputMode & HILDON_GTK_INPUT_MODE_INVISIBLE) {
        // no autocap for passwords
        autoUpper = false;
        sendHildonCommand(HILDON_IM_SHIFT_UNSTICKY, w);
    }

    int cpos = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
    QString analyze;
    const int analyzeCount = 10;

    // Improve performance: only analyze 10 chars before the cursor
    if (cpos) {
        analyze = w->inputMethodQuery(Qt::ImSurroundingText).toString()
                                                            .mid(qMax(cpos - analyzeCount, 0), qMin(cpos, analyzeCount));
    }

    int spaces = 0;

    while (spaces < analyze.length()) {
        if (analyze.at(analyze.length() - spaces - 1).isSpace())
            spaces++;
        else
            break;
    }

    // not very nice, but QTextBoundaryFinder doesn't really work here
    static const QString punctuation = QLatin1String(".!?\xa1\xbf"); // spanish inverted ! and ?

    if (!cpos || analyze.length() == spaces ||
        (spaces && punctuation.contains(analyze.at(analyze.length() - spaces - 1)))) {
        autoUpper = true;
        sendHildonCommand(HILDON_IM_SHIFT_STICKY, w);
    } else {
        autoUpper = false;
        sendHildonCommand(HILDON_IM_SHIFT_UNSTICKY, w);
    }
}

void QHildonInputContext::commitPreeditBuffer()
{
    qHimDebug() << "HIM: commitPreeditBuffer()";

    QWidget *w = focusWidget();
    if (!w)
        return;

    QInputMethodEvent e;

    if (spaceAfterCommit)
        e.setCommitString(preEditBuffer + QLatin1Char(' '));
    else
        e.setCommitString(preEditBuffer);

    QApplication::sendEvent(w, &e);
    preEditBuffer.clear();
}

void QHildonInputContext::sendSurrounding(bool sendAllContents)
{
    QWidget *w = focusWidget();
    if (!w)
        return;

    QString surrounding;
    int cpos;
    if (sendAllContents) {
         // Qt::ImSurrounding only returns the current block

        if (QTextEdit *te = qobject_cast<QTextEdit*>(w)) {
            surrounding = te->toPlainText();
            cpos = te->textCursor().position();
        } else if (QPlainTextEdit *pte = qobject_cast<QPlainTextEdit*>(w)) {
            surrounding = pte->toPlainText();
            cpos = pte->textCursor().position();
        } else if (QGraphicsObject *declarativeTextEdit = qDeclarativeTextEdit_cast(w)) {
            surrounding = declarativeTextEdit->property("text").toString();
            cpos = declarativeTextEdit->property("cursorPosition").toInt();
        } else {
            surrounding = w->inputMethodQuery(Qt::ImSurroundingText).toString();
            cpos = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
        }
    } else {
        surrounding = w->inputMethodQuery(Qt::ImSurroundingText).toString();
        cpos = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
    }

    if (surrounding.isEmpty())
        cpos = 0;

    XEvent xev;
    HildonIMSurroundingContentMessage *surroundingContentMsg = reinterpret_cast<HildonIMSurroundingContentMessage*>(&xev.xclient.data);

    // Split surrounding context into parts that are small enough to send in a X11 message
    QByteArray ba = surrounding.toUtf8();
    bool firstPart = true;
    int offset = 0;

    while (firstPart || (offset < ba.size())) {
        //this call will take care of adding the trailing '\0' for surrounding string
        memset(&xev, 0, sizeof(XEvent));
        xev.xclient.message_type = ATOM(_HILDON_IM_SURROUNDING_CONTENT);
        xev.xclient.format = HILDON_IM_SURROUNDING_CONTENT_FORMAT;

        int len = qMin(ba.size() - offset, int(HILDON_IM_CLIENT_MESSAGE_BUFFER_SIZE) - 1);
        ::memcpy(surroundingContentMsg->surrounding, ba.constData() + offset, len);
        offset += len;

        if (firstPart)
            surroundingContentMsg->msg_flag = HILDON_IM_MSG_START;
        else if (offset == ba.size())
            surroundingContentMsg->msg_flag = HILDON_IM_MSG_END;
        else
            surroundingContentMsg->msg_flag = HILDON_IM_MSG_CONTINUE;

        sendX11Event(&xev);
        firstPart = false;
    }

    // Send the cursor offset in the surrounding
    memset(&xev, 0, sizeof(XEvent));
    xev.xclient.message_type = ATOM(_HILDON_IM_SURROUNDING);
    xev.xclient.format = HILDON_IM_SURROUNDING_FORMAT;

    HildonIMSurroundingMessage *surroundingMsg = reinterpret_cast<HildonIMSurroundingMessage *>(&xev.xclient.data);
    surroundingMsg->commit_mode = commitMode;
    surroundingMsg->cursor_offset = cpos;
    sendX11Event(&xev);
}


void QHildonInputContext::sendInputMode()
{
    qHimDebug() << "HIM: sendInputMode";

    Window w = findHildonIm();
    if (!w)
        return;

    XEvent ev;
    memset(&ev, 0, sizeof(XEvent));

    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = ATOM(_HILDON_IM_INPUT_MODE);
    ev.xclient.format = HILDON_IM_INPUT_MODE_FORMAT;

    HildonIMInputModeMessage *msg = reinterpret_cast<HildonIMInputModeMessage *>(&ev.xclient.data);
    msg->input_mode = static_cast<HildonGtkInputMode>(inputMode);
    msg->default_input_mode = static_cast<HildonGtkInputMode>(HILDON_GTK_INPUT_MODE_FULL);

    XSendEvent(X11->display, w, false, 0, &ev);
    XSync(X11->display, False);
}

/*! \internal
In redirect mode we use a proxy widget (fullscreen vkb). When the cursor position
 *  changes there, the HIM update the cursor position in the client (Qt application)
 */
void QHildonInputContext::setClientCursorLocation(bool offsetIsRelative, int cursorOffset)
{
    qHimDebug() << "HIM: setClientCursorLocation(" << offsetIsRelative << ", " << cursorOffset<< ")";

    QWidget *w = focusWidget();
    if (!w)
        return;

    if (offsetIsRelative)
        cursorOffset += w->inputMethodQuery(Qt::ImCursorPosition).toInt();


    QList<QInputMethodEvent::Attribute> attributes;

    attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection,
                                               cursorOffset, 0, QVariant());

    QInputMethodEvent e(QString(), attributes);
    QApplication::sendEvent(w, &e);
}

void QHildonInputContext::setMaskState(int *mask,
                                              HildonIMInternalModifierMask lock_mask,
                                              HildonIMInternalModifierMask sticky_mask,
                                              bool was_press_and_release)
{
    //LOGMESSAGE3("setMaskState", lock_mask, sticky_mask)
    //LOGMESSAGE3(" - ", "mask=", *mask)

   /* Locking Fn is disabled in TELE and NUMERIC */
    if (!(inputMode & HILDON_GTK_INPUT_MODE_ALPHA) &&
        !(inputMode & HILDON_GTK_INPUT_MODE_HEXA)  &&
        ((inputMode & HILDON_GTK_INPUT_MODE_TELE) ||
         (inputMode & HILDON_GTK_INPUT_MODE_NUMERIC))
       ) {
        if (*mask & lock_mask){
            /* already locked, remove lock and set it to sticky */
            *mask &= ~(lock_mask | sticky_mask);
            *mask |= sticky_mask;
        }else if (*mask & sticky_mask){
            /* the key is already sticky, it's fine */
        }else if (was_press_and_release){
            /* Pressing the key for the first time stickies the key for one character,
             * but only if no characters were entered while holding the key down */
            *mask |= sticky_mask;
        }
        return;
    }

    if (*mask & lock_mask)
    {
        /* Pressing the key while already locked clears the state */
        if (lock_mask & HILDON_IM_SHIFT_LOCK_MASK)
            sendHildonCommand(HILDON_IM_SHIFT_UNLOCKED, QInputContext::focusWidget());
        else if (lock_mask & HILDON_IM_LEVEL_LOCK_MASK)
            sendHildonCommand(HILDON_IM_MOD_UNLOCKED, QInputContext::focusWidget());

        *mask &= ~(lock_mask | sticky_mask);
    } else if (*mask & sticky_mask) {
        /* When the key is already sticky, a second press locks the key */
        *mask |= lock_mask;

        if (lock_mask & HILDON_IM_SHIFT_LOCK_MASK)
            sendHildonCommand(HILDON_IM_SHIFT_LOCKED, QInputContext::focusWidget());
        else if (lock_mask & HILDON_IM_LEVEL_LOCK_MASK)
            sendHildonCommand(HILDON_IM_MOD_LOCKED, QInputContext::focusWidget());
    }else if (was_press_and_release){
        /* Pressing the key for the first time stickies the key for one character,
         * but only if no characters were entered while holding the key down */
        *mask |= sticky_mask;
    }

}

#endif
