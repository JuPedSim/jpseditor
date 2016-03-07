/**
 * \file       jpsconnection.cpp 
 * \date        Mar 07, 2016
 * \version     v0.8.1
 * \copyright   <2009-2016> Forschungszentrum Jülich GmbH. All rights reserved.
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
 **/

     
#include "jpsconnection.h"

jpsConnection::jpsConnection(ptrWaypoint waypoint1, ptrWaypoint waypoint2, const int &frameID)
{
    _waypoint1=waypoint1;
    _waypoint2=waypoint2;
    _firstFrame=frameID;
    _lastFrame=frameID;
}

jpsConnection::~jpsConnection()
{

}

std::pair<ptrWaypoint, ptrWaypoint> jpsConnection::GetWaypoints() const
{
    return std::make_pair(_waypoint1,_waypoint2);
}

bool jpsConnection::OccursInFrame(const int& frameID) const
{
    if (frameID>=_firstFrame && frameID<=_lastFrame)
    {
        return true;
    }
    else
        return false;
}

void jpsConnection::SetFirstFrame(const int &frameID)
{
    _firstFrame=frameID;
}

void jpsConnection::SetLastFrame(const int &frameID)
{
    _lastFrame=frameID;
}

const int &jpsConnection::GetFirstFrame() const
{
    return _firstFrame;
}

const int &jpsConnection::GetLastFrame() const
{
    return _lastFrame;
}

bool jpsConnection::operator == (const ptrConnection con2) const
{
    if ((con2->GetWaypoints().first==_waypoint1 && con2->GetWaypoints().second==_waypoint2)
            || (con2->GetWaypoints().second==_waypoint1 && con2->GetWaypoints().first==_waypoint2))
    {
        return true;
    }
    else
        return false;
}

