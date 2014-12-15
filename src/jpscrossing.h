#ifndef JPSCROSSING_H
#define JPSCROSSING_H

#include "rooms.h"

class jpsCrossing
{
public:
    jpsCrossing(jpsLineItem *line);
    ~jpsCrossing(){}
    QList<jpsRoom *> get_roomList();
    QString get_name();
    int get_id();
    void set_id(int id);
    jpsLineItem *get_cLine();
    void change_name(QString name);
    void add_rooms(jpsRoom* room1, jpsRoom* room2=0L);


private:
    QList<jpsRoom *> roomList;
    QString cName;
    jpsLineItem *cLine;
    int cId;

};

#endif // JPSCROSSING_H
