#ifndef JPSEXIT_H
#define JPSEXIT_H

#include "rooms.h"

class jpsExit
{
public:
    jpsExit(jpsLineItem *line);
    ~jpsExit(){}
    QList<jpsRoom *> get_roomList();
    QString get_name();
    jpsLineItem *get_cLine();
    void change_name(QString name);
    void add_rooms(jpsRoom* room1, jpsRoom* room2=0L);

private:
    QList<jpsRoom *> roomList;
    QString cName;
    jpsLineItem *cLine;

};

#endif // JPSEXIT_H
