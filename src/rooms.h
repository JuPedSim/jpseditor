/**
 * \file        rooms.h
 * \date        Oct-01-2018
 * \version     v0.8.4
 * \copyright   <2009-2018> Forschungszentrum Jülich GmbH. All rights reserved.
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
    void addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon=0);
    void addinnerWall(jpsLineItem* newWall, int id_polygon=0);
    void removeWall(QList<jpsLineItem *> wall);
    const QList<jpsCrossing *> &GetDoors() const;
    void removeDoor(jpsCrossing* crossing);
    QString get_name();
    void change_name(QString name);
    QList<jpsLineItem*> get_listWalls();
    void activate();
    //QVector<QPointF> get_vertices() const;
    void set_id(int id_room);
    void set_ax(float ax);
    void set_by(float by);
    void set_cz(float cz);
    void set_up(QPointF up);
    void set_down(QPointF down);
    float get_ax();
    float get_by();
    float get_cz();
    QPointF get_up();
    QPointF get_down();
    int get_id();
    QPointF get_center();
    void highlight(const QString &color="random");
    QString get_type() const;
    void set_type(const QString &string);
    QList<QPointF> GetDoorVertices() const;

    void AddDoor(jpsCrossing *door);
    void AddInnerDoor(jpsCrossing *door, int id_polygon=0);
    QVector<QPointF> RoomAsSortedPolygon(const QVector<QLineF> &lines) const;
    const QVector<QLineF>& GetOuterPolygon() const;
    qreal GetArea() const;
    QVector<QPointF> get_vertices() const;
    void IdentifyInnerOuter();

    // calculate attributes
    QRectF CalculateBoundingBox() const;
    qreal CalculateArea(const QVector<QLineF> &poly) const;

    float get_elevation();
    void set_elevation(float elevation);
    void correctPlaneCoefficients(); /// calculates the coefficients A_x, B_y and C_z for stairs
    bool is_highlighted();
    bool getVisible();
    void setVisible(bool visibility);

private:
    int id;
    bool highlighted;
    float A_x;
    float B_y;
    float C_z;
    QPointF _up;
    QPointF _down;
    QString name;
    QList<jpsLineItem*> wall_list;
    QVector<QLineF> outer_polygon;
    QVector<QVector<QLineF>> inner_polygons;
//    QVector<QPointF> sorted_polygon;
    QString _type;
    QList<jpsCrossing* > _doorList;
    qreal _area;
    bool visible;

    float _elevation; /// this makes only sense for horizontal rooms. 
};

bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps=0.001);

QColor rndColors(int seed);

#endif // ROOMS_H
