/**
 * \file        jpswaypoint.h
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
 * This class is representing an spatial association which rely on a certain landmark.
 * jpswaypoint is not enabled in v0.8.1.
 *
 **/

#ifndef JPSWAYPOINT_H
#define JPSWAYPOINT_H

#include <QGraphicsEllipseItem>

class jpsWaypoint
{
public:
    jpsWaypoint();
    jpsWaypoint(const QPointF& point,const qreal& rA, const qreal& rB, const int& id=0, const QString& type="Waypoint");
    jpsWaypoint(QRectF rect, const int& id=0);
    jpsWaypoint(QGraphicsEllipseItem* ellipseItem, const int& id=0);
    ~jpsWaypoint();
    //Getter
    const QPointF& GetPos() const;
    const QRectF &GetRect() const;
    const double& GetA() const;
    const double& GetB() const;
    const int& GetId() const;
    QGraphicsEllipseItem* GetEllipseItem();
    const QString& GetCaption() const;
    const QString& GetType() const;

    //Setter
    void SetPos(QPointF point);
    void SetRect(QRect rect);
    void SetEllipseItem(QGraphicsEllipseItem* ellipseItem);
    void SetId(const int& id);
    void SetCaption(const QString& string);
    void SetType(const QString& type);

    //Occurence
    const int& GetFirstFrame() const;
    const int& GetLastFrame() const;
    void SetFirstFrame(const int& frame);
    void SetLastFrame(const int& frame);
    bool OccursInFrame(const int& frame) const;

    //Currency
    bool IsCurrent() const;
    bool IsCurrentInFrame(const int& frameID);
    void SetCurrentness(bool stat, const int &frameID=1);
    const int& GetFirstFrameCurrent() const;
    const int& GetLastFrameCurrent() const;
    void ChangeCurrentness(const int& frameID);
    const QString& GetText();
    void SetText(const QString& text);

    bool Visited(const int& frameID) const;


private:
    QGraphicsEllipseItem* _ellipseItem;
    QPointF _pos;
    QRectF _rect;
    double _a;
    double _b;
    int _id;
    QString _caption;
    QString _type;

    //Occurence
    int _firstFrame;
    int _lastFrame;

    //Currentness
    int _FFcurrentness;
    int _LFcurrentness;
    bool _currentness;
    QString _text;

};

#endif // JPSWAYPOINT_H
