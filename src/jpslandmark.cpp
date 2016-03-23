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

jpsLandmark::jpsLandmark(QGraphicsPixmapItem *pixmap, const QString &caption, const QPointF &realPos, const qreal &rA, const qreal &rB, const int &id, const QString &type)
{
    _caption=caption;
    _realPos=realPos;
    _pos=realPos;
    _room=nullptr;
    _visibility=0;
    _pixmapItem=pixmap;
    _a=rA;
    _b=rB;
    _id=id;
    _type=type;
    _rect=QRectF(realPos.x()-rA,realPos.y()-rB,rA*2,rB*2);
    _textItem=nullptr;
    _pixmapText=nullptr;
    _ellipseItem=nullptr;

}

jpsLandmark::~jpsLandmark()
{
    delete _pixmapItem;
    delete _ellipseItem;
    delete _textItem;
    delete _pixmapText;
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


const QPointF &jpsLandmark::GetPos() const
{
    return _pos;

}

const QPointF &jpsLandmark::GetRealPos() const
{
    return _realPos;
}

const QRectF &jpsLandmark::GetRect() const
{
    return _rect;
}

const double& jpsLandmark::GetA() const
{
    return _a;
}

const double& jpsLandmark::GetB() const
{
    return _b;
}

const int &jpsLandmark::GetId() const
{
    return _id;
}

QGraphicsEllipseItem *jpsLandmark::GetEllipseItem() const
{
    return _ellipseItem;
}

const QString &jpsLandmark::GetCaption() const
{
    return _caption;
}

const QString &jpsLandmark::GetType() const
{
    return _type;
}

QGraphicsTextItem *jpsLandmark::GetTextItem() const
{
    return _textItem;
}

QGraphicsTextItem *jpsLandmark::GetPixmapTextItem() const
{
    return _pixmapText;
}

const QList<jpsConnection *> &jpsLandmark::GetConnections() const
{
    return _connections;
}


void jpsLandmark::SetPos(const QPointF& point)
{
    _pos=point;
    _rect.setRect(point.x(),point.y(),_rect.width(),_rect.height());
}

void jpsLandmark::SetRealPos(const QPointF &point)
{
    _realPos=point;
}

void jpsLandmark::SetRect(const QRectF& rect)
{
    _rect=rect;
    _a=_rect.width();
    _b=_rect.height();
    _pos=_rect.center();
}

void jpsLandmark::SetEllipseItem(QGraphicsEllipseItem *ellipseItem)
{
    _ellipseItem=ellipseItem;
}

void jpsLandmark::SetId(const int &id)
{
    _id=id;
}

void jpsLandmark::SetCaption(const QString &string)
{
    _caption=string;
    if (_textItem!=nullptr)
        _textItem->setPlainText(_caption);

    if (_pixmapText!=nullptr)
        _pixmapText->setPlainText(_caption);
}

void jpsLandmark::SetType(const QString &type)
{
    _type=type;
}

void jpsLandmark::SetTextItem(QGraphicsTextItem *textItem)
{
    _textItem=textItem;
}

void jpsLandmark::SetPixMapText(QGraphicsTextItem *textItem)
{
    _pixmapText=textItem;
}

void jpsLandmark::NewConnection(jpsConnection *newConnection)
{
    _connections.push_back(newConnection);
}

void jpsLandmark::RemoveConnection(jpsConnection *connection)
{
    if (_connections.contains(connection))
    {
        _connections.removeOne(connection);
    }
}



