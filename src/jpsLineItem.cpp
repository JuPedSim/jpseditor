/**
 * \file        jpsLineItem.cpp
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
 * This class is representing drawn line in the drawing section.
 *
 **/

#include "jpsLineItem.h"
#include <iostream>

jpsLineItem::jpsLineItem(QGraphicsLineItem *line)
{
    mLine=line;
    lineType = wall;
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
/*    if (is_Door())
        return "Door";
    else if (is_Wall())
        return "Wall";
    else if (IsHLine())
        return "HLine";
    else
        return "NOT_KNOWN";*/

    switch (lineType){
        case wall:
            return "Wall";
        case door:
            return "Door";
        case exit:
            return "Exit";
        case hline:
            return "HLine";
        case undefined:
            return "NOT_KNOWN";
    }
}

void jpsLineItem::set_id(int id)
{
    mId=id;
}

void jpsLineItem::set_name(QString name)
{
    mName=name;
}

/*void jpsLineItem::set_type(bool wall, bool door, bool exit, bool hline)
{
    if (wall==true)
    {
        setWall();
    }
    else if (door==true)
    {
        setDoor();
    }
    else if (exit==true)
    {
        setExit();
    }
    else
        setHLine();
}*/

void jpsLineItem::setType(LineType type) {
    switch (type){
        case wall:
            setWall();
            break;
        case door:
            setDoor();
            break;
        case exit:
            setExit();
            break;
        case hline:
            setHLine();
            break;
    }

}

void jpsLineItem::setWall()
{
/*    wall=true;
    door=false;
    exit=false;
    _hLine=false;*/
    defaultColor="black";
}

void jpsLineItem::setDoor()
{
//    door=true;
//    wall=false;
//    exit=false;
//    _hLine=false;
    lineType = door;
    defaultColor="blue";
}

void jpsLineItem::setExit()
{
//    exit=true;
//    wall=false;
//    door=false;
//    _hLine=false;
    lineType = exit;
    defaultColor="darkMagenta";

}

void jpsLineItem::setHLine()
{
//    exit=false;
//    wall=false;
//    door=false;
//    _hLine=true;
    lineType = hline;
    defaultColor="darkCyan";

}

void jpsLineItem::set_defaultColor(QString color)
{
    defaultColor=color;
}

bool jpsLineItem::is_Wall()
{
    if(lineType == wall)
    {
        return true;
    } else
    {
        return false;
    }

}

bool jpsLineItem::is_Door()
{
    if(lineType == door)
    {
        return true;
    } else
    {
        return false;
    }
}

bool jpsLineItem::is_Exit()
{
    if(lineType == exit)
    {
        return true;
    } else
    {
        return false;
    }
}

bool jpsLineItem::IsHLine()
{
    if(lineType == hline)
    {
        return true;
    } else
    {
        return false;
    }
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




