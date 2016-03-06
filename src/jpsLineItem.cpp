/**
 * \file        jpsLineItem.cpp
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
 * This class is representing drawn line in the drawing section.
 *
 **/

#include "jpsLineItem.h"
#include <iostream>

jpsLineItem::jpsLineItem(QGraphicsLineItem *line)
{
    mLine=line;
}

jpsLineItem::~jpsLineItem()
{

}

QGraphicsLineItem *jpsLineItem::get_line()
{
    return mLine;
}

void jpsLineItem::set_line(QGraphicsLineItem *line)
{
    mLine=line;
}

int jpsLineItem::get_id()
{
    return mId;
}

QString jpsLineItem::get_name()
{
    return mName;
}

QString jpsLineItem::GetType()
{
    if (is_Door())
        return "Door";
    else if (is_Wall())
        return "Wall";
    else if (IsHLine())
        return "HLine";
}

void jpsLineItem::set_id(int id)
{
    mId=id;
}

void jpsLineItem::set_name(QString name)
{
    mName=name;
}

void jpsLineItem::set_type(bool wall, bool door, bool exit, bool hline)
{
    if (wall==true)
    {
        set_Wall();
    }
    else if (door==true)
    {
        set_Door();
    }
    else if (exit==true)
    {
        set_Exit();
    }
    else
        SetHLine();
}

void jpsLineItem::set_Wall()
{
    wall=true;
    door=false;
    exit=false;
    _hLine=false;
    defaultColor="black";
}

void jpsLineItem::set_Door()
{
    door=true;
    wall=false;
    exit=false;
    _hLine=false;
    defaultColor="blue";
}

void jpsLineItem::set_Exit()
{
    exit=true;
    wall=false;
    door=false;
    _hLine=false;
    defaultColor="darkMagenta";

}

void jpsLineItem::SetHLine()
{
    exit=false;
    wall=false;
    door=false;
    _hLine=true;
    defaultColor="darkCyan";

}

void jpsLineItem::set_defaultColor(QString color)
{
    defaultColor=color;
}

bool jpsLineItem::is_Wall()
{
    return wall;
}

bool jpsLineItem::is_Door()
{
    return door;
}

bool jpsLineItem::is_Exit()
{
    return exit;
}

bool jpsLineItem::IsHLine()
{
    return _hLine;
}

void jpsLineItem::add_intersectionPoint(QPointF *point)
{
    intersectionVector.push_back(point);
}

void jpsLineItem::remove_intersectionPoint(QPointF *point)
{
    intersectionVector.removeOne(point);
}

QList<QPointF *> jpsLineItem::get_intersectionVector()
{
    return intersectionVector;
}

QList<jpsLineItem *> jpsLineItem::get_intersectLineVector()
{
    return intersectLineVector;
}

void jpsLineItem::add_intersectLine(jpsLineItem *lineItem)
{
    intersectLineVector.push_back(lineItem);
}

void jpsLineItem::remove_interLine(jpsLineItem *lineItem)
{
    intersectLineVector.removeOne(lineItem);
}

QString jpsLineItem::get_defaultColor()
{
    return defaultColor;
}




