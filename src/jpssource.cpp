/**
 * \file       jpssource.cpp
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
 *
 * \section Description
 * This class define a element - source - in JuPedSim.
 * It contains a GraphicsRectItem as geometry, and others attributes for jpscore
 *
 **/

#include "jpssource.h"
#include "global.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QStyle>
#include <QStyleOption>

JPSSource::JPSSource(QGraphicsRectItem *sourceRectItem)
    : QObject(), QGraphicsRectItem(), m_resizing(false),
      m_angle(0.0), m_shearHorizontal(0.0), m_shearVertical(0.0)
{
    qDebug("Enter JPSSource::JPSSource");
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable);

    //take over memory that point to currentSource,
    //because currentSource will be deleted after instantiation of JPSSource.
    rectItem = sourceRectItem;

    //Set-up rectangle
    setRect(rectItem->rect());

    //Set-up default values of a JPSsource
    id = 0;
    caption="Source";
    agents_max = "";
    frequency = "";
    group_id = "";
    N_create = "";
    percent = "";
    rate = "";
    startX = "";
    startY = "";
    time = "";
    time_max = "";
    time_min = "";
    greedy = "";
    beSaved = true;

    x_min = this->rect().bottomLeft().x();
    y_min = this->rect().bottomLeft().y();
    x_max = this->rect().topRight().x();
    y_max = this->rect().topRight().y();

    qDebug("Leave JPSSource::JPSSource");
}

JPSSource::~JPSSource()
= default;

void JPSSource::setId(int id) {
    qDebug("Enter JPSSource::setId");
    JPSSource::id = id;
    qDebug("Leave JPSSource::setId");
}

const QString &JPSSource::getFrequency() const {
    qDebug("Enter/Return JPSSource::getFrequency");
    return frequency;
}

void JPSSource::setFrequency(const QString &frequency) {
    qDebug("Enter JPSSource::setFrequency");
    JPSSource::frequency = frequency;
    qDebug("Leave JPSSource::setFrequency");
}

const QString &JPSSource::getN_create() const {
    qDebug("Enter/Return JPSSource::getN_create");
    return N_create;
}

void JPSSource::setN_create(const QString &N_create) {
    qDebug("Enter JPSSource::setN_create");
    JPSSource::N_create = N_create;
    qDebug("Leave JPSSource::setN_create");
}

const QString &JPSSource::getPercent() const {
    qDebug("Enter/Return QString &JPSSource::getPercent");
    return percent;
}

void JPSSource::setPercent(const QString &percent) {
    qDebug("Enter JPSSource::setPercent");
    JPSSource::percent = percent;
    qDebug("Leave JPSSource::setPercent");
}

const QString &JPSSource::getRate() const {
    qDebug("Enter/Return JPSSource::getRate");
    return rate;
}

void JPSSource::setRate(const QString &rate) {
    qDebug("Enter JPSSource::setRate");
    JPSSource::rate = rate;
    qDebug("Leave JPSSource::setRate");
}

const QString &JPSSource::getTime_min() const {
    qDebug("Enter/Retrun JPSSource::getTime_min");
    return time_min;
}

void JPSSource::setTime_min(const QString &time_min) {
    qDebug("Enter JPSSource::setTime_min");
    JPSSource::time_min = time_min;
    qDebug("Leave JPSSource::setTime_min");
}

const QString &JPSSource::getTime_max() const {
    qDebug("Enter/Return JPSSource::getTime_max");
    return time_max;
}

void JPSSource::setTime_max(const QString &time_max) {
    qDebug("Enter JPSSource::setTime_max");
    JPSSource::time_max = time_max;
    qDebug("Leave JPSSource::setTime_max");
}

const QString &JPSSource::getAgents_max() const {
    qDebug("Enter/Return JPSSource::getAgents_max");
    return agents_max;
}

void JPSSource::setAgents_max(const QString &agents_max) {
    qDebug("Enter JPSSource::setAgents_max");
    JPSSource::agents_max = agents_max;
    qDebug("Leave JPSSource::setAgents_max");
}

const QString &JPSSource::getGroup_id() const {
    qDebug("Enter/Return JPSSource::getGroup_id");
    return group_id;
}

void JPSSource::setGroup_id(const QString &group_id) {
    qDebug("Enter JPSSource::setGroup_id");
    JPSSource::group_id = group_id;
    qDebug("Leave JPSSource::setGroup_id");
}

const QString &JPSSource::getCaption() const{
    qDebug("Enter/Leave QString &JPSSource::getCaption");
    return caption;
}

void JPSSource::setCaption(const QString &caption) {
    qDebug("Enter JPSSource::setCaption");
    JPSSource::caption = caption;
    qDebug("Leave JPSSource::setCaption");
}

const QString &JPSSource::getGreedy() const {
    qDebug("Enter/Return JPSSource::getGreedy");
    return greedy;
}

void JPSSource::setGreedy(const QString &greedy) {
    qDebug("Enter JPSSource::setGreedy");
    JPSSource::greedy = greedy;
    qDebug("Leave JPSSource::setGreedy");
}

const QString &JPSSource::getTime() const {
    qDebug("Enter/Return JPSSource::getTime");
    return time;
}

