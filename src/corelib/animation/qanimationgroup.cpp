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

/*!
    \class QAnimationGroup
    \brief The QAnimationGroup class is an abstract base class for groups of animations.
    \since 4.6
    \ingroup animation

    An animation group is a container for animations (subclasses of
    QAbstractAnimation). A group is usually responsible for managing
    the \l{QAbstractAnimation::State}{state} of its animations, i.e.,
    it decides when to start, stop, resume, and pause them. Currently,
    Qt provides two such groups: QParallelAnimationGroup and
    QSequentialAnimationGroup. Look up their class descriptions for
    details.

    Since QAnimationGroup inherits from QAbstractAnimation, you can
    combine groups, and easily construct complex animation graphs.
    You can query QAbstractAnimation for the group it belongs to
    (using the \l{QAbstractAnimation::}{group()} function).

    To start a top-level animation group, you simply use the
    \l{QAbstractAnimation::}{start()} function from
    QAbstractAnimation. By a top-level animation group, we think of a
    group that itself is not contained within another group. Starting
    sub groups directly is not supported, and may lead to unexpected
    behavior.

    \omit OK, we'll put in a snippet on this here \endomit

    QAnimationGroup provides methods for adding and retrieving
    animations. Besides that, you can remove animations by calling
    remove(), and clear the animation group by calling
    clearAnimations(). You may keep track of changes in the group's
    animations by listening to QEvent::ChildAdded and
    QEvent::ChildRemoved events.

    \omit OK, let's find a snippet here as well. \endomit

    QAnimationGroup takes ownership of the animations it manages, and
    ensures that they are deleted when the animation group is deleted.

    You can also use a \l{The State Machine Framework}{state machine}
    to create complex animations. The framework provides a special
    state, QAnimationState, that plays an animation upon entry and
    transitions to a new state when the animation has finished
    playing. This technique can also be combined with using animation
    groups.

    \sa QAbstractAnimation, QVariantAnimation, {The Animation Framework}
*/

#ifndef QT_NO_ANIMATION

#include "qanimationgroup.h"
#include <QtCore/qdebug.h>
#include <QtCore/qcoreevent.h>
#include "qanimationgroup_p.h"

QT_BEGIN_NAMESPACE


/*!
    Constructs a QAnimationGroup.
    \a parent is passed to QObject's constructor.
*/
QAnimationGroup::QAnimationGroup(QObject *parent)
    : QAbstractAnimation(*new QAnimationGroupPrivate, parent)
{
}

/*!
    \internal
*/
QAnimationGroup::QAnimationGroup(QAnimationGroupPrivate &dd, QObject *parent)
    : QAbstractAnimation(dd, parent)
{
}

/*!
    Destroys the animation group. It will also destroy all its animations.
*/
QAnimationGroup::~QAnimationGroup()
{
}

/*!
    Returns a pointer to the animation at \a index in this group. This
    function is useful when you need access to a particular animation.  \a
    index is between 0 and animationCount() - 1.

    \sa animationCount(), indexOfAnimation()
*/
QAbstractAnimation *QAnimationGroup::animationAt(int index) const
{
    Q_D(const QAnimationGroup);

    if (index < 0 || index >= d->animations.size()) {
        qWarning("QAnimationGroup::animationAt: index is out of bounds");
        return 0;
    }

    return d->animations.at(index);
}


/*!
    Returns the number of animations managed by this group.

    \sa indexOfAnimation(), addAnimation(), animationAt()
*/
int QAnimationGroup::animationCount() const
{
    Q_D(const QAnimationGroup);
    return d->animations.size();
}

/*!
    Returns the index of \a animation. The returned index can be passed
    to the other functions that take an index as an argument.

    \sa insertAnimationAt(), animationAt(), takeAnimationAt()
*/
int QAnimationGroup::indexOfAnimation(QAbstractAnimation *animation) const
{
    Q_D(const QAnimationGroup);
    return d->animations.indexOf(animation);
}

