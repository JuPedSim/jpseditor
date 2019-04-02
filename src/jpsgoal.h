//
// Created by Tao Zhong on 2019-03-26.
//

#ifndef JPSEDITOR_JPSGOAL_H
#define JPSEDITOR_JPSGOAL_H

#include <QBrush>
#include <QGraphicsRectItem>
#include <QPen>
#include "graphicscene.h"
#include "elementtypes.h"

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class JPSGoal : public QObject, public QGraphicsRectItem
{
    Q_OBJECT


#ifdef ANIMATE_ALIGNMENT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
#endif
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(double angle READ angle WRITE setAngle)
    Q_PROPERTY(double shearHorizontal READ shearHorizontal
                       WRITE setShearHorizontal)
    Q_PROPERTY(double shearVertical READ shearVertical
                       WRITE setShearVertical)

public:
    explicit JPSGoal(QGraphicsRectItem *goalRectItem = nullptr);

    ~JPSGoal();

    enum {Type = GoalElementType};

    int type() const { return Type; }

    double angle() const { return m_angle; }
    double shearHorizontal() const { return m_shearHorizontal; }
    double shearVertical() const { return m_shearVertical; }

    int getId() const;

    void setId(int id);

    const QString &getCaption() const;

    void setCaption(const QString &caption);

    const QString &getBeSaved() const;

    void setBeSaved(const QString &beSaved);

    const QString &getIsFinal() const;

    void setIsFinal(const QString &isFinal);

signals:
    void dirty();

public slots:
    //hide non-virtual funcs from QAbstractGraphicsShapeItem
    void setPen(const QPen &pen) ;
    void setBrush(const QBrush &brush);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setAngle(double angle);
    void setShearHorizontal(double shearHorizontal)
    { setShear(shearHorizontal, m_shearVertical); }
    void setShearVertical(double shearVertical)
    { setShear(m_shearHorizontal, shearVertical); }
    void setShear(double shearHorizontal, double shearVertical);

    QGraphicsRectItem *getRectItem() const;

    void setRectItem(QGraphicsRectItem *rectItem);

protected:
    QVariant itemChange(GraphicsItemChange change,const QVariant &value);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    //JuPedSim attributes
    int id;
    QString caption;
    QString beSaved;
    QString isFinal;

    //QGraphicsItem attributes
    QPen currentPen;
    void updateTransform();
    bool m_resizing;
    double m_angle;
    double m_shearHorizontal;
    double m_shearVertical;
    QGraphicsRectItem *rectItem;
};


#endif //JPSEDITOR_JPSGOAL_H
