/**
 * \file       jpsconnection.cpp 
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
 **/

     
#include "jpsconnection.h"

jpsConnection::jpsConnection(jpsLandmark* landmark1, jpsLandmark* landmark2, QGraphicsLineItem *lineItem, const int &frameID)
{
    _landmark1=landmark1;
    _landmark2=landmark2;
    _firstFrame=frameID;
    _lastFrame=frameID;
    _lineItem=lineItem;
}

jpsConnection::~jpsConnection()
{

}

std::pair<jpsLandmark*, jpsLandmark*> jpsConnection::GetLandmarks() const
{
    return std::make_pair(_landmark1,_landmark2);
}

//bool jpsConnection::OccursInFrame(const int& frameID) const
//{
//    if (frameID>=_firstFrame && frameID<=_lastFrame)
//    {
//        return true;
//    }
//    else
//        return false;
//}

//void jpsConnection::SetFirstFrame(const int &frameID)
//{
//    _firstFrame=frameID;
//}

//void jpsConnection::SetLastFrame(const int &frameID)
//{
//    _lastFrame=frameID;
//}

//const int &jpsConnection::GetFirstFrame() const
//{
//    return _firstFrame;
//}

//const int &jpsConnection::GetLastFrame() const
//{
//    return _lastFrame;
//}

bool jpsConnection::operator == (const jpsConnection& con2) const
{
    if ((con2.GetLandmarks().first==_landmark1 && con2.GetLandmarks().second==_landmark2)
            || (con2.GetLandmarks().second==_landmark1 && con2.GetLandmarks().first==_landmark2))
    {
        return true;
    }
    else
        return false;
}

void jpsConnection::SetLineItem(QGraphicsLineItem *lineItem)
{
    _lineItem=lineItem;
}

QGraphicsLineItem *jpsConnection::GetLineItem()
{
    return _lineItem;
}

void jpsConnection::SetFirstLandmark(jpsLandmark* landmark)
{
    _landmark1=landmark;
}

void jpsConnection::SetSecondLandmark(jpsLandmark* landmark)
{
    _landmark2=landmark;
}



