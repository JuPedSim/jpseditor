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
    qDebug("Enter JPSGoal::JPSGoal");
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    //take over memory that point to currentSource,
    //because currentSource will be deleted after instantiation of JPSGoal.
    rectItem = goalRectItem;

    //Set-up rectangle
    setRect(rectItem->rect());

    //Set-up default values of a JPSGoal
    id = 0;
    caption="Goal";
    beSaved="true";
    isFinal="true";
    x_min = QString::number(this->rect().bottomLeft().x());
    y_min = QString::number(this->rect().bottomLeft().y());
    x_max = QString::number(this->rect().topRight().x());
    y_max = QString::number(this->rect().topRight().y());
    qDebug("Leave JPSGoal::JPSGoal");
}

JPSGoal::~JPSGoal()
= default;

// For JuPedSim
int JPSGoal::getId() const {
    qDebug("Enter/Return JPSGoal::getId");
    return id;
}

void JPSGoal::setId(int id) {
    qDebug("Enter JPSGoal::setId");
    JPSGoal::id = id;
    qDebug("Leave JPSGoal::setId");
}

const QString &JPSGoal::getCaption() const {
    qDebug("Enter/Return JPSGoal::getCaption");
    return caption;
}

void JPSGoal::setCaption(const QString &caption) {
    qDebug("Enter JPSGoal::setCaption");
    JPSGoal::caption = caption;
    qDebug("Leave JPSGoal::setCaption");
}

const QString &JPSGoal::getBeSaved() const {
    qDebug("Enter/Return JPSGoal::getBeSaved");
    return beSaved;
}

void JPSGoal::setBeSaved(const QString &beSaved) {
    qDebug("Enter JPSGoal::setBeSaved");
    JPSGoal::beSaved = beSaved;
    qDebug("Leave JPSGoal::setBeSaved");
}

const QString &JPSGoal::getIsFinal() const {
    qDebug("Enter/Return QString &JPSGoal::getIsFinal");
    return isFinal;
}

void JPSGoal::setIsFinal(const QString &isFinal) {
    qDebug("Enter JPSGoal::setIsFinal");
    JPSGoal::isFinal = isFinal;
    qDebug("Leave JPSGoal::setIsFinal");
}

QGraphicsRectItem *JPSGoal::getRectItem() const {
    qDebug("Enter JPSGoal::getRectItem");
    return rectItem;
    qDebug("Leave JPSGoal::getRectItem");
}

void JPSGoal::setRectItem(QGraphicsRectItem *rectItem) {
    qDebug("Enter JPSGoal::setRectItem");
    JPSGoal::rectItem = rectItem;
    qDebug("Leave JPSGoal::setRectItem");
}

const QString &JPSGoal::getXMin() const {
    qDebug("Enter/Return QString &JPSGoal::getXMin");
    return x_min;
}

void JPSGoal::setXMin(const QString &xMin) {
    qDebug("Enter JPSGoal::setXMin");
    x_min = xMin;
    qDebug("Leave JPSGoal::setXMin");
}

const QString &JPSGoal::getXMax() const {
    qDebug("Enter/Return PSGoal::getXMax");
    return x_max;
}

void JPSGoal::setXMax(const QString &xMax) {
    qDebug("Enter JPSGoal::setXMax");
    x_max = xMax;
    qDebug("Leave JPSGoal::setXMax");
}

const QString &JPSGoal::getYMin() const {
    qDebug("Enter/Return QString &JPSGoal::getYMin");
    return y_min;
}

void JPSGoal::setYMin(const QString &yMin) {
    qDebug("Enter JPSGoal::setYMin");
    y_min = yMin;
    qDebug("Leave JPSGoal::setYMin");
}

const QString &JPSGoal::getYMax() const {
    qDebug("Enter/Return JPSGoal::getYMax");
    return y_max;
}

void JPSGoal::setYMax(const QString &yMax) {
    qDebug("Enter JPSGoal::setYMax");
    y_max = yMax;
    qDebug("Leave JPSGoal::setYMax");
}

// For drawing
void JPSGoal::setPen(const QPen &pen_)
{
    qDebug("Enter JPSGoal::setPen");
    if (isSelected() && pen_ != pen()) {
        QGraphicsRectItem::setPen(pen_);
        emit dirty();
    }
    qDebug("Leave JPSGoal::setPen");
}


void JPSGoal::setBrush(const QBrush &brush_)
{
    qDebug("Enter JPSGoal::setBrush");
    if (isSelected() && brush_ != brush()) {
        QGraphicsRectItem::setBrush(brush_);
        emit dirty();
    }
    qDebug("Leave JPSGoal::setBrush");
}


void JPSGoal::setAngle(double angle)
{
    qDebug("Enter JPSGoal::setAngle");
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
    qDebug("Leave JPSGoal::setAngle");
}


void JPSGoal::setShear(double shearHorizontal, double shearVertical)
{
    qDebug("Enter JPSGoal::setShear");
    if (isSelected() &&
        (!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
         !qFuzzyCompare(m_shearVertical, shearVertical))) {
        m_shearHorizontal = shearHorizontal;
        m_shearVertical = shearVertical;
        updateTransform();
    }
    qDebug("Leave JPSGoal::setShear");
}


void JPSGoal::updateTransform()
{
    qDebug("Enter JPSGoal::updateTransform");
    QTransform transform;
    transform.shear(m_shearHorizontal, m_shearVertical);
    transform.rotate(m_angle);
    setTransform(transform);
    qDebug("Leave JPSGoal::updateTransform");
}


QVariant JPSGoal::itemChange(GraphicsItemChange change,
                               const QVariant &value)
{
    qDebug("Enter JPSGoal::itemChange");
    if (isDirtyChange(change))
            emit dirty();
    return QGraphicsRectItem::itemChange(change, value);
    qDebug("Leave JPSGoal::itemChange");
}


void JPSGoal::keyPressEvent(QKeyEvent *event)
{
    qDebug("Enter JPSGoal::keyPressEvent");
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
    qDebug("Leave JPSGoal::keyPressEvent");
    QGraphicsRectItem::keyPressEvent(event);
}


void JPSGoal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("Enter JPSGoal::mousePressEvent");
    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
        qDebug("Leave JPSGoal::mousePressEvent");
    }
    else
        qDebug("Leave JPSGoal::mousePressEvent");
        QGraphicsRectItem::mousePressEvent(event);
}


void JPSGoal::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("Enter JPSGoal::mouseMoveEvent");
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
        qDebug("Leave JPSGoal::mouseMoveEvent");
    }
    else
        qDebug("Leave JPSGoal::mouseMoveEvent");
        QGraphicsRectItem::mouseMoveEvent(event);
}


void JPSGoal::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("Enter JPSGoal::mouseReleaseEvent");
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
        qDebug("Leave JPSGoal::mouseReleaseEvent");
    }
    else
        qDebug("Leave JPSGoal::mouseReleaseEvent");
        QGraphicsRectItem::mouseReleaseEvent(event);
}



/*
    since 0.8.8

    Hide paint() in father class QGraphicsRectItem, change the style of rectangle.
*/
void JPSGoal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget * /* widget */)
{
    qDebug("Enter JPSGoal::paint");
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
    qDebug("Leave JPSGoal::paint");
}

