/**
 * \file        jpslandmark.h
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

#ifndef JPSLANDMARK_H
#define JPSLANDMARK_H
#include <QPointF>
#include <QGraphicsTextItem>
#include <memory>
#include "rooms.h"

class jpsConnection;
class jpsRegion;


class jpsLandmark
{
public:
    jpsLandmark();
    jpsLandmark(QGraphicsPixmapItem* pixmap,const QString& caption, const QPointF& realPos,const qreal& rA=0, const qreal& rB=0, const int& id=0, const QString& type="Landmark");
    ~jpsLandmark();
    //Setter

    //Getter
    QGraphicsPixmapItem* GetPixmap() const;
    jpsRoom *GetRoom() const;
    int GetVisibility() const;
    const QPointF& GetPos() const;
    const QPointF& GetRealPos() const;
    const QRectF &GetRect() const;
    const double& GetA() const;
    const double& GetB() const;
    const int& GetId() const;
    QGraphicsEllipseItem* GetEllipseItem() const;
    const QString& GetCaption() const;
    const QString& GetType() const;
    QGraphicsTextItem* GetTextItem() const;
    QGraphicsTextItem* GetPixmapTextItem() const;
    const QList<jpsConnection*>& GetConnections() const;

    //Setter
    void SetPixmap(QGraphicsPixmapItem* pixmap);
    void SetRoom(jpsRoom* room);
    void SetVisibility(int visibility);
    void SetPos(const QPointF& point);
    void SetRealPos(const QPointF& point);
    void SetRect(const QRectF &rect);
    void SetEllipseItem(QGraphicsEllipseItem* ellipseItem);
    void SetId(const int& id);
    void SetCaption(const QString& string);
    void SetType(const QString& type);
    void SetTextItem(QGraphicsTextItem* textItem);
    void SetPixMapText(QGraphicsTextItem* textItem);

    //Connections
    void NewConnection(jpsConnection* newConnection);
    void RemoveConnection(jpsConnection* connection);

    //Region
    void SetRegion(jpsRegion* region);


//    //Occurence
//    const int& GetFirstFrame() const;
//    const int& GetLastFrame() const;
//    void SetFirstFrame(const int& frame);
//    void SetLastFrame(const int& frame);
//    bool OccursInFrame(const int& frame) const;

//    //Currency
//    bool IsCurrent() const;
//    bool IsCurrentInFrame(const int& frameID);
//    void SetCurrentness(bool stat, const int &frameID=1);
//    const int& GetFirstFrameCurrent() const;
//    const int& GetLastFrameCurrent() const;
//    void ChangeCurrentness(const int& frameID);
//    const QString& GetText();
//    void SetText(const QString& text);

//    bool Visited(const int& frameID) const;


private:
    QString _caption;
    qreal _a;
    qreal _b;
    int _id;
    QString _type;
    QPointF _pos;
    QPointF _realPos;
    jpsRoom* _room;
    int _visibility;
    QGraphicsPixmapItem* _pixmapItem;
    QRectF _rect;
    QGraphicsEllipseItem* _ellipseItem;
    QGraphicsTextItem* _textItem;
    QGraphicsTextItem* _pixmapText;
    QList<jpsConnection*> _connections;
    jpsRegion* _region;

};

#endif // JPSLANDMARK_H
