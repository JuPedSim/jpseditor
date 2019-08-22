/**
 * \file        jpsObstacle.cpp
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
 * This class is representing an obstacle in a certain room of the building. (Table, shelf, ...)
 *
 **/

#include "jpsobstacle.h"

jpsObstacle::jpsObstacle(int id): JPSZone(id, nullptr, NotAssigned)
{
    QString string="Obstacle ";
    string.append(QString::number(id));
    JPSZone::setName(string);
    _room = nullptr;
}

void jpsObstacle::set_room(JPSZone *room)
{
    _room=room;
}

JPSZone* jpsObstacle::get_room()
{
    return _room;
}

