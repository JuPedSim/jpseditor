#ifndef ROOMS_H
#define ROOMS_H
#include <QGraphicsView>
#include "jpsLineItem.h"

class jpsRoom
{

public:
    jpsRoom(int id_room);
    ~jpsRoom(){}
    void addWall(QList<jpsLineItem *> newWalls);
    void addWall(jpsLineItem* newWall);
    void removeWall(QList<jpsLineItem *> wall);
    QString get_name();
    void change_name(QString name);
    QList<jpsLineItem*> get_listWalls();
    void activate();
    QList<QPointF> get_vertices();
    void set_id(int id_room);
    int get_id();
    QPointF get_center();
    void highlight();
    QString get_type();
    void set_type(const QString &string);


private:
    int id;
    bool highlighted;
    QString name;
    QList<jpsLineItem*> item_list;
    QString _type;
};





#endif // ROOMS_H
