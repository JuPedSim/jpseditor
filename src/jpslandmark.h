#ifndef JPSLANDMARK_H
#define JPSLANDMARK_H
#include <QPointF>
#include "rooms.h"
#include "jpswaypoint.h"


class jpsLandmark
{
public:
    jpsLandmark();
    jpsLandmark(QGraphicsPixmapItem* pixmap, QString name, QPointF point);
    ~jpsLandmark();
    ///Setter
    void set_pixmap(QGraphicsPixmapItem* pixmap);
    void set_name(QString name);
    void set_pos(QPointF pos);
    void set_room(jpsRoom* room);
    void set_visibility(int visibility);
    ///Getter
    QGraphicsPixmapItem* get_pixmap();
    QString get_name();
    const QPointF& get_pos();
    jpsRoom *get_room();
    int get_visibility();
    ///Waypoint handling
    void AddWaypoint(jpsWaypoint* waypoint);
    void RemoveWaypoint(jpsWaypoint* waypoint);
    QList<jpsWaypoint* > GetWaypoints();
    void RemoveAllWaypoints();


private:
    QString _name;
    QPointF _pos;
    jpsRoom* _room;
    int _visibility;
    QGraphicsPixmapItem* _pixmapItem;
    QList<jpsWaypoint* > _waypoints;

};

#endif // JPSLANDMARK_H
