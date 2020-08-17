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
    : mId(-1), mLine(line), lineType(undefined), defaultColor("black")
{
    qDebug("Enter jpsLineItem::jpsLineItem");
    qDebug("Leave jpsLineItem::jpsLineItem");
}

jpsLineItem::~jpsLineItem()
{

}

QGraphicsLineItem *jpsLineItem::get_line()
{
    qDebug("Enter/Leave jpsLineItem::get_line");
    return mLine;
}

void jpsLineItem::set_line(QGraphicsLineItem *line)
{
    qDebug("Enter jpsLineItem::set_line");
    mLine=line;
    qDebug("Leave jpsLineItem::set_line");
}

int jpsLineItem::get_id()
{
    qDebug("Enter/Return jpsLineItem::get_id");
    return mId;
}

QString jpsLineItem::get_name()
{
    qDebug("Enter/Return jpsLineItem::get_name");
    return mName;
}

QString jpsLineItem::getType()
{
    qDebug("Enter jpsLineItem::getType");
    switch (lineType){
        case wall:
            qDebug("Leave jpsLineItem::getType(wall)");
            return "wall";
        case track:
            qDebug("Leave jpsLineItem::getType(track)");
            return "track";
        case transition:
            qDebug("Leave jpsLineItem::getType(transition)");
            return "transition";
        case hline:
            qDebug("Leave jpsLineItem::getType(hline)");
            return "hline";
        case undefined:
            qDebug("Leave jpsLineItem::getType(undefined)");
            return "undefined";
        default:
            qDebug("Leave jpsLineItem::getType(invalid)");
            return "invaild";
    }
}

void jpsLineItem::set_id(int id)
{
    qDebug("Enter jpsLineItem::set_id");
    mId=id;
    qDebug("Leave jpsLineItem::set_id");
}

void jpsLineItem::set_name(QString name)
{
    qDebug("Enter jpsLineItem::set_name");
    mName=name;
    qDebug("Leave jpsLineItem::set_name");
}

void jpsLineItem::setType(LineType type) {
    qDebug("Enter jpsLineItem::setType");
    switch (type){
        case wall:
            setWall();
            break;
        case track:
            setTrack();
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
    qDebug("Leave jpsLineItem::setType");
}

void jpsLineItem::setWall()
{
    qDebug("Enter jpsLineItem::setWall");
    lineType = wall;
    defaultColor="black";
    qDebug("Leave jpsLineItem::setWall");
}


void jpsLineItem::setHline()
{
    qDebug("Enter jpsLineItem::setHline");
    lineType = hline;
    defaultColor="darkCyan";
    qDebug("Leave jpsLineItem::setHline");
}

void jpsLineItem::setTransition()
{
    qDebug("Enter jpsLineItem::setTransition");
    lineType = transition;
    defaultColor="darkBlue";
    qDebug("Leave jpsLineItem::setTransition");
}

void jpsLineItem::setTrack()
{
    qDebug("Enter jpsLineItem::setTrack");
    lineType = track;
    defaultColor="darkGreen";
    qDebug("Leave jpsLineItem::setTrack");
}

void jpsLineItem::setUndefined()
{
    qDebug("Enter jpsLineItem::setUndefined");
    lineType = undefined;
    defaultColor="black";
    qDebug("Leave jpsLineItem::setUndefined");
}

void jpsLineItem::set_defaultColor(QString color)
{
    qDebug("Enter jpsLineItem::set_defaultColor");
    defaultColor=color;
    qDebug("Leave jpsLineItem::set_defaultColor");
}

bool jpsLineItem::is_Wall()
{
    qDebug("Enter jpsLineItem::is_Wall");
    if(lineType == wall)
    {
        qDebug("Leave jpsLineItem::is_Wall(true)");
        return true;
    } else
    {
        qDebug("Leave jpsLineItem::is_Wall(flase)");
        return false;
    }

}


bool jpsLineItem::is_Transition()
{
    qDebug("Enter jpsLineItem::is_Transition");
    if(lineType == transition)
    {
        qDebug("Leave jpsLineItem::is_Transition(true)");
        return true;
    } else
    {
        qDebug("Leave jpsLineItem::is_Transition(false)");
        return false;
    }
}

bool jpsLineItem::IsHLine()
{
    qDebug("Enter jpsLineItem::IsHLine");
    if(lineType == hline)
    {
        qDebug("Leave jpsLineItem::IsHLine(true)");
        return true;
    } else
    {
        qDebug("Leave jpsLineItem::IsHLine(false)");
        return false;
    }
}

void jpsLineItem::add_intersectionPoint(QPointF *point)
{
    qDebug("Enter jpsLineItem::add_intersectionPoint");
    intersectionVector.push_back(point);
    qDebug("Leave jpsLineItem::add_intersectionPoint");
}

void jpsLineItem::remove_intersectionPoint(QPointF *point)
{
    qDebug("Enter jpsLineItem::remove_intersectionPoint");
    intersectionVector.removeOne(point);
    qDebug("Leave jpsLineItem::remove_intersectionPoint");
}

QList<QPointF *> jpsLineItem::get_intersectionVector()
{
    qDebug("Enter/Return jpsLineItem::get_intersectionVector");
    return intersectionVector;
}

QList<jpsLineItem *> jpsLineItem::get_intersectLineVector()
{
    qDebug("Enter/Return jpsLineItem::get_intersectLineVector");
    return intersectLineVector;
}

void jpsLineItem::add_intersectLine(jpsLineItem *lineItem)
{
    qDebug("Enter jpsLineItem::add_intersectLine");
    intersectLineVector.push_back(lineItem);
    qDebug("Return jpsLineItem::add_intersectLine");
}

void jpsLineItem::remove_interLine(jpsLineItem *lineItem)
{
    qDebug("Enter jpsLineItem::remove_interLine");
    intersectLineVector.removeOne(lineItem);
    qDebug("Leave jpsLineItem::remove_interLine");
}

QString jpsLineItem::get_defaultColor()
{
    qDebug("Enter/Leave jpsLineItem::get_defaultColor");
    return defaultColor;
}
