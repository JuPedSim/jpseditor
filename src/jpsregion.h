#ifndef JPSREGION_H
#define JPSREGION_H
#include "jpslandmark.h"

class jpsRegion
{
public:
    jpsRegion(const int& id, const QString& caption, const QPointF& pos, const qreal& a, const qreal& b, const QString& type="Region");
    jpsRegion(const int& id, const QString& caption, QGraphicsEllipseItem* ellipse, QGraphicsTextItem* text=nullptr, const QString& type="Region");
    ~jpsRegion();

    void AddLandmark(jpsLandmark* landmark);
    void RemoveLandmark(jpsLandmark* landmark);

    //Getter
    const QPointF& GetPos() const;
    const QString& GetCaption() const;

private:
    QString _caption;
    qreal _a;
    qreal _b;
    int _id;
    QString _type;
    QPointF _pos;
    QGraphicsEllipseItem* _ellipseItem;
    QGraphicsTextItem* _textItem;

    //Landmarks
    QList<jpsLandmark*> _landmarks;
};

#endif // JPSREGION_H
