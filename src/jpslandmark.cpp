/**
 * \file        jpslandmark.cpp
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
 * This class is representing a landmark in the building which could be set by the user.
 * The usage of landmarks is not enabled in v0.8.1.
 *
 **/

#include "jpslandmark.h"
#include<cmath>

jpsLandmark::jpsLandmark()
{

}

jpsLandmark::jpsLandmark(QGraphicsPixmapItem *pixmap, const QString &caption, const QPointF &realPos, const qreal &rA, const qreal &rB, const int &id, const QString &type)
{
    qDebug("Enter jpsLandmark::jpsLandmark");
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
    _region=nullptr;
    qDebug("Leave jpsLandmark::jpsLandmark");

}

jpsLandmark::~jpsLandmark()
{
    qDebug("Enter jpsLandmark::~jpsLandmark");
    delete _pixmapItem;
    delete _ellipseItem;
    delete _textItem;
    delete _pixmapText;
    qDebug("Leave jpsLandmark::~jpsLandmark");
}

void jpsLandmark::SetPixmap(QGraphicsPixmapItem *pixmap)
{
    qDebug("Enter jpsLandmark::SetPixmap");
    _pixmapItem=pixmap;
    qDebug("Leave jpsLandmark::SetPixmap");
}



void jpsLandmark::SetRoom(JPSZone *room)
{
    qDebug("Enter jpsLandmark::SetRoom");
    _room=room;
    qDebug("Leave jpsLandmark::SetRoom");
}

void jpsLandmark::SetVisibility(int visibility)
{
    qDebug("Enter jpsLandmark::SetVisibility");
    _visibility=visibility;
    qDebug("Leave jpsLandmark::SetVisibility");
}

QGraphicsPixmapItem* jpsLandmark::GetPixmap() const
{
    qDebug("Enter/Return jpsLandmark::GetPixmap");
    return _pixmapItem;
}



JPSZone *jpsLandmark::GetRoom() const
{
    qDebug("Enter/Return jpsLandmark::GetRoom");
    return _room;
}

int jpsLandmark::GetVisibility() const
{
    qDebug("Enter/Return jpsLandmark::GetVisibility");
    return _visibility;
}


const QPointF &jpsLandmark::GetPos() const
{
    qDebug("Enter/Return jpsLandmark::GetPos");
    return _pos;

}

const QPointF &jpsLandmark::GetRealPos() const
{
    qDebug("Enter/Return jpsLandmark::GetRealPos");
    return _realPos;
}

const QRectF &jpsLandmark::GetRect() const
{
    qDebug("Enter/Return jpsLandmark::GetRect");
    return _rect;
}

const double& jpsLandmark::GetA() const
{
    qDebug("Enter/Return jpsLandmark::GetA");
    return _a;
}

const double& jpsLandmark::GetB() const
{
    qDebug("Enter/Retrun jpsLandmark::GetB");
    return _b;
}

const int &jpsLandmark::GetId() const
{
    qDebug("Enter/Return jpsLandmark::GetId");
    return _id;
}

QGraphicsEllipseItem *jpsLandmark::GetEllipseItem() const
{
    qDebug("Enter/Return jpsLandmark::GetEllipseItem");
    return _ellipseItem;
}

const QString &jpsLandmark::GetCaption() const
{
    qDebug("Enter/Return jpsLandmark::GetEllipseItem");
    return _caption;
}

const QString &jpsLandmark::GetType() const
{
    qDebug("Enter/Return jpsLandmark::GetType");
    return _type;
}

QGraphicsTextItem *jpsLandmark::GetTextItem() const
{
    qDebug("Enter/Return jpsLandmark::GetTextItem");
    return _textItem;
}

QGraphicsTextItem *jpsLandmark::GetPixmapTextItem() const
{
    qDebug("Enter/Return jpsLandmark::GetPixmapTextItem");
    return _pixmapText;
}

const QList<jpsConnection *> &jpsLandmark::GetConnections() const
{
    qDebug("Enter/Return jpsLandmark::GetConnections");
    return _connections;
}


void jpsLandmark::SetPos(const QPointF& point)
{
    qDebug("Enter jpsLandmark::SetPos");
    _pos=point;
    _rect.setRect(point.x(),point.y(),_rect.width(),_rect.height());
    qDebug("Leave jpsLandmark::SetPos");
}

void jpsLandmark::SetRealPos(const QPointF &point)
{
    qDebug("Enter jpsLandmark::SetRealPos");
    _realPos=point;
    qDebug("Leave jpsLandmark::SetRealPos");
}

void jpsLandmark::SetRect(const QRectF& rect)
{
    qDebug("Enter jpsLandmark::SetRect");
    _rect=rect;
    _a=std::fabs(_rect.width()/2.0);
    _b=std::fabs(_rect.height()/2.0);
    _pos=_rect.center();
    qDebug("Leave jpsLandmark::SetRect");
}

void jpsLandmark::SetEllipseItem(QGraphicsEllipseItem *ellipseItem)
{
    qDebug("Enter jpsLandmark::SetEllipseItem");
    _ellipseItem=ellipseItem;
    qDebug("Leave jpsLandmark::SetEllipseItem");
}

void jpsLandmark::SetId(const int &id)
{
    qDebug("Enter jpsLandmark::SetId");
    _id=id;
    qDebug("Leave jpsLandmark::SetId");
}

void jpsLandmark::SetCaption(const QString &string)
{
    qDebug("Enter jpsLandmark::SetCaption");
    _caption=string;
    if (_textItem!=nullptr)
        _textItem->setPlainText(_caption);

    if (_pixmapText!=nullptr)
        _pixmapText->setPlainText(_caption);
    qDebug("Leave jpsLandmark::SetCaption");
}

void jpsLandmark::SetType(const QString &type)
{
    qDebug("Enter jpsLandmark::SetType");
    _type=type;
    qDebug("Leave jpsLandmark::SetType");
}

void jpsLandmark::SetTextItem(QGraphicsTextItem *textItem)
{
    qDebug("Enter jpsLandmark::SetTextItem");
    _textItem=textItem;
    qDebug("Leave jpsLandmark::SetTextItem");
}

void jpsLandmark::SetPixMapText(QGraphicsTextItem *textItem)
{
    qDebug("Enter jpsLandmark::SetPixMapText");
    _pixmapText=textItem;
    qDebug("Leave jpsLandmark::SetPixMapText");
}

void jpsLandmark::NewConnection(jpsConnection *newConnection)
{
    qDebug("Enter jpsLandmark::NewConnection");
    _connections.push_back(newConnection);
    qDebug("Leave jpsLandmark::NewConnection");
}

void jpsLandmark::RemoveConnection(jpsConnection *connection)
{
    qDebug("Enter jpsLandmark::RemoveConnection");
    if (_connections.contains(connection))
    {
        _connections.removeOne(connection);
    }
    qDebug("Leave jpsLandmark::RemoveConnection");
}

void jpsLandmark::SetRegion(jpsRegion *region)
{
    qDebug("Enter jpsLandmark::SetRegion");
    _region=region;
    qDebug("Leave jpsLandmark::SetRegion");
}

jpsRegion *jpsLandmark::GetRegion() const
{
    qDebug("Enter/Return jpsLandmark::GetRegion");
    return _region;
}



