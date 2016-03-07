/**
 * \file       jpsconnection.h 
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

     
#ifndef JPSCONNECTION_H
#define JPSCONNECTION_H

#include <memory>
#include <utility>
#include "jpswaypoint.h"

using ptrWaypoint = std::shared_ptr<jpsWaypoint>;


class jpsConnection
{
using ptrConnection = std::shared_ptr<jpsConnection>;
public:
    jpsConnection(ptrWaypoint waypoint1, ptrWaypoint waypoint2, const int& frameID=-1);
    ~jpsConnection();
    std::pair<ptrWaypoint,ptrWaypoint> GetWaypoints() const;
    //Occurence
    bool OccursInFrame(const int &frameID) const;
    void SetFirstFrame(const int &frameID);
    void SetLastFrame(const int &frameID);
    const int& GetFirstFrame() const;
    const int& GetLastFrame() const;
    // Operator overload
    bool operator ==(const ptrConnection con2) const;
private:
    ptrWaypoint _waypoint1;
    ptrWaypoint _waypoint2;
    int _firstFrame;
    int _lastFrame;
};

#endif // JPSCONNECTION_H
