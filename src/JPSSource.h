/*
 *
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
 *
 **/

#ifndef JPSEDITOR_JPSSOURCE_H
#define JPSEDITOR_JPSSOURCE_H

#include <QBrush>
#include <QGraphicsRectItem>
#include <QPen>
#include "graphicscene.h"
#include "elementtypes.h"


class QGraphicsSceneMouseEvent;
class QKeyEvent;

class JPSSource : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

#ifdef ANIMATE_ALIGNMENT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
#endif
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(double angle READ angle WRITE setAngle)
    Q_PROPERTY(double shearHorizontal READ shearHorizontal
                       WRITE setShearHorizontal)
    Q_PROPERTY(double shearVertical READ shearVertical
                       WRITE setShearVertical)

public:
    explicit JPSSource(QGraphicsRectItem *sourceRectItem = nullptr);

    ~JPSSource();

    enum {Type = SourceElementType};

    int type() const { return Type; }

    int getId() const;

    void setId(int id);

    const QString &getFrequency() const;

    void setFrequency(const QString &frequency);

    const QString &getN_create() const;

    void setN_create(const QString &N_create);

    const QString &getPercent() const;

    void setPercent(const QString &percent);

    const QString &getRate() const;
    void setRate(const QString &rate);

    const QString &getTime_min() const;

    void setTime_min(const QString &time_min);

    const QString &getTime_max() const;

    void setTime_max(const QString &time_max);

    const QString &getAgents_max() const;

    void setAgents_max(const QString &agents_max);

    const QString &getGroup_id() const;

    void setGroup_id(const QString &group_id);

    const QString &getCaption() const;

    void setCaption(const QString &caption);

    const QString &getGreedy() const;

    void setGreedy(const QString &greedy);

    const QString &getTime() const;

    void setTime(const QString &time);

    const QString &getStartX() const;

    void setStartX(const QString &startX);

    const QString &getStartY() const;

    void setStartY(const QString &startY);

    qreal getX_min() const;

    void setX_min(qreal x_min);

    qreal getX_max() const;

    void setX_max(qreal x_max);

    qreal getY_min() const;

    void setY_min(qreal y_min);

    qreal getY_max() const;

    void setY_max(qreal y_max);

    bool isBeSaved() const;

    void setBeSaved(bool beSaved);



    double angle() const { return m_angle; }
    double shearHorizontal() const { return m_shearHorizontal; }
    double shearVertical() const { return m_shearVertical; }

signals:
    void dirty();

public slots:
    //hide non-virtual funcs from QAbstractGraphicsShapeItem
    void setPen(const QPen &pen) ;
    void setBrush(const QBrush &brush);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget);
    void setAngle(double angle);
    void setShearHorizontal(double shearHorizontal)
    { setShear(shearHorizontal, m_shearVertical); }
    void setShearVertical(double shearVertical)
    { setShear(m_shearHorizontal, shearVertical); }
    void setShear(double shearHorizontal, double shearVertical);

protected:
    QVariant itemChange(GraphicsItemChange change,const QVariant &value);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    //JuPedSim attributes
    int id;
    QString frequency;
    QString N_create;
    QString percent;
    QString rate;
    QString time_min;
    QString time_max;
    QString agents_max;
    QString group_id;
    QString caption;
    QString greedy;
    QString time;
    QString startX;
    QString startY;
    qreal x_min;
    qreal x_max;
    qreal y_min;
    qreal y_max;
    bool beSaved;

    //QGraphicsItem attributes
    QPen currentPen;
    void updateTransform();
    bool m_resizing;
    double m_angle;
    double m_shearHorizontal;
    double m_shearVertical;
    QGraphicsRectItem *rectItem;
};



#endif //JPSEDITOR_JPSSOURCE_H
