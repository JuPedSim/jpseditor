#include "jpsLineItem.h"
#include <iostream>

jpsLineItem::jpsLineItem(QGraphicsLineItem *line)
{
    mLine=line;
}

jpsLineItem::~jpsLineItem()
{

}

QGraphicsLineItem *jpsLineItem::get_line()
{
    return mLine;
}

void jpsLineItem::set_line(QGraphicsLineItem *line)
{
    mLine=line;
}

int jpsLineItem::get_id()
{
    return mId;
}

QString jpsLineItem::get_name()
{
    return mName;
}

void jpsLineItem::set_id(int id)
{
    mId=id;
}

void jpsLineItem::set_name(QString name)
{
    mName=name;
}

void jpsLineItem::set_type(bool wall, bool door, bool exit)
{
    if (wall==true)
    {
        set_Wall();
    }
    else if (door==true)
    {
        set_Door();
    }
    else if (exit==true)
    {
        set_Exit();
    }
}

void jpsLineItem::set_Wall()
{
    wall=true;
    door=false;
    exit=false;
}

void jpsLineItem::set_Door()
{
    door=true;
    wall=false;
    exit=false;
}

void jpsLineItem::set_Exit()
{
    exit=true;
    wall=false;
    exit=false;
}

bool jpsLineItem::is_Wall()
{
    return wall;
}

bool jpsLineItem::is_Door()
{
    return door;
}

bool jpsLineItem::is_Exit()
{
    return exit;
}

void jpsLineItem::add_intersectionPoint(QPointF *point)
{
    intersectionVector.push_back(point);
}

QList<QPointF *> jpsLineItem::get_intersectionVector()
{
    return intersectionVector;
}




