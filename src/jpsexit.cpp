#include "jpsexit.h"


jpsExit::jpsExit(jpsLineItem *line)
{
    cLine=line;
}

QList<jpsRoom *> jpsExit::get_roomList()
{
    return roomList;
}


QString jpsExit::get_name()
{
    return cName;
}

jpsLineItem *jpsExit::get_cLine()
{
    return cLine;
}

void jpsExit::change_name(QString name)
{
    cName=name;
}

QString jpsExit::get_type()
{
    return _type;
}

int jpsExit::get_id()
{
    return _id;
}

void jpsExit::set_id(int id)
{
    _id=id;
}

void jpsExit::set_type(QString type)
{
    _type=type;
}

void jpsExit::add_rooms(jpsRoom *room1, jpsRoom *room2)
{
    roomList.clear();
    roomList.push_back(room1);
    if (room2!=0L)
    {
        roomList.push_back(room2);
    }
}
