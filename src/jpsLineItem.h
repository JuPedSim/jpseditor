#ifndef JPSLineItem_H
#define JPSLineItem_H

#include <QGraphicsView>


class jpsLineItem
{
public:
    jpsLineItem(QGraphicsLineItem* line);
    ~jpsLineItem();
    QGraphicsLineItem* get_line();
    void set_line(QGraphicsLineItem* line);
    int get_id();
    QString get_name();
    void set_id(int id);
    void set_name(QString name);
    void set_type(bool wall, bool door, bool exit);
    void set_Wall();
    void set_Door();
    void set_Exit();
    bool is_Wall();
    bool is_Door();
    bool is_Exit();
    void add_intersectionPoint(QPointF *point);
    void remove_intersectionPoint(QPointF *point);
    QList<QPointF *> get_intersectionVector();
    QList<jpsLineItem* > get_intersectLineVector();
    void add_intersectLine(jpsLineItem * lineItem);
    void remove_interLine(jpsLineItem * lineItem);



private:
    QGraphicsLineItem* mLine;
    int mId;
    QString mName;
    bool wall;
    bool door;
    bool exit;
    QList<QPointF *> intersectionVector;
    QList<jpsLineItem* > intersectLineVector;
    /*
    jpsCrossing* assigned_crossing=0L;
    jpsRoom* assigned_room=0L;
    jpsExit* assinged_exit=0L;
    */
};

#endif // JPSLineItem_H
