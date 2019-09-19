/**
 * \file        jpsexit.cpp
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
 * This class is representing an exit to the outside. This class will vanish in the nearer future since the class
 * jpscrossing will overtake the objectives of this class.
 *
 **/


#include "jpstransition.h"


jpsTransition::jpsTransition(jpsLineItem *line)
{
    cLine=line;
}

QList<JPSZone *> jpsTransition::get_roomList()
{
    return roomList;
}

QString jpsTransition::get_name()
{
    return cName;
}

jpsLineItem *jpsTransition::get_cLine()
{
    return cLine;
}

void jpsTransition::change_name(QString name)
{
    cName=name;
}

QString jpsTransition::get_type()
{
    return _type;
}

int jpsTransition::get_id()
{
    return _id;
}

void jpsTransition::set_id(int id)
{
    _id=id;
}

void jpsTransition::set_type(QString type)
{
    _type=type;
}

void jpsTransition::set_rooms(JPSZone *room1, JPSZone *room2)
{
    roomList.clear();
    roomList.push_back(room1);
    roomList.push_back(room2);
//    if (room1 != nullptr)
//    {
//        roomList.push_back(room1);
//    }
//
//    roomList.push_back(room1);
//    if (room2 != nullptr)
//    {
//        roomList.push_back(room2);
//    }
}
