/**
 * \file       jpssource.h
 * \date        Mar-13-2019
 * \version     v0.8.8
 * \author      Tao Zhong
 * \copyright   <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
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
 **/

/*
 * This class define a element - source - in JuPedSim.
 * It contains a GraphicsRectItem as geometry, and others attributes
 * It will be called in graphicview and contaioned in sourceVector, which is a QList<JPSSource*>
 * */

#include "jpssource.h"
#include "global.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QCursor>

JPSSource::JPSSource(QGraphicsRectItem *rectItem, QGraphicsScene *scene)
    : QObject(), QGraphicsRectItem(), m_resizing(false),
      m_angle(0.0), m_shearHorizontal(0.0), m_shearVertical(0.0)
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             #if QT_VERSION >= 0x040600
             QGraphicsItem::ItemSendsGeometryChanges|
             #endif
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    currentPen.setColor(Qt::darkRed);
    currentPen.setCosmetic(true);
    currentPen.setWidth(2);

    setRect(rectItem->rect());
    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
    setFocus();

    caption="Source";
    agents_max = "0";
    frequency = "0";
    group_id = "0";
    N_create = "0";
    percent = "0";
    rate = "0";
    startX = "0";
    startY = "0";
    time = "0";
    time_max = "0";
    time_min = "0";
    greedy = "false";
    beSaved = true;

    x_min = this->rect().bottomLeft().x();
    y_min = this->rect().bottomLeft().y();
    x_max = this->rect().topRight().x();
    y_max = this->rect().topRight().y();

}

JPSSource::~JPSSource()
{

}

void JPSSource::setId(int id) {
    JPSSource::id = id;
}

const QString &JPSSource::getFrequency() const {
    return frequency;
}

void JPSSource::setFrequency(const QString &frequency) {
    JPSSource::frequency = frequency;
}

const QString &JPSSource::getN_create() const {
    return N_create;
}

void JPSSource::setN_create(const QString &N_create) {
    JPSSource::N_create = N_create;
}

const QString &JPSSource::getPercent() const {
    return percent;
}

void JPSSource::setPercent(const QString &percent) {
    JPSSource::percent = percent;
}

const QString &JPSSource::getRate() const {
    return rate;
}

void JPSSource::setRate(const QString &rate) {
    JPSSource::rate = rate;
}

const QString &JPSSource::getTime_min() const {
    return time_min;
}

void JPSSource::setTime_min(const QString &time_min) {
    JPSSource::time_min = time_min;
}

const QString &JPSSource::getTime_max() const {
    return time_max;
}

void JPSSource::setTime_max(const QString &time_max) {
    JPSSource::time_max = time_max;
}

const QString &JPSSource::getAgents_max() const {
    return agents_max;
}

void JPSSource::setAgents_max(const QString &agents_max) {
    JPSSource::agents_max = agents_max;
}

const QString &JPSSource::getGroup_id() const {
    return group_id;
}

void JPSSource::setGroup_id(const QString &group_id) {
    JPSSource::group_id = group_id;
}

const QString &JPSSource::getCaption() const {
    return caption;
}

void JPSSource::setCaption(const QString &caption) {
    JPSSource::caption = caption;
}

const QString &JPSSource::getGreedy() const {
    return greedy;
}

void JPSSource::setGreedy(const QString &greedy) {
    JPSSource::greedy = greedy;
}

const QString &JPSSource::getTime() const {
    return time;
}

void JPSSource::setTime(const QString &time) {
    JPSSource::time = time;
}

const QString &JPSSource::getStartX() const {
    return startX;
}

void JPSSource::setStartX(const QString &startX) {
    JPSSource::startX = startX;
}

const QString &JPSSource::getStartY() const {
    return startY;
}

void JPSSource::setStartY(const QString &startY) {
    JPSSource::startY = startY;
}

qreal JPSSource::getX_min() const {
    return x_min;
}

void JPSSource::setX_min(qreal x_min) {
    JPSSource::x_min = x_min;
}

qreal JPSSource::getX_max() const {
    return x_max;
}

void JPSSource::setX_max(qreal x_max) {
    JPSSource::x_max = x_max;
}

qreal JPSSource::getY_min() const {
    return y_min;
}

void JPSSource::setY_min(qreal y_min) {
    JPSSource::y_min = y_min;
}

qreal JPSSource::getY_max() const {
    return y_max;
}

void JPSSource::setY_max(qreal y_max) {
    JPSSource::y_max = y_max;
}

int JPSSource::getId() const {
    return id;
}

bool JPSSource::isBeSaved() const {
    return beSaved;
}

void JPSSource::setBeSaved(bool beSaved) {
    JPSSource::beSaved = beSaved;
}

// For drawing
void JPSSource::setPen(const QPen &pen_)
{
    if (isSelected() && pen_ != pen()) {
        QGraphicsRectItem::setPen(pen_);
        emit dirty();
    }
}


void JPSSource::setBrush(const QBrush &brush_)
{
    if (isSelected() && brush_ != brush()) {
        QGraphicsRectItem::setBrush(brush_);
        emit dirty();
    }
}


void JPSSource::setAngle(double angle)
{
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
}


void JPSSource::setShear(double shearHorizontal, double shearVertical)
{
    if (isSelected() &&
        (!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
         !qFuzzyCompare(m_shearVertical, shearVertical))) {
        m_shearHorizontal = shearHorizontal;
        m_shearVertical = shearVertical;
        updateTransform();
    }
}


void JPSSource::updateTransform()
{
    QTransform transform;
    transform.shear(m_shearHorizontal, m_shearVertical);
    transform.rotate(m_angle);
    setTransform(transform);
}


QVariant JPSSource::itemChange(GraphicsItemChange change,
                             const QVariant &value)
{
    if (isDirtyChange(change))
            emit dirty();
    return QGraphicsRectItem::itemChange(change, value);
}


void JPSSource::keyPressEvent(QKeyEvent *event)
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


void JPSSource::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}


void JPSSource::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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


void JPSSource::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
    }
    else
        QGraphicsRectItem::mouseReleaseEvent(event);
}





