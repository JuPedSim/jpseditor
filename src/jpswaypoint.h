#ifndef JPSWAYPOINT_H
#define JPSWAYPOINT_H

#include <QGraphicsEllipseItem>

class jpsWaypoint
{
public:
    jpsWaypoint();
    jpsWaypoint(QRectF rect);
    jpsWaypoint(QGraphicsEllipseItem* ellipseItem);
    ~jpsWaypoint();
    ///Getter
    const QPointF& GetPos();
    const QRectF &GetRect();
    double GetA();
    double GetB();
    QGraphicsEllipseItem* GetEllipseItem();

    ///Setter
    void SetPos(QPointF point);
    void SetRect(QRect rect);
    void SetEllipseItem(QGraphicsEllipseItem* ellipseItem);


private:
    QGraphicsEllipseItem* _ellipseItem;
    QPointF _pos;
    QRectF _rect;
    double _a;
    double _b;

};

#endif // JPSWAYPOINT_H
