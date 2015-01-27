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
    QString get_type();
    int get_id();
    void set_id(int id);
    void set_type(QString type);
    void add_rooms(jpsRoom* room1, jpsRoom* room2=0L);

private:
    QList<jpsRoom *> roomList;
    QString cName;
    jpsLineItem *cLine;
    QString _type;
    int _id;

};

#endif // JPSEXIT_H
