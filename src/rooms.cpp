#include "rooms.h"
#include <QtGui>
#include <iostream>
#include <QGraphicsLineItem>


jpsRoom::jpsRoom(int id_room)
{
    id=id_room;
    QString string="Room ";
    name=string.append(QString::number(id));

}

void jpsRoom::addWall(QList <jpsLineItem *> newWall)
{
    for (int i=0; i<newWall.size(); i++)
    {
        item_list.push_back(newWall[i]);
    }

}

void jpsRoom::removeWall(QList <jpsLineItem *> wall)
{
    for (int i=0; i<wall.size(); i++)
    {
        item_list.removeOne(wall[i]);
    }
}

QString jpsRoom::get_name()
{
    return name;
}

void jpsRoom::change_name(QString name)
{
    this->name=name;
}

QList<jpsLineItem *> jpsRoom::get_listWalls()
{
    return item_list;
}

void jpsRoom::activate()
{
    for (signed int i=0; i<item_list.size(); i++)
    {
        item_list[i]->get_line()->setPen(QPen(Qt::yellow));
    }
}

QList<QPointF> jpsRoom::get_vertices()
{
    QList<QPointF> vertices;

    for (int i=0; i<item_list.size(); i++)
    {
        if (vertices.contains(item_list[i]->get_line()->line().p1())==false)
        {
            vertices.push_back(item_list[i]->get_line()->line().p1());
        }
        if (vertices.contains(item_list[i]->get_line()->line().p2())==false)
        {
            vertices.push_back(item_list[i]->get_line()->line().p2());
        }
    }

    return vertices;
}

void jpsRoom::set_id(int id_room)
{
    id=id_room;
}

int jpsRoom::get_id()
{
    return id;
}



