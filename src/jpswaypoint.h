/**
 * \file        jpswaypoint.h
 * \date        Jun 26, 2015
 * \version     v0.7
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
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
 * This class is representing an spatial association which rely on a certain landmark.
 * jpswaypoint is not enabled in v0.7.
 *
 **/

#ifndef JPSWAYPOINT_H
#define JPSWAYPOINT_H

#include <QGraphicsEllipseItem>

class jpsWaypoint
{
public:
    jpsWaypoint();
    jpsWaypoint(QRectF rect);
    jpsWaypoint(QGraphicsEllipseItem* ellipseItem);
    ~jpsWaypoint();
    ///Getter
    const QPointF& GetPos();
    const QRectF &GetRect();
    double GetA();
    double GetB();
    QGraphicsEllipseItem* GetEllipseItem();

    ///Setter
    void SetPos(QPointF point);
    void SetRect(QRect rect);
    void SetEllipseItem(QGraphicsEllipseItem* ellipseItem);


private:
    QGraphicsEllipseItem* _ellipseItem;
    QPointF _pos;
    QRectF _rect;
    double _a;
    double _b;

};

#endif // JPSWAYPOINT_H
