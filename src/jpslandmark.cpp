/**
 * \file        jpslandmark.cpp
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
 * This class is representing a landmark in the building which could be set by the user.
 * The usage of landmarks is not enabled in v0.7.
 *
 **/

#include "jpslandmark.h"

jpsLandmark::jpsLandmark()
{

}

jpsLandmark::jpsLandmark(QGraphicsPixmapItem* pixmap, QString name, QPointF point)
{
    _name=name;
    _pos=point;
    _room=nullptr;
    _visibility=0;
    _pixmapItem=pixmap;


}

jpsLandmark::~jpsLandmark()
{
    RemoveAllWaypoints();
}

void jpsLandmark::set_pixmap(QGraphicsPixmapItem *pixmap)
{
    _pixmapItem=pixmap;
}

void jpsLandmark::set_name(QString name)
{
    _name=name;

}

void jpsLandmark::set_pos(QPointF pos)
{
    _pos=pos;
}

void jpsLandmark::set_room(jpsRoom *room)
{
    _room=room;
}

void jpsLandmark::set_visibility(int visibility)
{
    _visibility=visibility;
}

QGraphicsPixmapItem* jpsLandmark::get_pixmap()
{
    return _pixmapItem;
}

QString jpsLandmark::get_name()
{
    return _name;
}

const QPointF &jpsLandmark::get_pos()
{
    return _pos;
}

jpsRoom *jpsLandmark::get_room()
{
    return _room;
}

int jpsLandmark::get_visibility()
{
    return _visibility;
}

void jpsLandmark::AddWaypoint(ptrWaypoint waypoint)
{
    _waypoints.push_back(waypoint);
}

void jpsLandmark::RemoveWaypoint(ptrWaypoint waypoint)
{
    delete waypoint.get();
    _waypoints.removeOne(waypoint);
}

QList<ptrWaypoint> jpsLandmark::GetWaypoints()
{
    return _waypoints;
}

void jpsLandmark::RemoveAllWaypoints()
{
    for (ptrWaypoint waypoint:_waypoints)
    {
        delete waypoint.get();
    }
    _waypoints.clear();
}

