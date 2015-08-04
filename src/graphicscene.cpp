#include "graphicscene.h"
#include <QVarLengthArray>


GraphicScene::GraphicScene(QObject *parent) : QGraphicsScene(parent)
{
    _translationX=0.0;
    _translationY=0.0;
    _gridmode=false;
    _statgrid="Line";
    _gridSize=1.0;
}

GraphicScene::~GraphicScene()
{

}

void GraphicScene::ChangeGridmode(const bool &stat)
{
    _gridmode=stat;
}

bool GraphicScene::GetGridmode() const
{
    return _gridmode;
}

void GraphicScene::ChangeTranslation(qreal x, qreal y)
{
    _translationX=x;
    _translationY=y;
}

void GraphicScene::SetGrid(QString grid)
{
    _statgrid=grid;
}

void GraphicScene::ChangeGridSize(const qreal &gridSize)
{
    _gridSize=gridSize;
}

void GraphicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (_gridmode)
    {
        if (_statgrid=="Line")
            DrawLineGrid(painter,rect);
        else
            DrawPointGrid(painter,rect);
    }   
}

void GraphicScene::DrawLineGrid(QPainter *painter, const QRectF &rect)
{
    //gridSize=1.0;
    qreal left = int(rect.left()-_translationX) - std::fmod(int(rect.left()-_translationX), _gridSize);
    qreal top = int(rect.top()-_translationY)- std::fmod(int(rect.top()-_translationY) , _gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right()-_translationX; x += _gridSize)
        lines.append(QLineF(x+_translationX, rect.top(), x+_translationX, rect.bottom()));
    for (qreal y = top; y < rect.bottom()-_translationY; y += _gridSize)
        lines.append(QLineF(rect.left(), y+_translationY, rect.right(), y+_translationY));

    //qDebug() << lines.size();
    painter->setPen(QPen(Qt::gray,0));
    painter->drawLines(lines.data(), lines.size());
}

void GraphicScene::DrawPointGrid(QPainter *painter, const QRectF &rect)
{


    qreal left = int(rect.left()-_translationX) - std::fmod(int(rect.left()-_translationX), _gridSize);
    qreal top = int(rect.top()-_translationY)- std::fmod(int(rect.top()-_translationY) , _gridSize);

    QVarLengthArray<QPointF, 100> points;

    for (qreal x = left; x < rect.right()-_translationX; x += _gridSize)
    {
        for (qreal y = top; y < rect.bottom()-_translationY; y += _gridSize)
        {
            points.append(QPointF(x+_translationX, y+_translationY));
        }
    }
    //qDebug() << lines.size();
    painter->setPen(QPen(Qt::black,0));
    painter->drawPoints(points.data(), points.size());
}

