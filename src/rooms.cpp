/**
 * \file        rooms.cpp
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
 * This class represents a (sub)room. Walls and/or doors belonging to a certain room have to defined with the help of roomwidget.
 *
 *
 **/

#include "rooms.h"
#include <QtGui>
#include <iostream>
#include <QGraphicsLineItem>


jpsRoom::jpsRoom(int id_room)
{
    id=id_room;
    QString string="Room ";
    name=string.append(QString::number(id));
    highlighted=false;
    _type="Not specified";

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

QList<QPointF> jpsRoom::get_vertices()
{
    QList<QPointF> vertices;

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

    return vertices;
}

QPointF jpsRoom::get_center()
{
    QList<QPointF> vertices = get_vertices();
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

bool jpsRoom::ContainsDoor(jpsLineItem *lineItem) const
{
    QList<QPointF> vertices = GetDoorVertices();

    for (QPointF vertex:vertices)
    {
        if (lineItem->get_line()->contains(vertex))
        {
            return true;
        }
    }
    return false;

}



void jpsRoom::set_id(int id_room)
{
    id=id_room;
}

int jpsRoom::get_id()
{
    return id;
}



