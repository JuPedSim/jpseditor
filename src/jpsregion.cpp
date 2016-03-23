#include "jpsregion.h"

jpsRegion::jpsRegion(const int &id, const QString &caption, const QPointF &pos, const qreal &a, const qreal &b, const QString &type)
{
    _id=id;
    _caption=caption;
    _pos=pos;
    _a=a;
    _b=b;
    _type=type;
    _ellipseItem=nullptr;
    _textItem=nullptr;
}

jpsRegion::jpsRegion(const int &id, const QString &caption, QGraphicsEllipseItem *ellipse, const QString &type)
{
    _id=id;
    _caption=caption;
    _ellipseItem=ellipse;
    _textItem=nullptr;
}

jpsRegion::~jpsRegion()
{

}

void jpsRegion::AddLandmark(jpsLandmark *landmark)
{
    _landmarks.push_back(landmark);
}

void jpsRegion::RemoveLandmark(jpsLandmark *landmark)
{
    _landmarks.removeOne(landmark);
}

