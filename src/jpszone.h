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

    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon=0);
    void addinnerWall(jpsLineItem* newWall, int id_polygon=0);
    void removeWall(QList<jpsLineItem *> wall);
    void removeWall(jpsLineItem *wall);
    void removeTrack(JPSTrack *track);
    QList<jpsLineItem*> get_listWalls();

    const QList<jpsCrossing *> &GetDoors() const;
    void removeDoor(jpsCrossing* crossing);
    QString get_name();
    void change_name(QString name);

    void activate();
    //QVector<QPointF> get_vertices() const;
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
    void correctPlaneCoefficients(); /// calculates the coefficients A_x, B_y and C_z for stairs
    bool is_highlighted();
    bool isVisible();
    void setVisible(bool visibility);

    // father room can only be JPSZone::Room;
    JPSZone *getFatherRoom() const;
    void setFatherRoom(JPSZone *room);

    void addZoneInList(JPSZone *zone);
    void removeZoneFromList(JPSZone *zone);

    const QString &getName() const;

    void setName(const QString &name);

    void addTrack(jpsLineItem *line, QString number);
    const QList<JPSTrack *> &getTrackList() const;
    bool isInTrackList(JPSTrack *track);

    const QList<JPSZone *> &getPlatfromList() const;
    const QList<JPSZone *> & getCorridorList() const;


private:
    int id;
    JPSZone *father_room;

    ZoneType zoneType;
    QString name;
    bool highlighted;
    bool visible;
    float A_x;
    float B_y;
    float C_z;
    QPointF up_;
    QPointF down_;

    QList<jpsLineItem *> wall_list;
    QList<JPSTrack *>track_list;

    QVector<QLineF> outer_polygon;
    QVector<QVector<QLineF>> inner_polygons;
    QList<jpsCrossing* > doorList_;
    qreal area_;

    float elevation_; /// this makes only sense for horizontal rooms.

    // For room type
    QList<JPSZone *> platfrom_list;
    QList<JPSZone *> corridor_list;

};

bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps=0.001);

QColor rndColors(int seed);

#endif // ROOMS_H
