/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

/*

| *property* | *Used for type* |
| period     | QEasingCurve::{In,Out,InOut,OutIn}Elastic |
| amplitude  | QEasingCurve::{In,Out,InOut,OutIn}Bounce, QEasingCurve::{In,Out,InOut,OutIn}Elastic |
| overshoot  | QEasingCurve::{In,Out,InOut,OutIn}Back   |

*/




/*!
    \class QEasingCurve
    \since 4.6
    \ingroup animation
    \brief The QEasingCurve class provides easing curves for controlling animation.

    Easing curves describe a function that controls how the speed of the interpolation
    between 0 and 1 should be. Easing curves allow transitions from
    one value to another to appear more natural than a simple constant speed would allow.
    The QEasingCurve class is usually used in conjunction with the QAnimation class,
    but can be used on its own.

    To calculate the speed of the interpolation, the easing curve provides the function
    valueForProgress(), where the \a progress argument specifies the progress of the
    interpolation: 0 is the start value of the interpolation, 1 is the end value of the
    interpolation. The returned value is the effective progress of the interpolation.
    If the returned value is the same as the input value for all input values the easing
    curve is a linear curve. This is the default behaviour.

    For example,
    \code
    QEasingCurve easing(QEasingCurve::InOutQuad);

    for(qreal t = 0.0; t < 1.0; t+=0.1)
        qWarning() << "Effective progress" << t << " is
                   << easing.valueForProgress(t);
    \endcode
    will print the effective progress of the interpolation between 0 and 1.

    When using a QAnimation, the easing curve will be used to control the
    progress of the interpolation between startValue and endValue:
    \code
    QAnimation animation;
    animation.setStartValue(0);
    animation.setEndValue(1000);
    animation.setDuration(1000);
    animation.setEasingCurve(QEasingCurve::InOutQuad);
    \endcode
 */

