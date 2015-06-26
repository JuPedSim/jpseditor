/**
 * \file        jpswaypoint.cpp
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

#include "jpswaypoint.h"

jpsWaypoint::jpsWaypoint()
{

}

jpsWaypoint::jpsWaypoint(QRectF rect)
{
    _rect=rect;
    _pos=_rect.center();
    _a=std::abs(_rect.width()/2.0);
    _b=std::abs(_rect.height()/2.0);
    _ellipseItem=nullptr;
}

jpsWaypoint::jpsWaypoint(QGraphicsEllipseItem* ellipseItem)
{
    _ellipseItem=ellipseItem;
    _rect=_ellipseItem->rect();
    _pos=_rect.center();
    _a=_rect.width();
    _b=_rect.height();
}

jpsWaypoint::~jpsWaypoint()
{

}

const QPointF &jpsWaypoint::GetPos()
{
    return _pos;

}

const QRectF &jpsWaypoint::GetRect()
{
    return _rect;
}

double jpsWaypoint::GetA()
{
    return _a;
}

double jpsWaypoint::GetB()
{
    return _b;
}

QGraphicsEllipseItem *jpsWaypoint::GetEllipseItem()
{
    return _ellipseItem;
}


void jpsWaypoint::SetPos(QPointF point)
{
    _pos=point;
    _rect.setRect(point.x(),point.y(),_rect.width(),_rect.height());
}

void jpsWaypoint::SetRect(QRect rect)
{
    _rect=rect;
    _a=_rect.width();
    _b=_rect.height();
    _pos=_rect.center();
}

void jpsWaypoint::SetEllipseItem(QGraphicsEllipseItem *ellipseItem)
{
    _ellipseItem=ellipseItem;
}




