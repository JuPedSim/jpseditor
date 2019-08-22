#ifndef ROOMS_H
#define ROOMS_H

#include <QGraphicsView>
#include "jpsLineItem.h"
#include "jpstrack.h"
#include "global.h"

class jpsCrossing;

class JPSZone
{

public:
    JPSZone(int id_zone, JPSZone *father, ZoneType type);

    ~JPSZone(){}

    QString getName() const;
    void setName(const QString &name);

    void set_id(int id_room);
    void set_ax(float ax);
    void set_by(float by);
    void set_cz(float cz);
    void set_up(QPointF up);
    void set_down(QPointF down);
    void switchHighlight();
    float get_ax();
    float get_by();
    float get_cz();
    QPointF get_up();
    QPointF get_down();
    int get_id();
    QPointF get_center();
    void highlight(const QString &color="random");
    void switchVisibility();
    ZoneType getType() const;
    void setType(const ZoneType &type);
    QList<QPointF> GetDoorVertices() const;

    void AddDoor(jpsCrossing *door);
    void AddInnerDoor(jpsCrossing *door, int id_polygon=0);
    QVector<QPointF> RoomAsSortedPolygon(const QVector<QLineF> &lines) const;
    const QVector<QLineF>& GetOuterPolygon() const;
    qreal GetArea() const;
    QVector<QPointF> get_vertices() const;
    void IdentifyInnerOuter();

    // calculate attributes
    QRectF CalculateBoundingBox() const;
    qreal CalculateArea(const QVector<QLineF> &poly) const;

    float get_elevation();
    void set_elevation(float elevation);
    void correctPlaneCoefficients(); // calculates the coefficients A_x, B_y and C_z for stairs
    bool is_highlighted();
    bool isVisible();
    void setVisible(bool visibility);

    // For room, father room can only be JPSZone::Room;
    JPSZone *getFatherRoom() const;
    void setFatherRoom(JPSZone *room);
    void addZoneInList(JPSZone *zone);
    void removeZoneFromList(JPSZone *zone);
    const QList<JPSZone *> &getPlatfromList() const;
    const QList<JPSZone *> &getCorridorList() const;

    // Wall
    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon=0);
    void addinnerWall(jpsLineItem* newWall, int id_polygon=0);
    void removeWall(QList<jpsLineItem *> wall);
    void removeWall(jpsLineItem *wall);
    QList<jpsLineItem *> get_listWalls();
    bool isInWallList(jpsLineItem* wall);

    // Crossing
    QList<jpsCrossing *> getCrossingList() const;
    bool isInCrossingList(jpsCrossing *crossing);
    void addCrossing(jpsLineItem* crossing);
    void addCrossing(QList<jpsLineItem *> crossings);
    void removeCrossing(jpsCrossing* crossing);

    // Track
    void addTrack(jpsLineItem *line, QString number);
    void removeTrack(JPSTrack *track);
    QList<JPSTrack *> getTrackList() const;
    bool isInTrackList(JPSTrack *track);

private:
    int id;
    JPSZone *father_room;

    ZoneType zoneType;
    QString name;
    bool highlighted;

    // For subroom
    QList<jpsLineItem *> wall_list;
    QList<JPSTrack *> track_list;

    // For room
    QList<jpsCrossing *> crossing_list;

    QList<JPSZone *> corridor_list;
    QList<JPSZone *> platfrom_list;

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