/*!
    \enum QEasingCurve::Type

    The type of easing curve.

    \value Linear       \inlineimage qeasingcurve-linear.png
                        \br
                        Easing equation function for a simple linear tweening,
                        with no easing.
    \value InQuad       \inlineimage qeasingcurve-inquad.png
                        \br
                        Easing equation function for a quadratic (t^2) easing
                        in: accelerating from zero velocity.
    \value OutQuad      \inlineimage qeasingcurve-outquad.png
                        \br
                        Easing equation function for a quadratic (t^2) easing
                        out: decelerating to zero velocity.
    \value InOutQuad    \inlineimage qeasingcurve-inoutquad.png
                        \br
                        Easing equation function for a quadratic (t^2) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInQuad    \inlineimage qeasingcurve-outinquad.png
                        \br
                        Easing equation function for a quadratic (t^2) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InCubic      \inlineimage qeasingcurve-incubic.png
                        \br
                        Easing equation function for a cubic (t^3) easing
                        in: accelerating from zero velocity.
    \value OutCubic     \inlineimage qeasingcurve-outcubic.png
                        \br
                        Easing equation function for a cubic (t^3) easing
                        out: decelerating from zero velocity.
    \value InOutCubic   \inlineimage qeasingcurve-inoutcubic.png
                        \br
                        Easing equation function for a cubic (t^3) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInCubic   \inlineimage qeasingcurve-outincubic.png
                        \br
                        Easing equation function for a cubic (t^3) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InQuart      \inlineimage qeasingcurve-inquart.png
                        \br
                        Easing equation function for a quartic (t^4) easing
                        in: accelerating from zero velocity.
    \value OutQuart     \inlineimage qeasingcurve-outquart.png
                        \br
                        Easing equation function for a quartic (t^4) easing
                        out: decelerating from zero velocity.
    \value InOutQuart   \inlineimage qeasingcurve-inoutquart.png
                        \br
                        Easing equation function for a quartic (t^4) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInQuart   \inlineimage qeasingcurve-outinquart.png
                        \br
                        Easing equation function for a quartic (t^4) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InQuint      \inlineimage qeasingcurve-inquint.png
                        \br
                        Easing equation function for a quintic (t^5) easing
                        in: accelerating from zero velocity.
    \value OutQuint     \inlineimage qeasingcurve-outquint.png
                        \br
                        Easing equation function for a quintic (t^5) easing
                        out: decelerating from zero velocity.
    \value InOutQuint   \inlineimage qeasingcurve-inoutquint.png
                        \br
                        Easing equation function for a quintic (t^5) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInQuint   \inlineimage qeasingcurve-outinquint.png
                        \br
                        Easing equation function for a quintic (t^5) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InSine       \inlineimage qeasingcurve-insine.png
                        \br
                        Easing equation function for a sinusoidal (sin(t)) easing
                        in: accelerating from zero velocity.
    \value OutSine      \inlineimage qeasingcurve-outsine.png
                        \br
                        Easing equation function for a sinusoidal (sin(t)) easing
                        out: decelerating from zero velocity.
    \value InOutSine    \inlineimage qeasingcurve-inoutsine.png
                        \br
                        Easing equation function for a sinusoidal (sin(t)) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInSine    \inlineimage qeasingcurve-outinsine.png
                        \br
                        Easing equation function for a sinusoidal (sin(t)) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InExpo       \inlineimage qeasingcurve-inexpo.png
                        \br
                        Easing equation function for an exponential (2^t) easing
                        in: accelerating from zero velocity.
    \value OutExpo      \inlineimage qeasingcurve-outexpo.png
                        \br
                        Easing equation function for an exponential (2^t) easing
                        out: decelerating from zero velocity.
    \value InOutExpo    \inlineimage qeasingcurve-inoutexpo.png
                        \br
                        Easing equation function for an exponential (2^t) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInExpo    \inlineimage qeasingcurve-outinexpo.png
                        \br
                        Easing equation function for an exponential (2^t) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InCirc       \inlineimage qeasingcurve-incirc.png
                        \br
                        Easing equation function for a circular (sqrt(1-t^2)) easing
                        in: accelerating from zero velocity.
    \value OutCirc      \inlineimage qeasingcurve-outcirc.png
                        \br
                        Easing equation function for a circular (sqrt(1-t^2)) easing
                        out: decelerating from zero velocity.
    \value InOutCirc    \inlineimage qeasingcurve-inoutcirc.png
                        \br
                        Easing equation function for a circular (sqrt(1-t^2)) easing
                        in/out: acceleration until halfway, then deceleration.
    \value OutInCirc    \inlineimage qeasingcurve-outincirc.png
                        \br
                        Easing equation function for a circular (sqrt(1-t^2)) easing
                        out/in: deceleration until halfway, then acceleration.
    \value InElastic    \inlineimage qeasingcurve-inelastic.png
                        \br
                        Easing equation function for an elastic
                        (exponentially decaying sine wave) easing in:
                        accelerating from zero velocity.  The peak amplitude
                        can be set with the \e amplitude parameter, and the
                        period of decay by the \e period parameter.
    \value OutElastic   \inlineimage qeasingcurve-outelastic.png
                        \br
                        Easing equation function for an elastic
                        (exponentially decaying sine wave) easing out:
                        decelerating from zero velocity.  The peak amplitude
                        can be set with the \e amplitude parameter, and the
                        period of decay by the \e period parameter.
    \value InOutElastic \inlineimage qeasingcurve-inoutelastic.png
                        \br
                        Easing equation function for an elastic
                        (exponentially decaying sine wave) easing in/out:
                        acceleration until halfway, then deceleration.
    \value OutInElastic \inlineimage qeasingcurve-outinelastic.png
                        \br
                        Easing equation function for an elastic
                        (exponentially decaying sine wave) easing out/in:
                        deceleration until halfway, then acceleration.
    \value InBack       \inlineimage qeasingcurve-inback.png
                        \br
                        Easing equation function for a back (overshooting
                        cubic easing: (s+1)*t^3 - s*t^2) easing in:
                        accelerating from zero velocity.
    \value OutBack      \inlineimage qeasingcurve-outback.png
                        \br
                        Easing equation function for a back (overshooting
                        cubic easing: (s+1)*t^3 - s*t^2) easing out:
                        decelerating from zero velocity.
    \value InOutBack    \inlineimage qeasingcurve-inoutback.png
                        \br
                        Easing equation function for a back (overshooting
                        cubic easing: (s+1)*t^3 - s*t^2) easing in/out:
                        acceleration until halfway, then deceleration.
    \value OutInBack    \inlineimage qeasingcurve-outinback.png
                        \br
                        Easing equation function for a back (overshooting
                        cubic easing: (s+1)*t^3 - s*t^2) easing out/in:
                        deceleration until halfway, then acceleration.
    \value InBounce     \inlineimage qeasingcurve-inbounce.png
                        \br
                        Easing equation function for a bounce (exponentially
                        decaying parabolic bounce) easing in: accelerating
                        from zero velocity.
    \value OutBounce    \inlineimage qeasingcurve-outbounce.png
                        \br
                        Easing equation function for a bounce (exponentially
                        decaying parabolic bounce) easing out: decelerating
                        from zero velocity.
    \value InOutBounce  \inlineimage qeasingcurve-inoutbounce.png
                        \br
                        Easing equation function for a bounce (exponentially
                        decaying parabolic bounce) easing in/out:
                        acceleration until halfway, then deceleration.
    \value OutInBounce  \inlineimage qeasingcurve-outinbounce.png
                        \br
                        Easing equation function for a bounce (exponentially
                        decaying parabolic bounce) easing out/in:
                        deceleration until halfway, then acceleration.
    \omitvalue InCurve
    \omitvalue OutCurve
    \omitvalue SineCurve
    \omitvalue CosineCurve
    \value Custom       This is returned if the user have specified a custom curve type with setCustomType(). Note that you cannot call setType() with this value, but type() can return it.
    \omitvalue NCurveTypes
*/

