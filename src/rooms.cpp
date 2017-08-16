/**
 * \file        rooms.cpp
 * \date        Jun 26, 2015
 * \version     v0.8.1
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
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
 * This class represents a (sub)room. Walls and/or doors belonging to a certain room have to defined with the help of roomwidget.
 *
 *
 **/

#include "rooms.h"
#include <QtGui>
#include <iostream>
#include <fstream>
#include <QGraphicsLineItem>
#include "jpscrossing.h"


jpsRoom::jpsRoom(int id_room)
{
    id=id_room;
    QString string="Room ";
    name=string.append(QString::number(id));
    highlighted=false;
    _type="Not specified";
    _area=0.0;

}


void jpsRoom::addWall(QList <jpsLineItem *> newWalls)
{
    for (int i=0; i<newWalls.size(); ++i)
    {
        addWall(newWalls[i]);
    }

}

void jpsRoom::addWall(jpsLineItem *newWall)
{
    if (newWall->is_Wall() && !wall_list.contains(newWall))
    {
        wall_list.push_back(newWall);
    }
}

void jpsRoom::addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon)
{
    for (int i=0; i<newWalls.size(); ++i)
    {
        addinnerWall(newWalls[i],id_polygon);
    }
}

void jpsRoom::addinnerWall(jpsLineItem *newWall, int id_polygon)
{
    if (newWall->is_Wall() && !wall_list.contains(newWall))
    {
        if (id_polygon >= inner_polygons.size())
            inner_polygons.push_back(QVector<QLineF>{});

        wall_list.push_back(newWall);
        inner_polygons[id_polygon].push_back(newWall->get_line()->line());
    }
}

void jpsRoom::removeWall(QList <jpsLineItem *> wall)
{
    for (int i=0; i<wall.size(); i++)
    {
        wall_list.removeOne(wall[i]);
    }
}

const QList<jpsCrossing *>& jpsRoom::GetDoors() const
{
    return _doorList;
}

void jpsRoom::removeDoor(jpsCrossing *crossing)
{
    if (_doorList.contains(crossing))
    {
        _doorList.removeOne(crossing);
    }
}

QString jpsRoom::get_name()
{
    return name;
}

void jpsRoom::change_name(QString name)
{
    this->name=name;
}

QList<jpsLineItem *> jpsRoom::get_listWalls()
{
    return wall_list;
}

void jpsRoom::activate()
{
    for (signed int i=0; i<wall_list.size(); i++)
    {
        wall_list[i]->get_line()->setPen(QPen(Qt::yellow));
    }
}

//QVector<QPointF> jpsRoom::get_vertices() const
//{
//    QVector<QPointF> vertices;

//    for (int i=0; i<wall_list.size(); i++)
//    {
//        if (vertices.contains(wall_list[i]->get_line()->line().p1())==false)
//        {
//            vertices.push_back(wall_list[i]->get_line()->line().p1());
//        }
//        if (vertices.contains(wall_list[i]->get_line()->line().p2())==false)
//        {
//            vertices.push_back(wall_list[i]->get_line()->line().p2());
//        }
//    }
//    for (jpsCrossing* crossing:_doorList)
//    {
//        if (vertices.contains(crossing->get_cLine()->get_line()->line().p1())==false)
//        {
//            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
//        }

//        if (vertices.contains(crossing->get_cLine()->get_line()->line().p1())==false)
//        {
//            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
//        }

//    }

//    return vertices;
//}

QPointF jpsRoom::get_center()
{
    QVector<QPointF> vertices = RoomAsSortedPolygon(outer_polygon);
    qreal sum_x=0;
    qreal sum_y=0;

    for (int i=0; i<vertices.size(); ++i)
    {
        sum_x+=vertices[i].x();
        sum_y+=vertices[i].y();
    }
    QPointF mean;
    mean.setX(sum_x/vertices.size());
    mean.setY(sum_y/vertices.size());

    return mean;
}


void jpsRoom::highlight(const QString& color)
{
    if (!highlighted)
    {
        QPen pen;
        if (color=="darkMagenta")
            pen = QPen(Qt::darkMagenta,4);
        else if (color=="darkGreen")
            pen = QPen(Qt::darkGreen,4);
        else
            pen = QPen(rndColors(qrand()%100),4);
        pen.setCosmetic(true);
        for (int i=0; i<wall_list.size(); i++)
        {

            wall_list[i]->get_line()->setPen(pen);
            wall_list[i]->set_defaultColor(color);
        }
        highlighted=true;
    }
    else
    {
        QPen pen = QPen(Qt::black,2);
        pen.setCosmetic(true);
        for (int i=0; i<wall_list.size(); i++)
        {
            wall_list[i]->get_line()->setPen(pen);
            wall_list[i]->set_defaultColor("black");
        }
        highlighted=false;
    }
}

