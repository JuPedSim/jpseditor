/**
 * \file        rooms.cpp
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

#include "rooms.h"
#include <QtGui>
#include <iostream>
#include <QGraphicsLineItem>
#include "jpscrossing.h"


jpsRoom::jpsRoom(int id_room)
{
    id=id_room;
    QString string="Room ";
    name=string.append(QString::number(id));
    highlighted=false;
    _type="Not specified";
    A_x=0.0; //plane equation: A_x . x + B_y . y + C_z = z
    B_y=0.0;
    C_z=0.0;
}

void jpsRoom::addWall(QList <jpsLineItem *> newWalls)
{
    for (int i=0; i<newWalls.size(); i++)
    {
        if (newWalls[i]->is_Wall() && !item_list.contains(newWalls[i]))
            item_list.push_back(newWalls[i]);
    }

}

void jpsRoom::addWall(jpsLineItem *newWall)
{
    item_list.push_back(newWall);
}

void jpsRoom::removeWall(QList <jpsLineItem *> wall)
{
    for (int i=0; i<wall.size(); i++)
    {
        item_list.removeOne(wall[i]);
    }
}

QString jpsRoom::get_name()
{
    return name;
}

void jpsRoom::change_name(QString name)
{
    this->name=name;
}

QList<jpsLineItem *> jpsRoom::get_listWalls()
{
    return item_list;
}

void jpsRoom::activate()
{
    for (signed int i=0; i<item_list.size(); i++)
    {
        item_list[i]->get_line()->setPen(QPen(Qt::yellow));
    }
}

QVector<QPointF> jpsRoom::get_vertices() const
{
    QVector<QPointF> vertices;

    for (int i=0; i<item_list.size(); i++)
    {
        if (vertices.contains(item_list[i]->get_line()->line().p1())==false)
        {
            vertices.push_back(item_list[i]->get_line()->line().p1());
        }
        if (vertices.contains(item_list[i]->get_line()->line().p2())==false)
        {
            vertices.push_back(item_list[i]->get_line()->line().p2());
        }
    }
    for (jpsCrossing* crossing:_doorList)
    {
        if (vertices.contains(crossing->get_cLine()->get_line()->line().p1())==false)
        {
            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
        }

        if (vertices.contains(crossing->get_cLine()->get_line()->line().p1())==false)
        {
            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
        }

    }

    return vertices;
}

QPointF jpsRoom::get_center()
{
    QVector<QPointF> vertices = get_vertices();
    qreal sum_x=0;
    qreal sum_y=0;

    for (int i=0; i<vertices.size(); i++)
    {
        sum_x+=vertices[i].x();
        sum_y+=vertices[i].y();
    }
    QPointF mean;
    mean.setX(sum_x/vertices.size());
    mean.setY(sum_y/vertices.size());

    return mean;
}


void jpsRoom::highlight()
{
    if (!highlighted)
    {
        for (int i=0; i<item_list.size(); i++)
        {
            item_list[i]->get_line()->setPen(QPen(Qt::darkGreen,0));
            item_list[i]->set_defaultColor("darkGreen");
        }
        highlighted=true;
    }
    else
    {
        for (int i=0; i<item_list.size(); i++)
        {
            item_list[i]->get_line()->setPen(QPen(Qt::black,0));
            item_list[i]->set_defaultColor("black");
        }
        highlighted=false;
    }
}

QString jpsRoom::get_type()
{
    return _type;
}

void jpsRoom::set_type(const QString &string)
{
    _type=string;
}

QList<QPointF> jpsRoom::GetDoorVertices() const
{
    QList<QPointF> vertices;

    for (int i=0; i<item_list.size(); i++)
    {
        int counterP1=0;
        int counterP2=0;
        for (int j=0; j<item_list.size(); j++)
        {

            if (j!=i)
            {
                if (item_list[i]->get_line()->line().p1()==item_list[j]->get_line()->line().p1()
                        || item_list[i]->get_line()->line().p1()==item_list[j]->get_line()->line().p2())
                {
                    counterP1++;
                }
                else if (item_list[i]->get_line()->line().p2()==item_list[j]->get_line()->line().p1()
                        || item_list[i]->get_line()->line().p2()==item_list[j]->get_line()->line().p2())
                {
                    counterP2++;
                }
            }
        }
        if (counterP1==0)
        {
            if (!vertices.contains(item_list[i]->get_line()->line().p1()))
            vertices.push_back(item_list[i]->get_line()->line().p1());
        }
        else if (counterP2==0)
        {
            if (!vertices.contains(item_list[i]->get_line()->line().p2()))
            vertices.push_back(item_list[i]->get_line()->line().p2());
        }
    }

    return vertices;
}

void jpsRoom::AddDoor(jpsCrossing *door)
{
    _doorList.push_back(door);
}

QPolygonF jpsRoom::RoomAsPolygon() const
{

    return QPolygonF(get_vertices());
}

QPolygonF jpsRoom::RoomAsSortedPolygon() const
{
    QList<jpsLineItem*> lines = item_list;
    for (jpsCrossing* crossing:_doorList)
    {
        lines.push_back(crossing->get_cLine());
    }
//    std::cout << lines.size() << std::endl;
    QVector<QPointF> points;

    points.push_back(lines.first()->get_line()->line().p1());
    points.push_back(lines.first()->get_line()->line().p2());

    lines.pop_front();

    for (int i=0; i<lines.size(); i++)
    {
        if (lines[i]->get_line()->line().p1() == points.last())
        {
            //points.push_back(line->get_line()->line().p1());
            points.push_back(lines[i]->get_line()->line().p2());
            lines.removeOne(lines[i]);
            i=-1;
        }
        else if (lines[i]->get_line()->line().p2() == points.last())
        {
            //points.push_back(line->get_line()->line().p1());
            points.push_back(lines[i]->get_line()->line().p1());
            lines.removeOne(lines[i]);
            i=-1;
        }
    }


    for (QPointF point:points)
    {
        std::cout << point.x() << " " << point.y() << std::endl;

    }
    std::cout << "----------------------------" << std::endl;

    return QPolygonF(points);
}

//bool jpsRoom::ContainsDoor(jpsLineItem *lineItem) const
//{
//    QList<QPointF> vertices = GetDoorVertices();

//    for (QPointF vertex:vertices)
//    {
//        if (lineItem->get_line()->contains(vertex))
//        {
//            return true;
//        }
//    }
//    return false;

//}



void jpsRoom::set_id(int id_room)
{
    id=id_room;
}

int jpsRoom::get_id()
{
    return id;
}
float jpsRoom::get_ax()
{
     return A_x;
}
void jpsRoom::set_ax(float AX)
{
     A_x=AX;
}

float jpsRoom::get_by()
{
     return B_y;     
}
void jpsRoom::set_by(float BY)
{
     B_y=BY;
}

float jpsRoom::get_cz()
{
     return C_z;     
}
void jpsRoom::set_cz(float CZ)
{
     C_z=CZ;
}

float jpsRoom::get_up()
{
     return up;
}

void jpsRoom::set_up(_up)
{
     up = _up;
}

float jpsRoom::get_down()
{
     return down;
}

void jpsRoom::set_down(_down)
{
     down = _down;
}