/*!
    \typedef QEasingCurve::EasingFunction

    This is a typedef for a pointer to a function with the following
    signature:

    \snippet doc/src/snippets/code/src_corelib_tools_qeasingcurve.cpp 0
*/

#include "qeasingcurve.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#include <QtCore/QString>
#endif

QT_BEGIN_NAMESPACE

static bool isConfigFunction(QEasingCurve::Type type)
{
    return type >= QEasingCurve::InElastic
            && type <= QEasingCurve::OutInBounce;
}

class QEasingCurveFunction
{
public:
    enum Type { In, Out, InOut, OutIn };

    QEasingCurveFunction(QEasingCurveFunction::Type type = In, qreal period = 0.3, qreal amplitude = 1.0,
        qreal overshoot = 1.70158f)
        : _t(type), _p(period), _a(amplitude), _o(overshoot)
    { }
    virtual ~QEasingCurveFunction() {}
    virtual qreal value(qreal t);
    virtual QEasingCurveFunction *copy() const;
    bool operator==(const QEasingCurveFunction& other);

    Type _t;
    qreal _p;
    qreal _a;
    qreal _o;
};

qreal QEasingCurveFunction::value(qreal t)
{
    return t;
}

QEasingCurveFunction *QEasingCurveFunction::copy() const
{
    return new QEasingCurveFunction(_t, _p, _a, _o);
}

bool QEasingCurveFunction::operator==(const QEasingCurveFunction& other)
{
    return _t == other._t &&
           _p == other._p &&
           _a == other._a &&
           _o == other._o;
}

QT_BEGIN_INCLUDE_NAMESPACE
#include "../../3rdparty/easing/easing.cpp"
QT_END_INCLUDE_NAMESPACE

class QEasingCurvePrivate
{
public:
    QEasingCurvePrivate()
        : type(QEasingCurve::Linear),
          config(0),
          func(&easeNone)
    { }
    ~QEasingCurvePrivate() { delete config; }
    void setType_helper(QEasingCurve::Type);

    QEasingCurve::Type type;
    QEasingCurveFunction *config;
    QEasingCurve::EasingFunction func;
};

struct ElasticEase : public QEasingCurveFunction
{
    ElasticEase(Type type)
        : QEasingCurveFunction(type, qreal(0.3), qreal(1.0))
    { }

    QEasingCurveFunction *copy() const
    {
        ElasticEase *rv = new ElasticEase(_t);
        rv->_p = _p;
        rv->_a = _a;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal p = (_p < 0) ? 0.3f : _p;
        qreal a = (_a < 0) ? 1.0f : _a;
        switch(_t) {
        case In:
            return easeInElastic(t, a, p);
        case Out:
            return easeOutElastic(t, a, p);
        case InOut:
            return easeInOutElastic(t, a, p);
        case OutIn:
            return easeOutInElastic(t, a, p);
        default:
            return t;
        }
    }
};

