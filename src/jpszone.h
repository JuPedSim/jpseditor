#ifndef ROOMS_H
#define ROOMS_H
#include <QGraphicsView>
#include "jpsLineItem.h"

class jpsCrossing;

class JPSZone
{

public:
    JPSZone(int id_zone);
    ~JPSZone(){}

    enum ZoneType {Room, Corridor, Office, Lobby, Entrance, Stair, Obstacle, Plattform, NotAssigned};

    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon=0);
    void addinnerWall(jpsLineItem* newWall, int id_polygon=0);
    void removeWall(QList<jpsLineItem *> wall);
    const QList<jpsCrossing *> &GetDoors() const;
    void removeDoor(jpsCrossing* crossing);
    QString get_name();
    void change_name(QString name);
    QList<jpsLineItem*> get_listWalls();
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

private:
    int id;
    ZoneType zoneType;
    QString name;

    bool highlighted;

    bool visible;

    float A_x;
    float B_y;
    float C_z;
    QPointF up_;
    QPointF down_;

    QList<jpsLineItem*> wall_list;
    QVector<QLineF> outer_polygon;
    QVector<QVector<QLineF>> inner_polygons;
//    QVector<QPointF> sorted_polygon;
    QList<jpsCrossing* > doorList_;
    qreal area_;


    float elevation_; /// this makes only sense for horizontal rooms.
};

bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps=0.001);

QColor rndColors(int seed);

#endif // ROOMS_H
