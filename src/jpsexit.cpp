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

void jpsExit::add_rooms(jpsRoom *room1, jpsRoom *room2)
{
    roomList.push_back(room1);
    if (room2!=0L)
    {
        roomList.push_back(room2);
    }
}