struct BounceEase : public QEasingCurveFunction
{
    BounceEase(Type type)
        : QEasingCurveFunction(type, 0.3f, 1.0f)
    { }

    QEasingCurveFunction *copy() const
    {
        BounceEase *rv = new BounceEase(_t);
        rv->_a = _a;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal a = (_a < 0) ? 1.0f : _a;
        switch(_t) {
        case In:
            return easeInBounce(t, a);
        case Out:
            return easeOutBounce(t, a);
        case InOut:
            return easeInOutBounce(t, a);
        case OutIn:
            return easeOutInBounce(t, a);
        default:
            return t;
        }
    }
};

struct BackEase : public QEasingCurveFunction
{
    BackEase(Type type)
        : QEasingCurveFunction(type, 0.3f, 1.0f, 1.70158f)
    { }

    QEasingCurveFunction *copy() const
    {
        BackEase *rv = new BackEase(_t);
        rv->_o = _o;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal o = (_o < 0) ? 1.70158f : _o;
        switch(_t) {
        case In:
            return easeInBack(t, o);
        case Out:
            return easeOutBack(t, o);
        case InOut:
            return easeInOutBack(t, o);
        case OutIn:
            return easeOutInBack(t, o);
        default:
            return t;
        }
    }
};

static QEasingCurve::EasingFunction curveToFunc(QEasingCurve::Type curve)
{
    switch(curve) {
    case QEasingCurve::Linear:
        return &easeNone;
    case QEasingCurve::InQuad:
        return &easeInQuad;
    case QEasingCurve::OutQuad:
        return &easeOutQuad;
    case QEasingCurve::InOutQuad:
        return &easeInOutQuad;
    case QEasingCurve::OutInQuad:
        return &easeOutInQuad;
    case QEasingCurve::InCubic:
        return &easeInCubic;
    case QEasingCurve::OutCubic:
        return &easeOutCubic;
    case QEasingCurve::InOutCubic:
        return &easeInOutCubic;
    case QEasingCurve::OutInCubic:
        return &easeOutInCubic;
    case QEasingCurve::InQuart:
        return &easeInQuart;
    case QEasingCurve::OutQuart:
        return &easeOutQuart;
    case QEasingCurve::InOutQuart:
        return &easeInOutQuart;
    case QEasingCurve::OutInQuart:
        return &easeOutInQuart;
    case QEasingCurve::InQuint:
        return &easeInQuint;
    case QEasingCurve::OutQuint:
        return &easeOutQuint;
    case QEasingCurve::InOutQuint:
        return &easeInOutQuint;
    case QEasingCurve::OutInQuint:
        return &easeOutInQuint;
    case QEasingCurve::InSine:
        return &easeInSine;
    case QEasingCurve::OutSine:
        return &easeOutSine;
    case QEasingCurve::InOutSine:
        return &easeInOutSine;
    case QEasingCurve::OutInSine:
        return &easeOutInSine;
    case QEasingCurve::InExpo:
        return &easeInExpo;
    case QEasingCurve::OutExpo:
        return &easeOutExpo;
    case QEasingCurve::InOutExpo:
        return &easeInOutExpo;
    case QEasingCurve::OutInExpo:
        return &easeOutInExpo;
    case QEasingCurve::InCirc:
        return &easeInCirc;
    case QEasingCurve::OutCirc:
        return &easeOutCirc;
    case QEasingCurve::InOutCirc:
        return &easeInOutCirc;
    case QEasingCurve::OutInCirc:
        return &easeOutInCirc;
    // Internal for, compatibility with QTimeLine only ??
    case QEasingCurve::InCurve:
        return &easeInCurve;
    case QEasingCurve::OutCurve:
        return &easeOutCurve;
    case QEasingCurve::SineCurve:
        return &easeSineCurve;
    case QEasingCurve::CosineCurve:
        return &easeCosineCurve;
    default:
        return 0;
    };
}