QString jpsRoom::get_type() const
{
    return _type;
}

void jpsRoom::set_type(const QString &string)
{
    _type=string;
}

QList<QPointF> jpsRoom::GetDoorVertices() const
{
    QList<QPointF> vertices;

    for (int i=0; i<wall_list.size(); i++)
    {
        int counterP1=0;
        int counterP2=0;
        for (int j=0; j<wall_list.size(); j++)
        {

            if (j!=i)
            {
                if (wall_list[i]->get_line()->line().p1()==wall_list[j]->get_line()->line().p1()
                        || wall_list[i]->get_line()->line().p1()==wall_list[j]->get_line()->line().p2())
                {
                    counterP1++;
                }
                else if (wall_list[i]->get_line()->line().p2()==wall_list[j]->get_line()->line().p1()
                        || wall_list[i]->get_line()->line().p2()==wall_list[j]->get_line()->line().p2())
                {
                    counterP2++;
                }
            }
        }
        if (counterP1==0)
        {
            if (!vertices.contains(wall_list[i]->get_line()->line().p1()))
            vertices.push_back(wall_list[i]->get_line()->line().p1());
        }
        else if (counterP2==0)
        {
            if (!vertices.contains(wall_list[i]->get_line()->line().p2()))
            vertices.push_back(wall_list[i]->get_line()->line().p2());
        }
    }

    return vertices;
}

void jpsRoom::AddDoor(jpsCrossing *door)
{
    if (!_doorList.contains(door))
        _doorList.push_back(door);
    //outer_polygon.push_back(door->get_cLine()->get_line()->line());
}

void jpsRoom::AddInnerDoor(jpsCrossing *door, int id_polygon)
{

    if (!_doorList.contains(door))
    {
        _doorList.push_back(door);
        if (id_polygon >= inner_polygons.size())
            inner_polygons.push_back(QVector<QLineF>{});

        inner_polygons[id_polygon].push_back(door->get_cLine()->get_line()->line());
    }
}

//QPolygonF jpsRoom::RoomAsPolygon() const
//{
//    return QPolygonF(get_vertices());
//}

QVector<QPointF> jpsRoom::RoomAsSortedPolygon(const QVector<QLineF>& lines) const
{
    QVector<QLineF> clines=lines;
    QVector<QPointF> points;

    points.push_back(lines.first().p1());
    points.push_back(lines.first().p2());

    clines.pop_front();

    for (int i=0; i<clines.size(); ++i)
    {
        if (clines[i].p1() == points.last())
        {
            //points.push_back(line->get_line()->line().p1());
            points.push_back(clines[i].p2());
            clines.remove(i);
            i=-1;
        }
        else if (clines[i].p2() == points.last())
        {
            //points.push_back(line->get_line()->line().p1());
            points.push_back(clines[i].p1());
            clines.remove(i);
            i=-1;
        }
    }


//    for (QPointF point:points)
//    {
//        std::cout << point.x() << " " << point.y() << std::endl;

//    }
//    std::cout << "----------------------------" << std::endl;


    return points;
}

const QVector<QLineF> &jpsRoom::GetOuterPolygon() const
{
    return outer_polygon;
}

qreal jpsRoom::GetArea() const
{
    return _area;
}

//void jpsRoom::GatherData() const
//{
//    QRectF bBox = CalculateBoundingBox();
//    int numberDoors = this->GetDoors().size();

//    int numberGreatDoors = 0; // number of doors greater than 1.25 meters
//    for (jpsCrossing* door:this->GetDoors())
//    {
//        if (door->get_cLine()->get_line()->line().length()>1.25)
//        {
//            ++numberGreatDoors;
//        }
//    }
//    // type; area; ratio bounding box- area; ratio bounding box

//    QString type = this->get_type();

//    std::ofstream data;
//    std::string filename = "C:/Users/Erik/Desktop/roomtypedata.txt";
//    data.open(filename, std::ofstream::out | std::ofstream::app);
//    data << type.toStdString() << "; " << std::to_string(_area) << "; "
//               << std::to_string((bBox.width()*bBox.height())/_area) << "; " << std::to_string(bBox.width()/bBox.height()) << "; "
//               << std::to_string(numberDoors) << std::endl;
//    data.close();
//}

//void jpsRoom::SortPolygon()
//{
//    sorted_polygon=RoomAsSortedPolygon(outer_polygon);
//}

