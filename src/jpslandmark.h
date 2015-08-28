/**
 * \file        jpslandmark.h
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

#ifndef JPSLANDMARK_H
#define JPSLANDMARK_H
#include <QPointF>
#include "rooms.h"
#include "jpswaypoint.h"


class jpsLandmark
{
public:
    jpsLandmark();
    jpsLandmark(QGraphicsPixmapItem* pixmap, QString name, QPointF point);
    ~jpsLandmark();
    //Setter
    void set_pixmap(QGraphicsPixmapItem* pixmap);
    void set_name(QString name);
    void set_pos(QPointF pos);
    void set_room(jpsRoom* room);
    void set_visibility(int visibility);
    //Getter
    QGraphicsPixmapItem* get_pixmap();
    QString get_name();
    const QPointF& get_pos();
    jpsRoom *get_room();
    int get_visibility();
    //Waypoint handling
    void AddWaypoint(jpsWaypoint* waypoint);
    void RemoveWaypoint(jpsWaypoint* waypoint);
    QList<jpsWaypoint* > GetWaypoints();
    void RemoveAllWaypoints();


private:
    QString _name;
    QPointF _pos;
    jpsRoom* _room;
    int _visibility;
    QGraphicsPixmapItem* _pixmapItem;
    QList<jpsWaypoint* > _waypoints;

};

#endif // JPSLANDMARK_H
