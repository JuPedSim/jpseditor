/**
 * \file        jpscrossing.cpp
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
 * This class is representing a door to another room in the building or to the outside.
 *
 **/


#include "jpscrossing.h"


jpsCrossing::jpsCrossing(jpsLineItem *line)
{
    cLine=line;
    _isExit=false;
}

QList<jpsRoom *> jpsCrossing::get_roomList()
{
    return roomList;
}


QString jpsCrossing::get_name()
{
    return cName;
}

int jpsCrossing::get_id()
{
    return cId;
}

void jpsCrossing::set_id(int id)
{
    cId=id;
}

jpsLineItem *jpsCrossing::get_cLine()
{
    return cLine;
}

void jpsCrossing::change_name(QString name)
{
    cName=name;
}

void jpsCrossing::add_rooms(jpsRoom *room1, jpsRoom *room2)
{
    roomList.clear();
    roomList.push_back(room1);
    room1->AddDoor(this);
    if(room1->get_type() != "stair")     //  assuming a crossing can
                                         //  not separate two stairs
         this->set_elevation(room1->get_elevation());
    else
         this->set_elevation(room2->get_elevation());

    if (room2!=nullptr)
    {
        roomList.push_back(room2);
        room2->AddDoor(this);
    }


}

void jpsCrossing::SetStatExit(bool stat)
{
    _isExit=stat;
}

bool jpsCrossing::IsExit()
{
    return _isExit;
}
float jpsCrossing::get_elevation()
{
     return _elevation;
}

void jpsCrossing::set_elevation(float elevation)
{
     _elevation = elevation;
}
