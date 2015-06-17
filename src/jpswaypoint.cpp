#include "jpswaypoint.h"

jpsWaypoint::jpsWaypoint()
{

}

jpsWaypoint::jpsWaypoint(QRectF rect)
{
    _rect=rect;
    _pos=_rect.center();
    _a=std::abs(_rect.width()/2.0);
    _b=std::abs(_rect.height()/2.0);
    _ellipseItem=nullptr;
}

jpsWaypoint::jpsWaypoint(QGraphicsEllipseItem* ellipseItem)
{
    _ellipseItem=ellipseItem;
    _rect=_ellipseItem->rect();
    _pos=_rect.center();
    _a=_rect.width();
    _b=_rect.height();
}

jpsWaypoint::~jpsWaypoint()
{

}

const QPointF &jpsWaypoint::GetPos()
{
    return _pos;

}

const QRectF &jpsWaypoint::GetRect()
{
    return _rect;
}

double jpsWaypoint::GetA()
{
    return _a;
}

double jpsWaypoint::GetB()
{
    return _b;
}

QGraphicsEllipseItem *jpsWaypoint::GetEllipseItem()
{
    return _ellipseItem;
}


void jpsWaypoint::SetPos(QPointF point)
{
    _pos=point;
    _rect.setRect(point.x(),point.y(),_rect.width(),_rect.height());
}

void jpsWaypoint::SetRect(QRect rect)
{
    _rect=rect;
    _a=_rect.width();
    _b=_rect.height();
    _pos=_rect.center();
}

void jpsWaypoint::SetEllipseItem(QGraphicsEllipseItem *ellipseItem)
{
    _ellipseItem=ellipseItem;
}