static QEasingCurveFunction *curveToFunctionObject(QEasingCurve::Type type)
{
    QEasingCurveFunction *curveFunc = 0;
    switch(type) {
    case QEasingCurve::InElastic:
        curveFunc = new ElasticEase(ElasticEase::In);
        break;
    case QEasingCurve::OutElastic:
        curveFunc = new ElasticEase(ElasticEase::Out);
        break;
    case QEasingCurve::InOutElastic:
        curveFunc = new ElasticEase(ElasticEase::InOut);
        break;
    case QEasingCurve::OutInElastic:
        curveFunc = new ElasticEase(ElasticEase::OutIn);
        break;
    case QEasingCurve::OutBounce:
        curveFunc = new BounceEase(BounceEase::Out);
        break;
    case QEasingCurve::InBounce:
        curveFunc = new BounceEase(BounceEase::In);
        break;
    case QEasingCurve::OutInBounce:
        curveFunc = new BounceEase(BounceEase::OutIn);
        break;
    case QEasingCurve::InOutBounce:
        curveFunc = new BounceEase(BounceEase::InOut);
        break;
    case QEasingCurve::InBack:
        curveFunc = new BackEase(BackEase::In);
        break;
    case QEasingCurve::OutBack:
        curveFunc = new BackEase(BackEase::Out);
        break;
    case QEasingCurve::InOutBack:
        curveFunc = new BackEase(BackEase::InOut);
        break;
    case QEasingCurve::OutInBack:
        curveFunc = new BackEase(BackEase::OutIn);
        break;
    default:
        curveFunc = new QEasingCurveFunction(QEasingCurveFunction::In, 0.3f, 1.0f, 1.70158f);     // ###
    }

    return curveFunc;
}

/*!
    Constructs an easing curve of the given \a type.
 */
QEasingCurve::QEasingCurve(Type type)
    : d_ptr(new QEasingCurvePrivate)
{
    setType(type);
}

/*!
    Construct a copy of \a other.
 */
QEasingCurve::QEasingCurve(const QEasingCurve &other)
: d_ptr(new QEasingCurvePrivate)
{
    // ### non-atomic, requires malloc on shallow copy
    *d_ptr = *other.d_ptr;
    if(other.d_ptr->config)
        d_ptr->config = other.d_ptr->config->copy();
}

/*!
    Destructor.
 */

QEasingCurve::~QEasingCurve()
{
    delete d_ptr;
}

/*!
    Copy \a other.
 */
QEasingCurve &QEasingCurve::operator=(const QEasingCurve &other)
{
    // ### non-atomic, requires malloc on shallow copy
    if (d_ptr->config) {
        delete d_ptr->config;
        d_ptr->config = 0;
    }

    *d_ptr = *other.d_ptr;
    if(other.d_ptr->config)
        d_ptr->config = other.d_ptr->config->copy();

    return *this;
}

/*!
    Compare this easing curve with \a other and returns true if they are
    equal. It will also compare the properties of a curve.
 */
bool QEasingCurve::operator==(const QEasingCurve &other) const
{
    bool res = d_ptr->func == other.d_ptr->func
            && d_ptr->type == other.d_ptr->type;
    if (res && d_ptr->config && other.d_ptr->config) {
        // catch the config content
        res = d_ptr->config->operator==(*(other.d_ptr->config));
    }
    return res;
}

/*!
    \fn bool QEasingCurve::operator!=(const QEasingCurve &other) const
    Compare this easing curve with \a other and returns true if they are not equal.
    It will also compare the properties of a curve.

    \sa operator==()
*/

/*!
    Returns the amplitude. This is not applicable for all curve types.
    It is only applicable for bounce and elastic curves (curves of type()
    QEasingCurve::InBounce, QEasingCurve::OutBounce, QEasingCurve::InOutBounce,
    QEasingCurve::OutInBounce, QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic).
 */
qreal QEasingCurve::amplitude() const
{
    return d_ptr->config ? d_ptr->config->_a : 1.0;
}

/*!
    Sets the amplitude to \a amplitude.

    This will set the amplitude of the bounce or the amplitude of the
    elastic "spring" effect. The higher the number, the higher the amplitude.
    \sa amplitude()
*/
void QEasingCurve::setAmplitude(qreal amplitude)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_a = amplitude;
}

/*!
    Returns the period. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic.
 */
