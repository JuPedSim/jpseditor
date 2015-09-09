/**
 * \file        jpswaypoint.cpp
 * \date        Jun 26, 2015
 * \version     v0.7
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
 * This class is representing an spatial association which rely on a certain landmark.
 * jpswaypoint is not enabled in v0.7.
 *
 **/

#include "jpswaypoint.h"
#include <cmath>
#include <iostream>
jpsWaypoint::jpsWaypoint()
{

}

jpsWaypoint::jpsWaypoint(const QPointF &point, const qreal &rA, const qreal &rB, const int &id, const QString &type)
{
    _pos=point;
    _a=rA;
    _b=rB;
    _id=id;
    _type=type;
    _rect=QRectF(point.x()-rA,point.y()-rB,rA*2,rB*2);
    _currentness=false;
    _FFcurrentness=-1;
    _LFcurrentness=-1;
    _lastFrame=-1;
    _firstFrame=-1;
    _caption="";
}

jpsWaypoint::jpsWaypoint(QRectF rect, const int &id)
{
    _rect=rect;
    _pos=_rect.center();
    _a=std::abs(_rect.width()/2.0);
    _b=std::abs(_rect.height()/2.0);
    _ellipseItem=nullptr;
    _id=id;
    _currentness=false;
    _FFcurrentness=-1;
    _LFcurrentness=-1;
    _lastFrame=-1;
    _firstFrame=-1;
}

jpsWaypoint::jpsWaypoint(QGraphicsEllipseItem* ellipseItem, const int &id)
{
    _ellipseItem=ellipseItem;
    _rect=_ellipseItem->rect();
    _pos=_rect.center();
    _a=_rect.width();
    _b=_rect.height();
    _id=id;
    _currentness=false;
    _FFcurrentness=-1;
    _LFcurrentness=-1;
    _lastFrame=-1;
    _firstFrame=-1;
}

jpsWaypoint::~jpsWaypoint()
{

}

const QPointF &jpsWaypoint::GetPos() const
{
    return _pos;

}

const QRectF &jpsWaypoint::GetRect() const
{
    return _rect;
}

const double& jpsWaypoint::GetA() const
{
    return _a;
}

const double& jpsWaypoint::GetB() const
{
    return _b;
}

const int &jpsWaypoint::GetId() const
{
    return _id;
}

QGraphicsEllipseItem *jpsWaypoint::GetEllipseItem()
{
    return _ellipseItem;
}

const QString &jpsWaypoint::GetCaption() const
{
    return _caption;
}

const QString &jpsWaypoint::GetType() const
{
    return _type;
}


void jpsWaypoint::SetPos(QPointF point)
{
    _pos=point;
    _rect.setRect(point.x(),point.y(),_rect.width(),_rect.height());
}

void jpsWaypoint::SetRect(QRect rect)
{
    _rect=rect;
    _a=_rect.width();
    _b=_rect.height();
    _pos=_rect.center();
}

void jpsWaypoint::SetEllipseItem(QGraphicsEllipseItem *ellipseItem)
{
    _ellipseItem=ellipseItem;
}

void jpsWaypoint::SetId(const int &id)
{
    _id=id;
}

void jpsWaypoint::SetCaption(const QString &string)
{
    _caption=string;
}

void jpsWaypoint::SetType(const QString &type)
{
    _type=type;
}

const int &jpsWaypoint::GetFirstFrame() const
{
    return _firstFrame;
}

const int &jpsWaypoint::GetLastFrame() const
{
    return _lastFrame;
}

void jpsWaypoint::SetFirstFrame(const int &frame)
{
    _firstFrame=frame;

}

void jpsWaypoint::SetLastFrame(const int &frame)
{
    _lastFrame=frame;

}

bool jpsWaypoint::OccursInFrame(const int &frame) const
{
    if (frame>=_firstFrame && frame<=_lastFrame)
    {
        return true;
    }
    return false;
}

bool jpsWaypoint::IsCurrent() const
{
    return _currentness;
}

bool jpsWaypoint::IsCurrentInFrame(const int &frameID)
{
    if (_FFcurrentness<= frameID && _LFcurrentness>=frameID)
        return true;
    else
        return false;
}

void jpsWaypoint::SetCurrentness(bool stat, const int& frameID)
{
    _currentness=stat;
    if (stat)
        _FFcurrentness=frameID;
}

const int &jpsWaypoint::GetFirstFrameCurrent() const
{
    return _FFcurrentness;
}

const int &jpsWaypoint::GetLastFrameCurrent() const
{
    return _LFcurrentness;
}

void jpsWaypoint::ChangeCurrentness(const int& frameID)
{
    _currentness=!_currentness;
    if (_currentness)
        _FFcurrentness=frameID;
    else
        _LFcurrentness=frameID;
}

const QString &jpsWaypoint::GetText()
{
    return _text;
}

void jpsWaypoint::SetText(const QString &text)
{
    _text=text;
}

bool jpsWaypoint::Visited(const int &frameID) const
{
    if (frameID > _LFcurrentness && _LFcurrentness!=-1)
        return true;

    else
        return false;
}






