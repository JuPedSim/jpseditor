/**
 * \file        jpsLineItem.h
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
 *
 * \section Description
 * This class is representing drawn line in the drawing section.
 *
 **/


#ifndef JPSLineItem_H
#define JPSLineItem_H

#include <QGraphicsView>
#include "global.h"

class jpsLineItem
{
public:
    jpsLineItem(QGraphicsLineItem* line);
    ~jpsLineItem();

    QGraphicsLineItem* get_line();
    void set_line(QGraphicsLineItem* line);
    int get_id();
    QString get_defaultColor();
    QString get_name();
    QString getType();
    void set_id(int id);
    void set_name(QString name);
//    void setType(bool wall, bool door, bool exit, bool hline=false);

    void setType(LineType type);

    void setWall();
    void setCrossing();
    void setHline();
    void setTransition();
    void setTrack();
    void set_defaultColor(QString color);
    bool is_Wall();
    bool is_Crossing();
    bool is_Transition();
    bool IsHLine();

    void add_intersectionPoint(QPointF *point);
    void remove_intersectionPoint(QPointF *point);
    QList<QPointF *> get_intersectionVector();
    QList<jpsLineItem* > get_intersectLineVector();
    void add_intersectLine(jpsLineItem * lineItem);
    void remove_interLine(jpsLineItem * lineItem);

private:
    QGraphicsLineItem* mLine;
    int mId;
    QString mName;
    QString defaultColor;
    LineType lineType;

    QList<QPointF *> intersectionVector;
    QList<jpsLineItem* > intersectLineVector;
};

#endif // JPSLineItem_H