qreal QEasingCurve::period() const
{
    return d_ptr->config ? d_ptr->config->_p : 0.3;
}

/*!
    Sets the period to \a period.
    Setting a small period value will give a high frequency of the curve. A
    large period will give it a small frequency.

    \sa period()
*/
void QEasingCurve::setPeriod(qreal period)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_p = period;
}

/*!
    Returns the overshoot. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InBack, QEasingCurve::OutBack,
    QEasingCurve::InOutBack or QEasingCurve::OutInBack.
 */
qreal QEasingCurve::overshoot() const
{
    return d_ptr->config ? d_ptr->config->_o : 1.70158f;
}

/*!
    Sets the overshoot to \a overshoot.

    0 produces no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent.

    \sa overshoot()
*/
void QEasingCurve::setOvershoot(qreal overshoot)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_o = overshoot;
}

/*!
    Returns the type of the easing curve.
*/
QEasingCurve::Type QEasingCurve::type() const
{
    return d_ptr->type;
}

void QEasingCurvePrivate::setType_helper(QEasingCurve::Type newType)
{
    qreal amp = -1.0;
    qreal period = -1.0;
    qreal overshoot = -1.0;

    if (config) {
        amp = config->_a;
        period = config->_p;
        overshoot = config->_o;
        delete config;
        config = 0;
    }

    if (isConfigFunction(newType) || (amp != -1.0) || (period != -1.0) || (overshoot != -1.0)) {
        config = curveToFunctionObject(newType);
        if (amp != -1.0)
            config->_a = amp;
        if (period != -1.0)
            config->_p = period;
        if (overshoot != -1.0)
            config->_o = overshoot;
        func = 0;
    } else if (newType != QEasingCurve::Custom) {
        func = curveToFunc(newType);
    }
    Q_ASSERT((func == 0) == (config != 0));
    type = newType;
}

/*!
    Sets the type of the easing curve to \a type.
*/
void QEasingCurve::setType(Type type)
{
    if (d_ptr->type == type)
        return;
    if (type < Linear || type >= NCurveTypes - 1) {
        qWarning("QEasingCurve: Invalid curve type %d", type);
        return;
    }

    d_ptr->setType_helper(type);
}

/*!
    Sets a custom easing curve that is defined by the user in the function \a func.
    The signature of the function is qreal myEasingFunction(qreal progress),
    where \e progress and the return value is considered to be normalized between 0 and 1.
    (In some cases the return value can be outside that range)
    After calling this function type() will return QEasingCurve::Custom.
    \a func cannot be zero.

    \sa customType()
    \sa valueForProgress()
*/
void QEasingCurve::setCustomType(EasingFunction func)
{
    if (!func) {
        qWarning("Function pointer must not be null");
        return;
    }
    d_ptr->func = func;
    d_ptr->setType_helper(Custom);
}

/*!
    Returns the function pointer to the custom easing curve.
    If type() does not return QEasingCurve::Custom, this function
    will return 0.
*/
QEasingCurve::EasingFunction QEasingCurve::customType() const
{
    return d_ptr->type == Custom ? d_ptr->func : 0;
}

/*!
    Return the effective progress for the easing curve at \a progress.
    While  \a progress must be between 0 and 1, the returned effective progress
    can be outside those bounds. For instance, QEasingCurve::InBack will
    return negative values in the beginning of the function.
 */
qreal QEasingCurve::valueForProgress(qreal progress) const
{
    progress = qBound<qreal>(0, progress, 1);
    if (d_ptr->func)
        return d_ptr->func(progress);
    else if (d_ptr->config)
        return d_ptr->config->value(progress);
    else
        return progress;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const QEasingCurve &item)
{
    debug << "type:" << item.d_ptr->type
          << "func:" << item.d_ptr->func;
    if (item.d_ptr->config) {
        debug << QString::fromAscii("period:%1").arg(item.d_ptr->config->_p, 0, 'f', 20)
              << QString::fromAscii("amp:%1").arg(item.d_ptr->config->_a, 0, 'f', 20)
              << QString::fromAscii("overshoot:%1").arg(item.d_ptr->config->_o, 0, 'f', 20);
    }
    return debug;
}
#endif

QT_END_NAMESPACE