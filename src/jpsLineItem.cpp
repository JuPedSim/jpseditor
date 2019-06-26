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
    lineType = undefined;
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
    switch (lineType){
        case wall:
            return "Wall";
        case crossing:
            return "Crossing";
        case track:
            return "Track";
        case transition:
            return "Exit";
        case hline:
            return "Hline";
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

void jpsLineItem::setType(LineType type) {
    switch (type){
        case wall:
            setWall();
            break;
        case track:
            setTrack();
            break;
        case crossing:
            setCrossing();
            break;
        case hline:
            setHline();
            break;
        case transition:
            setTransition();
            break;
        default:
            break;
    }
}

void jpsLineItem::setWall()
{
    lineType = wall;
    defaultColor="black";
}

void jpsLineItem::setCrossing()
{
    lineType = crossing;
    defaultColor="darkMagenta";
}

void jpsLineItem::setHline()
{
    lineType = hline;
    defaultColor="darkCyan";
}

void jpsLineItem::setTransition()
{
    lineType = transition;
    defaultColor="darkBlue";
}

void jpsLineItem::setTrack()
{
    lineType = track;
    defaultColor="darkGreen";
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

bool jpsLineItem::is_Crossing()
{
    if(lineType == crossing)
    {
        return true;
    } else
    {
        return false;
    }
}

bool jpsLineItem::is_Transition()
{
    if(lineType == transition)
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




