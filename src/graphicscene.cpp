#include "graphicscene.h"
#include <iostream>

GraphicScene::GraphicScene()
{

}

GraphicScene::GraphicScene(QObject *parent) : QGraphicsScene(parent)
{
    _translationX=0.0;
    _translationY=0.0;
    _gridmode=false;
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

void GraphicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (_gridmode)
    {
        const int gridSize = 1;

        qreal left = int(rect.left()-_translationX) - (int(rect.left()-_translationX) % gridSize);
        qreal top = int(rect.top()-_translationY) - (int(rect.top()-_translationY) % gridSize);

        QVarLengthArray<QLineF, 100> lines;

        for (qreal x = left; x < rect.right()-_translationX; x += gridSize)
            lines.append(QLineF(x+_translationX, rect.top(), x+_translationX, rect.bottom()));
        for (qreal y = top; y < rect.bottom()-_translationY; y += gridSize)
            lines.append(QLineF(rect.left(), y+_translationY, rect.right(), y+_translationY));

        //qDebug() << lines.size();
        painter->setPen(QPen(Qt::gray,0));
        painter->drawLines(lines.data(), lines.size());
    }

}

