/**
 * \file       graphicscene.cpp 
 * \date        Oct-01-2018
 * \version     v0.8.4
 * \copyright   <2009-2018> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 **/

     
#include "graphicscene.h"
#include <QVarLengthArray>
#include <cmath>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QList>


GraphicScene::GraphicScene(QObject *parent) : QGraphicsScene(parent)
{
    qDebug("Enter GraphicScene::GraphicScene");
    _translationX=0.0;
    _translationY=0.0;
    _gridmode=false;
    _statgrid="Line";
    _gridSize=1.0;
    qDebug("Leave GraphicScene::GraphicScene");
}

GraphicScene::~GraphicScene()
{

}

void GraphicScene::ChangeGridmode(const bool &stat)
{
    qDebug("Enter/Return ChangeGridmode");
    _gridmode=stat;
}

bool GraphicScene::GetGridmode() const
{
    qDebug("Enter/Return GraphicScene::GetGridmode");
    return _gridmode;
}

void GraphicScene::ChangeTranslation(qreal x, qreal y)
{
    qDebug("Enter GraphicScene::ChangeTranslation");
    _translationX=x;
    _translationY=y;
    qDebug("Leave GraphicScene::ChangeTranslation");
}

void GraphicScene::SetGrid(QString grid)
{
    qDebug("Enter/Return GraphicScene::SetGrid");
    _statgrid=grid;
}

void GraphicScene::ChangeGridSize(const qreal &gridSize)
{
    qDebug("Enter/Return GraphicScene::ChangeGridSize");
    _gridSize=gridSize;
}

void GraphicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    qDebug("Enter GraphicScene::drawBackground");
    if (_gridmode)
    {
        if (_statgrid=="Line")
            DrawLineGrid(painter,rect);
        else
            DrawPointGrid(painter,rect);
    }
    qDebug("Leave GraphicScene::drawBackground");
}

void GraphicScene::DrawLineGrid(QPainter *painter, const QRectF &rect)
{
    qDebug("Enter GraphicScene::DrawLineGrid");
    //gridSize=1.0;
    qreal left = int(rect.left()-_translationX) - std::fmod(int(rect.left()-_translationX), _gridSize);
    qreal top = int(rect.top()-_translationY)- std::fmod(int(rect.top()-_translationY) , _gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right()-_translationX; x += _gridSize)
        lines.append(QLineF(x+_translationX, rect.top(), x+_translationX, rect.bottom()));
    for (qreal y = top; y < rect.bottom()-_translationY; y += _gridSize)
        lines.append(QLineF(rect.left(), y+_translationY, rect.right(), y+_translationY));

    QRectF origin(-0.5,-0.5,1,1);
    QLineF xaxis(0,0,0,100000);
    QLineF yaxis(0,0,100000,0);

    //qDebug() << lines.size();
    painter->setPen(QPen(Qt::gray,0));
    painter->drawLines(lines.data(), lines.size());

    //draw orgin and x y axis
//    painter->setPen(QPen(Qt::red,0));
//    painter->drawRect(origin);
//    painter->fillRect(origin, Qt::red);
//    painter->drawLine(xaxis);
//    painter->drawLine(yaxis);
    qDebug("Leave GraphicScene::DrawLineGrid");
}

void GraphicScene::DrawPointGrid(QPainter *painter, const QRectF &rect)
{
    qDebug("Enter GraphicScene::DrawPointGrid");

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
    qDebug("Leave GraphicScene::DrawPointGrid");
}
