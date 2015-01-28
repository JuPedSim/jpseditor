#include "rooms.h"
#include <QtGui>
#include <iostream>
#include <QGraphicsLineItem>


jpsRoom::jpsRoom(int id_room)
{
    id=id_room;
    QString string="Room ";
    name=string.append(QString::number(id));
    highlighted=false;

}

void jpsRoom::addWall(QList <jpsLineItem *> newWalls)
{
    for (int i=0; i<newWalls.size(); i++)
    {
        item_list.push_back(newWalls[i]);
    }

}

void jpsRoom::addWall(jpsLineItem *newWall)
{
    item_list.push_back(newWall);
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

QPointF jpsRoom::get_center()
{
    QList<QPointF> vertices = get_vertices();
    qreal sum_x=0;
    qreal sum_y=0;

    for (int i=0; i<vertices.size(); i++)
    {
        sum_x+=vertices[i].x();
        sum_y+=vertices[i].y();
    }
    QPointF mean;
    mean.setX(sum_x/vertices.size());
    mean.setY(sum_y/vertices.size());

    return mean;
}


void jpsRoom::highlight()
{
    if (!highlighted)
    {
        for (int i=0; i<item_list.size(); i++)
        {
            item_list[i]->get_line()->setPen(QPen(Qt::darkGreen,0));
            item_list[i]->set_defaultColor("darkGreen");
        }
        highlighted=true;
    }
    else
    {
        for (int i=0; i<item_list.size(); i++)
        {
            item_list[i]->get_line()->setPen(QPen(Qt::black,0));
            item_list[i]->set_defaultColor("black");
        }
        highlighted=false;
    }
}

QString jpsRoom::get_type()
{
    return _type;
}

void jpsRoom::set_type(const QString &string)
{
    _type=string;
}



void jpsRoom::set_id(int id_room)
{
    id=id_room;
}

int jpsRoom::get_id()
{
    return id;
}



