#include "jpsregion.h"

jpsRegion::jpsRegion(const int &id, const QString &caption, const QPointF &pos, const qreal &a, const qreal &b, const QString &type, const int &floor)
{
    qDebug("Enter jpsRegion::jpsRegion (first Constructor)");
    _id=id;
    _caption=caption;
    _pos=pos;
    _a=a;
    _b=b;
    _type=type;
    _ellipseItem=nullptr;
    _textItem=nullptr;
    _floor=floor;
    qDebug("Leave jpsRegion::jpsRegion");
}

jpsRegion::jpsRegion(const int &id, const QString &caption, QGraphicsEllipseItem *ellipse, QGraphicsTextItem* text, const QString &type, const int &floor)
{
    qDebug("Enter jpsRegion::jpsRegion (second Constructor");
    _id=id;
    _caption=caption;
    _ellipseItem=ellipse;
    _type=type;
    _pos=_ellipseItem->pos();
    _a=_ellipseItem->rect().width()/2.0;
    _b=_ellipseItem->rect().height()/2.0;
    _textItem=text;
    _floor=floor;
    qDebug("Leave jpsRegion::jpsRegion");
}

jpsRegion::~jpsRegion()
{
    qDebug("Enter jpsRegion::~jpsRegion");
    delete _ellipseItem;
    delete _textItem;
    qDebug("Leave jpsRegion::~jpsRegion");

}

void jpsRegion::AddLandmark(jpsLandmark *landmark)
{
    qDebug("Enter jpsRegion::AddLandmark");
    _landmarks.push_back(landmark);
    qDebug("Leave jpsRegion::AddLandmark");
}

void jpsRegion::RemoveLandmark(jpsLandmark *landmark)
{
    qDebug("Enter jpsRegion::RemoveLandmark");
    _landmarks.removeOne(landmark);
    qDebug("Leave jpsRegion::RemoveLandmark");
}

const int &jpsRegion::GetId() const
{
    qDebug("Enter/Return jpsRegion::GetId");
    return _id;
}

const QPointF &jpsRegion::GetPos() const
{
    qDebug("Enter/Retrun jpsRegion::GetPos");
    return _pos;
}

const QString &jpsRegion::GetCaption() const
{
    qDebug("Enter/Retrun jpsRegion::GetPos");
    return _caption;
}

QGraphicsTextItem *jpsRegion::GetTextItem() const
{
    qDebug("Enter/Return jpsRegion::GetPos");
    return _textItem;
}

QGraphicsEllipseItem *jpsRegion::GetEllipseItem() const
{
    qDebug("Enter/Return jpsRegion::GetEllipseItem");
    return _ellipseItem;
}

QList<jpsLandmark *> jpsRegion::GetLandmarks() const
{
    qDebug("Enter/Return jpsRegion::GetLandmarks");
    return _landmarks;
}

const qreal &jpsRegion::GetA() const
{
    qDebug("Enter/Return jpsRegion::GetA");
    return _a;
}

const qreal &jpsRegion::GetB() const
{
    qDebug("Enter/Return jpsRegion::GetB");
    return _b;
}

void jpsRegion::SetTextItem(QGraphicsTextItem *textItem)
{
    qDebug("Enter jpsRegion::SetTextItem");
    _textItem=textItem;
    qDebug("Leave jpsRegion::SetTextItem");
}

void jpsRegion::SetEllipse(QGraphicsEllipseItem *ellipseItem)
{
    qDebug("Enter jpsRegion::SetEllipse");
    _ellipseItem=ellipseItem;
    qDebug("Leave jpsRegion::SetEllipse");
}

