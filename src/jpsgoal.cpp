/***************************************************************
 *
 * \file jpsgoal.cpp
 * \date 2019-03-26
 * \version 0.8.8
 * \author Tao Zhong
 * \copyright <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section Lincense
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 * This class define a element - goal - in JuPedSim.
 * It contains a GraphicsRectItem as geometry, and others attributes for jpscore
 *
****************************************************************/


#include "jpsgoal.h"
#include "global.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QStyle>
#include <QStyleOption>

JPSGoal::JPSGoal(QGraphicsRectItem *goalRectItem)
    : QObject(), QGraphicsRectItem(), m_resizing(false),
    m_angle(0.0), m_shearHorizontal(0.0), m_shearVertical(0.0)
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    //take over memory that point to currentSource,
    //because currentSource will be deleted after instantiation of JPSGoal.
    rectItem = goalRectItem;

    //Set-up rectangle
    setRect(rectItem->rect());
}

JPSGoal::~JPSGoal()
= default;

// For drawing
void JPSGoal::setPen(const QPen &pen_)
{
    if (isSelected() && pen_ != pen()) {
        QGraphicsRectItem::setPen(pen_);
        emit dirty();
    }
}


void JPSGoal::setBrush(const QBrush &brush_)
{
    if (isSelected() && brush_ != brush()) {
        QGraphicsRectItem::setBrush(brush_);
        emit dirty();
    }
}


void JPSGoal::setAngle(double angle)
{
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
}


void JPSGoal::setShear(double shearHorizontal, double shearVertical)
{
    if (isSelected() &&
        (!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
         !qFuzzyCompare(m_shearVertical, shearVertical))) {
        m_shearHorizontal = shearHorizontal;
        m_shearVertical = shearVertical;
        updateTransform();
    }
}


void JPSGoal::updateTransform()
{
    QTransform transform;
    transform.shear(m_shearHorizontal, m_shearVertical);
    transform.rotate(m_angle);
    setTransform(transform);
}


QVariant JPSGoal::itemChange(GraphicsItemChange change,
                               const QVariant &value)
{
    if (isDirtyChange(change))
            emit dirty();
    return QGraphicsRectItem::itemChange(change, value);
}


void JPSGoal::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier ||
        event->modifiers() & Qt::ControlModifier) {
        bool move = event->modifiers() & Qt::ControlModifier;
        bool changed = true;
        double dx1 = 0.0;
        double dy1 = 0.0;
        double dx2 = 0.0;
        double dy2 = 0.0;
        switch (event->key()) {
            case Qt::Key_Left:
                if (move)
                    dx1 = -1.0;
                dx2 = -1.0;
                break;
            case Qt::Key_Right:
                if (move)
                    dx1 = 1.0;
                dx2 = 1.0;
                break;
            case Qt::Key_Up:
                if (move)
                    dy1 = -1.0;
                dy2 = -1.0;
                break;
            case Qt::Key_Down:
                if (move)
                    dy1 = 1.0;
                dy2 = 1.0;
                break;
            default:
                changed = false;
        }
        if (changed) {
            setRect(rect().adjusted(dx1, dy1, dx2, dy2));
            event->accept();
            emit dirty();
            return;
        }
    }
    QGraphicsRectItem::keyPressEvent(event);
}


void JPSGoal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}


void JPSGoal::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
#ifdef ALTERNATIVE_RESIZING
        qreal dx = event->pos().x() - event->lastPos().x();
        qreal dy = event->pos().y() - event->lastPos().y();
        setRect(rect().adjusted(0, 0, dx, dy).normalized());
#else
        QRectF rectangle(rect());
        if (event->pos().x() < rectangle.x())
            rectangle.setBottomLeft(event->pos());
        else
            rectangle.setBottomRight(event->pos());
        setRect(rectangle);
#endif
        scene()->update();
    }
    else
        QGraphicsRectItem::mouseMoveEvent(event);
}


void JPSGoal::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
    }
    else
        QGraphicsRectItem::mouseReleaseEvent(event);
}



/*
    since 0.8.8

    Hide paint() in father class QGraphicsRectItem, change the style of rectangle.
*/
void JPSGoal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget * /* widget */)
{
    QPen pen(Qt::darkGreen,0);

    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawRect(this->rect());
        return;
    } else
    {
        painter->setPen(pen);
        painter->drawRect(this->rect());
    }
}
