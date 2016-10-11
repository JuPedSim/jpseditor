/**
 * \file        rooms.h
 * \date        Jun 26, 2015
 * \version     v0.8.1
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
 * This class represents a (sub)room. Walls and/or doors belonging to a certain room have to defined with the help of roomwidget.
 *
 *
 **/


#ifndef ROOMS_H
#define ROOMS_H
#include <QGraphicsView>
#include "jpsLineItem.h"

class jpsCrossing;

class jpsRoom
{

public:
    jpsRoom(int id_room);
    ~jpsRoom(){}
    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void removeWall(QList<jpsLineItem *> wall);
    QString get_name();
    void change_name(QString name);
    QList<jpsLineItem*> get_listWalls();
    void activate();
    QVector<QPointF> get_vertices() const;
    void set_id(int id_room);
    int get_id();
    QPointF get_center();
    void highlight();
    QString get_type();
    void set_type(const QString &string);
    QList<QPointF> GetDoorVertices() const;
    //bool ContainsDoor(jpsLineItem* lineItem) const;
    void AddDoor(jpsCrossing *door);
    QPolygonF RoomAsPolygon() const;
    /**
     * @brief RoomAsSortedPolygon
     * @return SortedPolygon: QPolygonF
     */
    QPolygonF RoomAsSortedPolygon() const;


private:
    int id;
    bool highlighted;
    QString name;
    QList<jpsLineItem*> item_list;
    QString _type;
    QList<jpsCrossing* > _doorList;

};





#endif // ROOMS_H
