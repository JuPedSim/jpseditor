/**
 * \file       jpsyahpointer.cpp 
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

     
#include "jpsyahpointer.h"

jpsYAHPointer::jpsYAHPointer(const QPointF &point, const qreal &angle)
{
    _cPos=point;
    _cDirection=angle;
    _allPos.push_back(point);
    _allDir.push_back(angle);
}

jpsYAHPointer::jpsYAHPointer(const qreal &x, const qreal &y, const qreal &angle)
{
    _cPos.setX(x);
    _cPos.setY(y);
    _cDirection=angle;
    _allPos.push_back(_cPos);
    _allDir.push_back(angle);
}

jpsYAHPointer::~jpsYAHPointer()
{

}

const QPointF &jpsYAHPointer::GetPos() const
{
    return _cPos;
}

const qreal &jpsYAHPointer::GetDirection() const
{
    return _cDirection;
}

void jpsYAHPointer::SetPos(const QPointF &point)
{
    _cPos=point;
    _allPos.push_back(point);
}

void jpsYAHPointer::SetDirection(const qreal &angle)
{
    _cDirection=angle;
    _allDir.push_back(angle);
}


const int &jpsYAHPointer::GetFirstFrame() const
{
    return _firstFrame;
}

const int &jpsYAHPointer::GetLastFrame() const
{
    return _lastFrame;
}

void jpsYAHPointer::SetFirstFrame(const int &frame)
{
    _firstFrame=frame;
}

void jpsYAHPointer::SetLastFrame(const int &frame)
{
    _lastFrame=frame;
}

const QPointF &jpsYAHPointer::GetPosWhenFrame(const int &frame)
{
    return _allPos[frame-1];
}

const qreal &jpsYAHPointer::GetDirWhenFrame(const int &frame)
{
    return _allDir[frame-1];
}

