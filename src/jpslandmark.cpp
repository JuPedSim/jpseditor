#include "jpslandmark.h"

jpsLandmark::jpsLandmark()
{

}

jpsLandmark::jpsLandmark(QGraphicsPixmapItem* pixmap, QString name, QPointF point)
{
    _name=name;
    _pos=point;
    _room=nullptr;
    _visibility=0;
    _pixmapItem=pixmap;


}

jpsLandmark::~jpsLandmark()
{
    RemoveAllWaypoints();
}

void jpsLandmark::set_pixmap(QGraphicsPixmapItem *pixmap)
{
    _pixmapItem=pixmap;
}

void jpsLandmark::set_name(QString name)
{
    _name=name;

}

void jpsLandmark::set_pos(QPointF pos)
{
    _pos=pos;
}

void jpsLandmark::set_room(jpsRoom *room)
{
    _room=room;
}

void jpsLandmark::set_visibility(int visibility)
{
    _visibility=visibility;
}

QGraphicsPixmapItem* jpsLandmark::get_pixmap()
{
    return _pixmapItem;
}

QString jpsLandmark::get_name()
{
    return _name;
}

const QPointF &jpsLandmark::get_pos()
{
    return _pos;
}

jpsRoom *jpsLandmark::get_room()
{
    return _room;
}

int jpsLandmark::get_visibility()
{
    return _visibility;
}

void jpsLandmark::AddWaypoint(jpsWaypoint *waypoint)
{
    _waypoints.push_back(waypoint);
}

void jpsLandmark::RemoveWaypoint(jpsWaypoint *waypoint)
{
    delete waypoint;
    _waypoints.removeOne(waypoint);
}

QList<jpsWaypoint *> jpsLandmark::GetWaypoints()
{
    return _waypoints;
}

void jpsLandmark::RemoveAllWaypoints()
{
    for (jpsWaypoint* waypoint:_waypoints)
    {
        delete waypoint;
    }
    _waypoints.clear();
}

