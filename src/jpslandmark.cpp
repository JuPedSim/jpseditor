/**
 * \file        jpslandmark.cpp
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
 * This class is representing a landmark in the building which could be set by the user.
 * The usage of landmarks is not enabled in v0.8.1.
 *
 **/

#include "jpslandmark.h"

jpsLandmark::jpsLandmark()
{

}

jpsLandmark::jpsLandmark(const QGraphicsPixmapItem *pixmap, const QString &caption, const QPointF &point, const qreal &rA, const qreal &rB, const int &id, const QString &type)
{
    _caption=caption;
    _pos=point;
    _room=nullptr;
    _visibility=0;
    _pixmapItem=pixmap;
    _a=rA;
    _b=rB;
    _id=id;
    _type=type;
    _rect=QRectF(point.x()-rA,point.y()-rB,rA*2,rB*2);

}

jpsLandmark::~jpsLandmark()
{

}

void jpsLandmark::SetPixmap(QGraphicsPixmapItem *pixmap)
{
    _pixmapItem=pixmap;
}



void jpsLandmark::SetRoom(jpsRoom *room)
{
    _room=room;
}

void jpsLandmark::SetVisibility(int visibility)
{
    _visibility=visibility;
}

QGraphicsPixmapItem* jpsLandmark::GetPixmap() const
{
    return _pixmapItem;
}



jpsRoom *jpsLandmark::GetRoom() const
{
    return _room;
}

int jpsLandmark::GetVisibility() const
{
    return _visibility;
}


