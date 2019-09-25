/**
 * \file        jpscrossing.cpp
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
 * This class is representing a crossing to another room in the building.
 *
 **/

#include <iostream>
#include "src/jpscrossing.h"
#include <QDebug>
#include <QMessageBox>

jpsCrossing::jpsCrossing(jpsLineItem *line) : roomList(QList<JPSZone *>())
{
    qDebug("Enter jpsCrossing::jpsCrossing");
    cId = 0;
    cLine=line;
    _elevation = 0;
    qDebug("Leave jpsCrossing::jpsCrossing");
}

QList<JPSZone *> jpsCrossing::get_roomList()
{
    qDebug("Enter/Return jpsCrossing::get_roomList");
    return roomList;
}

QString jpsCrossing::get_name()
{
    qDebug("Enter/Retrun jpsCrossing::get_name");
    return cName;
}

int jpsCrossing::get_id()
{
    qDebug("Enter/Return jpsCrossing::get_id");
    return cId;
}

void jpsCrossing::set_id(int id)
{
    qDebug("Enter jpsCrossing::set_id");
    cId=id;
    qDebug("Leave jpsCrossing::set_id");
}

jpsLineItem *jpsCrossing::get_cLine()
{
    qDebug("Enter/Return jpsCrossing::get_cLine");
    return cLine;
}

void jpsCrossing::change_name(QString name)
{
    qDebug("Enter jpsCrossing::change_name");
    cName=name;
    qDebug("Leave jpsCrossing::change_name");
}

bool jpsCrossing::isState() const {
    qDebug("Enter/Return jpsCrossing::isState");
    return state;
}

void jpsCrossing::setState(bool state) {
    qDebug("Enter jpsCrossing::setState");
    jpsCrossing::state = state;
    qDebug("Leave jpsCrossing::setState");
}

QString jpsCrossing::getMaxAgents() const {
    qDebug("Enter/Return jpsCrossing::getMaxAgents");
    return max_agents;
}

void jpsCrossing::setMaxAgents(QString maxAgents) {
    qDebug("Enter jpsCrossing::setMaxAgents");
    max_agents = maxAgents;
    qDebug("Leave jpsCrossing::setMaxAgents");
}

QString jpsCrossing::getOutflow() const {
    qDebug("Enter/Return jpsCrossing::getOutflow");
    return outflow;
}

void jpsCrossing::setOutflow(QString outflow) {
    qDebug("Enter jpsCrossing::setOutflow");
    jpsCrossing::outflow = outflow;
    qDebug("Leave jpsCrossing::setOutflow");
}

void jpsCrossing::add_rooms(JPSZone *subroom1, JPSZone *subroom2)
{
     qDebug("Enter jpsCrossing::add_rooms");
     roomList.clear();

     if(subroom1 != nullptr)
     {
         qDebug("\t room1 = <%s> of type <%s>",
                subroom1->getName().toStdString().c_str(),
                QString(subroom1->getType()).toStdString().c_str());

         if(subroom1->getType() != Stair)  // assuming a crossing can not separate two stairs
             this->set_elevation(subroom1->get_elevation());

         roomList.push_back(subroom1);
         subroom1->addInEnterAndExitList(this);
     }

    if (subroom2 != nullptr)
    {
         qDebug("\t room2 = <%s> of type <%s>",
                subroom2->getName().toStdString().c_str(),
                QString(subroom2->getType()).toStdString().c_str());
         if(subroom2->getType() != Stair)
              this->set_elevation(subroom2->get_elevation());
        roomList.push_back(subroom2);
        subroom2->addInEnterAndExitList(this);
    }
    qDebug("Leave jpsCrossing::add_rooms");
}

void jpsCrossing::setSubroom(JPSZone *subroom)
{
    qDebug("Enter jpsCrossing::setSubroom");
    if(!roomList.contains(subroom))
        roomList.append(subroom);
    qDebug("Leave jpsCrossing::setSubroom");
}

void jpsCrossing::RemoveRoom(JPSZone *room)
{
    qDebug("Enter jpsCrossing::RemoveRoom");
    for (JPSZone* myRoom : roomList)
    {
        if (room==myRoom)
        {
            roomList.removeOne(room);
            break;
        }
    }
    qDebug("Leave jpsCrossing::RemoveRoom");
}
float jpsCrossing::get_elevation()
{
    qDebug("Enter/Return jpsCrossing::get_elevation");
     return _elevation;
}

void jpsCrossing::set_elevation(float elevation)
{
    qDebug("Enter/Retrun jpsCrossing::get_elevation");
     _elevation = elevation;
}
