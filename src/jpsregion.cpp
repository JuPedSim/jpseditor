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

jpsRegion::jpsRegion(const int &id, const QString &caption, QGraphicsEllipseItem *ellipse, QGraphicsTextItem* text, const QString &type)
{
    _id=id;
    _caption=caption;
    _ellipseItem=ellipse;
    _type=type;
    _pos=_ellipseItem->pos();
    _a=_ellipseItem->rect().width()/2.0;
    _b=_ellipseItem->rect().height()/2.0;
    _textItem=text;
}

jpsRegion::~jpsRegion()
{
    delete _ellipseItem;
    delete _textItem;

}

void jpsRegion::AddLandmark(jpsLandmark *landmark)
{
    _landmarks.push_back(landmark);
}

void jpsRegion::RemoveLandmark(jpsLandmark *landmark)
{
    _landmarks.removeOne(landmark);
}

const QPointF &jpsRegion::GetPos() const
{
    return _pos;
}

const QString &jpsRegion::GetCaption() const
{
    return _caption;
}

