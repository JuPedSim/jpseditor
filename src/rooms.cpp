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
#ifdef TRACE_LOGGING
#include "dtrace.h"
#endif


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
    _elevation=0;
}

void jpsRoom::addWall(QList <jpsLineItem *> newWalls)
{
    dtrace("Enter jpsRoom::removeWall(newWall)");     
    for (int i=0; i<newWalls.size(); i++)
    {
        if (newWalls[i]->is_Wall() && !item_list.contains(newWalls[i]))
            item_list.push_back(newWalls[i]);
    }
    dtrace("Leave jpsRoom::removeWall(newWall)");
}

void jpsRoom::addWall(jpsLineItem *newWall)
{
    item_list.push_back(newWall);
}

void jpsRoom::removeWall(QList <jpsLineItem *> wall)
{
     dtrace("Enter jpsRoom::removeWall(wall)");   
    for (int i=0; i<wall.size(); i++)
    {
        item_list.removeOne(wall[i]);
    }
     dtrace("Leave jpsRoom::removeWall");
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
     dtrace("Enter jpsRoom::highlight. highlight=<%d>", highlight);
     QPen pen = QPen(Qt::black, 5);
     pen.setCosmetic(true);

    if (!highlighted)
    {
         pen.setColor(Qt::darkGreen);
        for (int i=0; i<item_list.size(); i++)
        {
            item_list[i]->get_line()->setPen(pen);
            item_list[i]->set_defaultColor("darkGreen");
        }
        highlighted=true;
    }
    else
    {   
         pen.setWidth(4);
        for (int i=0; i<item_list.size(); i++)
        {             
             item_list[i]->get_line()->setPen(pen);
            item_list[i]->set_defaultColor("black");
        }
        highlighted=false;
    }
    dtrace("Enter jpsRoom::highlight. highlight=<%d>", highlight);
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

QPointF jpsRoom::get_up()
{
     return _up;
}

void jpsRoom::set_up(QPointF up)
{
     _up = up;
}

QPointF jpsRoom::get_down()
{
     return _down;
}

void jpsRoom::set_down(QPointF down)
{
     _down = down;
}


float jpsRoom::get_elevation()
{
     return _elevation;
}

void jpsRoom::set_elevation(float elevation)
{
     if(this->get_type().toUpper() != "STAIR") // only for horizontal floors
          for (auto crossing: _doorList)
               crossing->set_elevation(elevation);

     _elevation = elevation;
}

void jpsRoom::correctPlaneCoefficients()
{
     dtrace("Enter correctPlaneCoefficients");
     dtrace("\t room=<%s> of type=<%s>", 
            this->get_name().toStdString().c_str(),
            this->get_type().toStdString().c_str());
    if(this->get_type().toUpper() != "STAIR")
    {
        this->set_ax(0);
        this->set_by(0);
        this->set_cz(this->get_elevation());
        return;
    }

     QPointF P1(0,0), P2(0,0), P3(0,0); /// plane is defined by three non-collinear points
     float elevation_1=0, elevation_2=0;
     P1 = _doorList[0]->get_cLine()->get_line()->line().p1();
     P2 = _doorList[0]->get_cLine()->get_line()->line().p2();
     elevation_1 = _doorList[0]->get_elevation();
     
     //from _doortList get three points with two different elevations 
     for (int i=1; i<_doorList.size(); i++)
     {
          if(_doorList[i]->get_elevation() != _doorList[0]->get_elevation()){
               P3 = _doorList[i]->get_cLine()->get_line()->line().p1();
               elevation_2 = _doorList[i]->get_elevation();
               break;
          }
     // @todo: check if the 3 points are collinear.
     }

     // variables for convenience
     float P1_x = P1.x();
     float P1_y = P1.y();
     float P1_z = elevation_1;

     float P2_x = P2.x();
     float P2_y = P2.y();
     float P2_z = elevation_1;

     float P3_x = P3.x();
     float P3_y = P3.y();
     float P3_z = elevation_2;
     dtrace("\t P1=(%.2f, %.2f, %.2f), P2=(%.2f, %.2f, %.2f), P3=(%.2f, %.2f, %.2f)",
            P1_x, P1_y, P1_z, 
            P2_x, P2_y, P2_z, 
            P3_x, P3_y, P3_z
          );
     float d = 1.0;
     // Thanks M. Osterkamp
     float c = (((1-P3_x/P1_x*1)-((P3_y-P3_x/P1_x*P1_y)/(P2_y-P2_x/P1_x*P1_y))
                 *(1-P2_x/P1_x*1))/((P3_z-P3_x/P1_x*P1_z)
                                    -((P3_y-P3_x/P1_x*P1_y)/(P2_y-P2_x/P1_x*P1_y))
                                    *(P2_z-P2_x/P1_x*P1_z)))*d;

     float b = ((1-P3_x/P1_x*1)*d-(P3_z-P3_x/P1_x*P1_z)*c)/(P3_y-P3_x/P1_x*P1_y);
     float a = (d-P1_z*c-P1_y*b)/P1_x;
     dtrace("\t a = %.2f, b = %.2f, c= %.2f", a, b, c);
     set_ax(-a/c);
     set_by(-b/c);
     set_cz(d/c);
     dtrace("\t ax = %.2f, by = %.2f, cz= %.2f", -a/c, -b/c, d/c);
     dtrace("Leave correctPlaneCoefficients");
}

bool jpsRoom::is_highlighted()
{
     return highlighted;}
