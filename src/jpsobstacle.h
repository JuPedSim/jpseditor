/**
 * \file        jpsObstacle.h
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
 * This class is representing an obstacle in a certain room of the building. (Table, shelf, ...)
 *
 **/

#ifndef JPSOBSTACLE_H
#define JPSOBSTACLE_H

#include "rooms.h"

class jpsObstacle: public jpsRoom
{
public:
    jpsObstacle(int cId);
    void set_room(jpsRoom* room);
    jpsRoom* get_room();
private:
    jpsRoom* _room;
};

#endif // JPSOBSTACLE_H