void JPSSource::setTime(const QString &time) {
    qDebug("Enter JPSSource::setTime");
    JPSSource::time = time;
    qDebug("Leave JPSSource::setTime");
}

const QString &JPSSource::getStartX() const {
    qDebug("Enter/Return JPSSource::getStartX");
    return startX;
}

void JPSSource::setStartX(const QString &startX) {
    qDebug("Enter JPSSource::setStartX");
    JPSSource::startX = startX;
    qDebug("Leave JPSSource::setStartX");
}

const QString &JPSSource::getStartY() const {
    qDebug("Enter/Return JPSSource::getStartY");
    return startY;
}

void JPSSource::setStartY(const QString &startY) {
    qDebug("Enter JPSSource::setStartY");
    JPSSource::startY = startY;
    qDebug("Leave JPSSource::setStartY");
}

qreal JPSSource::getX_min() const {
    qDebug("Enter/Return JPSSource::getX_min");
    return x_min;
}

void JPSSource::setX_min(qreal x_min) {
    qDebug("Enter JPSSource::setX_min");
    JPSSource::x_min = x_min;
    qDebug("Leave JPSSource::setX_min");
}

qreal JPSSource::getX_max() const {
    qDebug("Enter/Return JPSSource::getX_max");
    return x_max;
}

void JPSSource::setX_max(qreal x_max) {
    qDebug("Enter JPSSource::setX_max");
    JPSSource::x_max = x_max;
    qDebug("Leave JPSSource::setX_max");
}

qreal JPSSource::getY_min() const {
    qDebug("Enter/Return JPSSource::getY_min");
    return y_min;
}

void JPSSource::setY_min(qreal y_min) {
    qDebug("Enter JPSSource::setY_min");
    JPSSource::y_min = y_min;
    qDebug("Leave JPSSource::setY_min");
}

qreal JPSSource::getY_max() const {
    qDebug("Enter/Return JPSSource::getY_max");
    return y_max;
}

void JPSSource::setY_max(qreal y_max) {
    qDebug("Enter JPSSource::setY_max");
    JPSSource::y_max = y_max;
    qDebug("Leave JPSSource::setY_max");
}

int JPSSource::getId() const {
    qDebug("Enter/Return JPSSource::getId");
    return id;
}

bool JPSSource::isBeSaved() const {
    qDebug("Enter/Return JPSSource::isBeSaved");
    return beSaved;
}

void JPSSource::setBeSaved(bool beSaved) {
    qDebug("Enter JPSSource::setBeSaved");
    JPSSource::beSaved = beSaved;
    qDebug("Leave JPSSource::setBeSaved");
}

// For drawing
void JPSSource::setPen(const QPen &pen_)
{
    qDebug("Enter JPSSource::setPen");
    if (isSelected() && pen_ != pen()) {
        QGraphicsRectItem::setPen(pen_);
        emit dirty();
    }
    qDebug("Leave JPSSource::setPen");
}


void JPSSource::setBrush(const QBrush &brush_)
{
    qDebug("Enter JPSSource::setBrush");
    if (isSelected() && brush_ != brush()) {
        QGraphicsRectItem::setBrush(brush_);
        emit dirty();
    }
    qDebug("Leave JPSSource::setBrush");
}


void JPSSource::setAngle(double angle)
{
    qDebug("Enter JPSSource::setAngle");
    if (isSelected() && !qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        updateTransform();
    }
    qDebug("Leave JPSSource::setAngle");
}


void JPSSource::setShear(double shearHorizontal, double shearVertical)
{
    qDebug("Enter JPSSource::setShear");
    if (isSelected() &&
        (!qFuzzyCompare(m_shearHorizontal, shearHorizontal) ||
         !qFuzzyCompare(m_shearVertical, shearVertical))) {
        m_shearHorizontal = shearHorizontal;
        m_shearVertical = shearVertical;
        updateTransform();
    }
    qDebug("Leave JPSSource::setShear");
}


void JPSSource::updateTransform()
{
    qDebug("Enter JPSSource::updateTransform");
    QTransform transform;
    transform.shear(m_shearHorizontal, m_shearVertical);
    transform.rotate(m_angle);
    setTransform(transform);
    qDebug("Leave JPSSource::updateTransform");
}


QVariant JPSSource::itemChange(GraphicsItemChange change,
                             const QVariant &value)
{
    qDebug("Enter JPSSource::itemChange");
    if (isDirtyChange(change))
            emit dirty();
    qDebug("Leave JPSSource::itemChange");
    return QGraphicsRectItem::itemChange(change, value);
}


void JPSSource::keyPressEvent(QKeyEvent *event)
{
    qDebug("Enter JPSSource::keyPressEvent");
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
    qDebug("Leave JPSSource::keyPressEvent");
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


/*
    since 0.8.8

    Hide paint() in father class QGraphicsRectItem, change the style of rectangle.
*/
void JPSSource::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget * /* widget */)
{
    QPen pen(Qt::darkRed,0);

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

QGraphicsRectItem *JPSSource::getRectItem() const
{
    qDebug("Enter/Leave JPSSource::getRectItem()");
    return rectItem;
}
