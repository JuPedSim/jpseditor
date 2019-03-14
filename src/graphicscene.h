/**
 * \file       graphicscene.h 
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

     
#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include "jpssource.h"

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:

    GraphicScene(QObject* parent=nullptr);
    GraphicScene(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsScene(x, y, w, h)
    { }
    ~GraphicScene();
    void ChangeGridmode(const bool& stat);
    bool GetGridmode() const;
    void ChangeTranslation(qreal x, qreal y);
    void SetGrid(QString grid);
    void ChangeGridSize(const qreal& factor);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void DrawLineGrid(QPainter *painter, const QRectF &rect);
    void DrawPointGrid(QPainter *painter, const QRectF &rect);

private:
    bool _gridmode;
    qreal _translationX;
    qreal _translationY;
    QString _statgrid;
    qreal _gridSize;

};

#endif // GRAPHICSCENE_H