void jpsRoom::IdentifyInnerOuter()
{

    QList<QLineF> lines;

    for (jpsLineItem* lineItem:wall_list)
    {
        lines.append(lineItem->get_line()->line());
    }
    for (jpsCrossing* crossing:_doorList)
    {
        lines.append(crossing->get_cLine()->get_line()->line());
    }

    QVector<QVector<QLineF>> polygons;

    while (!lines.empty())
    {
        polygons.push_back(QVector<QLineF>{});

        polygons.last().push_back(lines.first());
        lines.pop_front();

        bool found=true;

        while (found==true)
        {
            found=false;

            for (const QLineF& line:lines)
            {
                if (EqualsPoint(polygons.last().front().p2(),line.p1()))
                {
                    polygons.last().push_front(line);
                    lines.removeOne(line);
                    found=true;
                    break;
                }
                else if (EqualsPoint(polygons.last().front().p2(), line.p2()))
                {
                    polygons.last().push_front(QLineF(line.p2(),line.p1()));
                    lines.removeOne(line);
                    found=true;
                    break;
                }
                else if (EqualsPoint(polygons.last().front().p1(), line.p1()))
                {
                    polygons.last().push_front(QLineF(line.p2(),line.p1()));
                    lines.removeOne(line);
                    found=true;
                    break;
                }
                else if (EqualsPoint(polygons.last().front().p1(), line.p2()))
                {
                    polygons.last().push_front(line);
                    lines.removeOne(line);
                    found=true;
                    break;
                }
            }


        }
    }
    qreal maxArea=0.0;
    qreal sumArea=0.0;
    int n=0;
    int maxN=0;
    for (const QVector<QLineF>& poly:polygons)
    {
        qreal cArea=CalculateArea(poly);
        if (cArea>maxArea)
        {
            maxArea=cArea;
            maxN=n;
        }
        sumArea+=std::fabs(cArea);
        ++n;

    }

    _area=std::fabs(maxArea)-(sumArea-std::fabs(maxArea));
    //std::cout << _area << std::endl;
    outer_polygon=polygons[maxN];
    polygons.remove(maxN);

    inner_polygons=polygons;


}


QRectF jpsRoom::CalculateBoundingBox() const
{
    if (outer_polygon.empty())
        return QRectF();

    //xmin/xmax/ymin/ymax
    qreal x_min= outer_polygon.first().p1().x();
    qreal x_max= outer_polygon.first().p1().x();
    qreal y_min= outer_polygon.first().p1().y();
    qreal y_max= outer_polygon.first().p1().y();

    for (const QLineF& line:outer_polygon)
    {
        if (line.p1().x()<x_min)
            x_min=line.p1().x();
        else if (line.p1().x()>x_max)
            x_max=line.p1().x();
        if (line.p1().y()<y_min)
            y_min=line.p1().y();
        else if (line.p1().y()>y_max)
            y_max=line.p1().y();
    }

    //std::cout << x_min << x_max << y_min << y_max << std::endl;

    return QRectF(QPointF(x_min,y_min),QPointF(x_max,y_max));
}

qreal jpsRoom::CalculateArea(const QVector<QLineF>& poly) const
{
    if (poly.isEmpty())
        return 0.0;

//    QVector<QPointF> points = RoomAsSortedPolygon(poly);

    qreal area=0.0;
    int i, j=poly.size()-1;

    for (i=0; i<poly.size(); ++i)
    {
        area+=(poly[j].p1().x()+poly[i].p1().x())*(poly[j].p1().y()-poly[i].p1().y());
        j=i;
    }

    area=std::fabs(area*0.5);
    //std::cout << area << std::endl;

//    qreal sumInnerArea=0.0;
//    for (const QVector<QLineF>& vec:inner_polygons)
//    {
//        QVector<QPointF> innerPoints= RoomAsSortedPolygon(vec);


//        qreal innerArea=0.0;
//        int k, l=innerPoints.size()-1;

//        for (k=0; i<innerPoints.size(); ++k)
//        {
//            innerArea+=(innerPoints[l].x()+innerPoints[k].x())*(innerPoints[l].y()-innerPoints[k].y());
//            l=k;
//        }
//        innerArea=std::fabs(innerArea*0.5);
//        sumInnerArea+=innerArea;

//    }


    return area;
}

//bool jpsRoom::ContainsDoor(jpsLineItem *lineItem) const
//{
//    QList<QPointF> vertices = GetDoorVertices();

//    for (QPointF vertex:vertices)
//    {
//        if (lineItem->get_line()->contains(vertex))
//        {
//            return true;
//        }
//    }
//    return false;

//}



void jpsRoom::set_id(int id_room)
{
    id=id_room;
}

int jpsRoom::get_id()
{
    return id;
}

bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps)
{
    if (std::fabs(point1.x()-point2.x())<eps && std::fabs(point1.y()-point2.y())<eps)
        return true;
    else
        return false;
}

inline QColor rndColors(int seed){
    QColor color;
    float currentHue = 0.0;
    for (int i = 0; i < seed; i++){
        color= QColor::fromHslF(currentHue, 1.0, 0.5);
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    return color;
}
