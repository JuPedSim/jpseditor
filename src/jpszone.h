/**
 * \file        jpszone.h
 * \date        Feb-26-2020
 * \version     v0.8.9
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
 * This class represents zones in geometry, in differernt types (room, stair, plattform)
 *
 **/

#ifndef ROOMS_H
#define ROOMS_H

#include <QGraphicsView>
#include "jpsLineItem.h"
#include "jpstrack.h"
#include "global.h"

class jpsTransition;

class JPSZone
{

public:
    JPSZone(int id_zone, ZoneType type);

    ~JPSZone(){}

    /// General
    QString getName() const;
    void setName(const QString &name);

    int get_id();
    void set_id(int id_room);
    void set_ax(float ax);
    void set_by(float by);
    void set_cz(float cz);

    bool is_highlighted();
    void highlight(const QString &color="random");

    ZoneType getType() const;
    QString getTypeInString() const;
    void setType(const ZoneType &type);

    void switchVisibility();
    bool isVisible();
    void setVisible(bool visibility);

    /// For stair
    void set_up(QPointF up);
    void set_down(QPointF down);
    void switchHighlight();
    float get_ax();
    float get_by();
    float get_cz();
    QPointF get_up();
    QPointF get_down();

    QPointF get_center();
    QList<QPointF> GetDoorVertices() const;

    QVector<QPointF> RoomAsSortedPolygon(const QVector<QLineF> &lines) const;
    const QVector<QLineF>& GetOuterPolygon() const;
    qreal GetArea() const;
    QVector<QPointF> get_vertices() const;
    void IdentifyInnerOuter();

    /// Calculate attributes
    QRectF CalculateBoundingBox() const;
    qreal CalculateArea(const QVector<QLineF> &poly) const;

    float get_elevation();
    void set_elevation(float elevation);
    void correctPlaneCoefficients(QList<jpsTransition *> transitions); // calculates the coefficients A_x, B_y and C_z

    // Wall
    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon=0);
    void addinnerWall(jpsLineItem* newWall, int id_polygon=0);
    void removeWall(QList<jpsLineItem *> wall);
    void removeWall(jpsLineItem *wall);
    QList<jpsLineItem *> get_listWalls();
    bool isInWallList(jpsLineItem* wall);

    // Track
    void addTrack(jpsLineItem *line, QString number);
    void removeTrack(JPSTrack *track);
    QList<JPSTrack *> getTrackList() const;
    bool isInTrackList(JPSTrack *track);

private:
    int id;
    ZoneType zoneType;
    QString name;
    bool highlighted;

    QList<jpsLineItem *> wall_list;
    QList<JPSTrack *> track_list;

    bool visible;
    float A_x;
    float B_y;
    float C_z;
    QPointF up_;
    QPointF down_;

    QVector<QLineF> outer_polygon;
    QVector<QVector<QLineF>> inner_polygons;

    qreal area_;

    // this makes only sense for horizontal rooms.
    float elevation_;

};



bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps=0.001);

QColor rndColors(int seed);

#endif // ROOMS_H