/*!
    Adds \a animation to this group. This will call insertAnimationAt with
    index equals to animationCount().

    \note The group takes ownership of the animation.

    \sa removeAnimation()
*/
void QAnimationGroup::addAnimation(QAbstractAnimation *animation)
{
    Q_D(QAnimationGroup);
    insertAnimationAt(d->animations.count(), animation);
}

/*!
    Inserts \a animation into this animation group at \a index.
    If \a index is 0 the animation is inserted at the beginning.
    If \a index is animationCount(), the animation is inserted at the end.

    \note The group takes ownership of the animation.

    \sa takeAnimationAt(), addAnimation(), indexOfAnimation(), removeAnimation()
*/
void QAnimationGroup::insertAnimationAt(int index, QAbstractAnimation *animation)
{
    Q_D(QAnimationGroup);

    if (index < 0 || index > d->animations.size()) {
        qWarning("QAnimationGroup::insertAnimationAt: index is out of bounds");
        return;
    }

    if (QAnimationGroup *oldGroup = animation->group())
        oldGroup->removeAnimation(animation);

    d->animations.insert(index, animation);
    QAbstractAnimationPrivate::get(animation)->group = this;
    // this will make sure that ChildAdded event is sent to 'this'
    animation->setParent(this);
    d->animationInsertedAt(index);
}

/*!
    Removes \a animation from this group. The ownership of \a animation is
    transferred to the caller.

    \sa takeAnimationAt(), insertAnimationAt(), addAnimation()
*/
void QAnimationGroup::removeAnimation(QAbstractAnimation *animation)
{
    Q_D(QAnimationGroup);

    if (!animation) {
        qWarning("QAnimationGroup::remove: cannot remove null animation");
        return;
    }
    int index = d->animations.indexOf(animation);
    if (index == -1) {
        qWarning("QAnimationGroup::remove: animation is not part of this group");
        return;
    }

    takeAnimationAt(index);
}

/*!
    Returns the animation at \a index and removes it from the animation group.

    \note The ownership of the animation is transferred to the caller.

    \sa removeAnimation(), addAnimation(), insertAnimationAt(), indexOfAnimation()
*/
QAbstractAnimation *QAnimationGroup::takeAnimationAt(int index)
{
    Q_D(QAnimationGroup);
    if (index < 0 || index >= d->animations.size()) {
        qWarning("QAnimationGroup::takeAnimationAt: no animation at index %d", index);
        return 0;
    }
    QAbstractAnimation *animation = d->animations.at(index);
    QAbstractAnimationPrivate::get(animation)->group = 0;
    // ### removing from list before doing setParent to avoid inifinite recursion
    // in ChildRemoved event
    d->animations.removeAt(index);
    animation->setParent(0);
    d->animationRemovedAt(index);
    return animation;
}

/*!
    Removes and deletes all animations in this animation group, and resets the current
    time to 0.

    \sa addAnimation(), removeAnimation()
*/
void QAnimationGroup::clearAnimations()
{
    Q_D(QAnimationGroup);
    qDeleteAll(d->animations);
}

/*!
    \reimp
*/
bool QAnimationGroup::event(QEvent *event)
{
    Q_D(QAnimationGroup);
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent *childEvent = static_cast<QChildEvent *>(event);
        if (QAbstractAnimation *a = qobject_cast<QAbstractAnimation *>(childEvent->child())) {
            if (a->group() != this)
                addAnimation(a);
        }
    } else if (event->type() == QEvent::ChildRemoved) {
        QChildEvent *childEvent = static_cast<QChildEvent *>(event);
        QAbstractAnimation *a = static_cast<QAbstractAnimation *>(childEvent->child());
        // You can only rely on the child being a QObject because in the QEvent::ChildRemoved
        // case it might be called from the destructor.
        int index = d->animations.indexOf(a);
        if (index != -1)
            takeAnimationAt(index);
    }
    return QAbstractAnimation::event(event);
}


void QAnimationGroupPrivate::animationRemovedAt(int index)
{
    Q_Q(QAnimationGroup);
    Q_UNUSED(index);
    if (animations.isEmpty()) {
        currentTime = 0;
        q->stop();
    }
}

QT_END_NAMESPACE

#include "moc_qanimationgroup.cpp"

#endif //QT_NO_ANIMATION