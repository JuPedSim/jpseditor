/**
 * \file        jpszone.cpp
 * \date        Jun-19-2019
 * \version     v0.8.9
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
 * This class represents all zones in geometry, include room, subroom(corridor, office, lobby
 * entrance, stair, plattform) and obstacles
 *
 **/

#include <QtGui>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QGraphicsLineItem>

#include "jpszone.h"
#include "jpscrossing.h"
#include "jpsLineItem.h"

JPSZone::JPSZone(int id_zone, JPSZone *father, ZoneType type)
    : enterAndExitList(QList<jpsCrossing *>()), track_list(QList<JPSTrack *>()), wall_list(QList<jpsLineItem *>()),
    lobby_list(QList<JPSZone *>())
{
    id=id_zone;
    zoneType = type;
    father_room = father; // Room hasn't father room, subroom must have a room as father room

    name = "Unnamed " + QString::number(id_zone);

    highlighted=false;
    area_=0.0;

    A_x=0.0; //plane equation: A_x . x + B_y . y + C_z = z
    B_y=0.0;
    C_z=0.0;
    elevation_=0;
    visible=true;
}

bool JPSZone::isInWallList(jpsLineItem *wall)
{
    qDebug("Enter JPSZone::isInWallList");
    if(wall == nullptr)
        return false;

    foreach(jpsLineItem* wall_inList, wall_list)
    {
        if(wall == wall_inList)
            return true;
    }
    qDebug("Leave JPSZone::isInWallList");
    return false;
}

void JPSZone::addWall(QList <jpsLineItem *> newWalls)
{
    qDebug("Enter JPSZone::addWall)");
    for (int i=0; i<newWalls.size(); i++)
    {
        addWall(newWalls[i]);
    }
    qDebug("Leave JPSZone::addWall");
}

void JPSZone::addWall(jpsLineItem *newWall)
{
    qDebug("Enter JPSZone::addWall");
    if(newWall == nullptr)
        return;

    if (!isInWallList(newWall))
    {
        wall_list.push_back(newWall);
    }
    qDebug("Leave JPSZone::addWall");
}

void JPSZone::addinnerWall(QList<jpsLineItem *> newWalls, int id_polygon)
{
    for (int i=0; i<newWalls.size(); ++i)
    {
        addinnerWall(newWalls[i],id_polygon);
    }
}

void JPSZone::addinnerWall(jpsLineItem *newWall, int id_polygon)
{
    if (newWall->is_Wall() && !wall_list.contains(newWall))
    {
        if (id_polygon >= inner_polygons.size())
            inner_polygons.push_back(QVector<QLineF>{});

        wall_list.push_back(newWall);
        inner_polygons[id_polygon].push_back(newWall->get_line()->line());
    }
}

void JPSZone::removeWall(QList <jpsLineItem *> wall)
{
    qDebug("Enter JPSZone::removeWall");
    for (int i=0; i<wall.size(); i++)
    {
        wall_list.removeOne(wall[i]);
    }
    qDebug("Leave JPSZone::removeWall");
}

/*
    Purpose: Remove selecting wall from wall list

    Note: Wall won't be deleted in graphic view after removing
*/
void JPSZone::removeWall(jpsLineItem *wall)
{
    if (wall == nullptr)
        return;

    wall_list.removeOne(wall);
}

void JPSZone::removeTrack(JPSTrack *track)
{
    qDebug("Enter/Leave JPSZone::removeTrack");
    track_list.removeOne(track);
}

QList<jpsLineItem *> JPSZone::get_listWalls()
{
    qDebug("Enter/Leave JPSZone::get_listWalls");
    return wall_list;
}

QVector<QPointF> JPSZone::get_vertices() const
{
    QVector<QPointF> vertices;

    for (int i=0; i<wall_list.size(); i++)
    {
        if (!vertices.contains(wall_list[i]->get_line()->line().p1()))
        {
            vertices.push_back(wall_list[i]->get_line()->line().p1());
        }
        if (!vertices.contains(wall_list[i]->get_line()->line().p2()))
        {
            vertices.push_back(wall_list[i]->get_line()->line().p2());
        }
    }
    for (jpsCrossing* crossing:crossing_list)
    {
        if (!vertices.contains(crossing->get_cLine()->get_line()->line().p1()))
        {
            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
        }

        if (!vertices.contains(crossing->get_cLine()->get_line()->line().p1()))
        {
            vertices.push_back(crossing->get_cLine()->get_line()->line().p1());
        }
    }

    return vertices;
}

QPointF JPSZone::get_center()
{
    qDebug() << "Enter JPSZone::get_center";
    IdentifyInnerOuter(); // write outer_polygon

    if(outer_polygon.isEmpty())
    {
        QPointF  origin(0,0);
        return origin;
    }

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

    qDebug() << "Leave JPSZone::get_center";
    return mean;
}

void JPSZone::highlight(const QString& color)
{
     qDebug("Enter JPSZone::highlight. highlighted=<%d>", highlighted);

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

        for (int i=0; i<wall_list.size(); ++i)
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
    qDebug("Enter JPSZone::highlight. highlight=<%d>", highlighted);
}

void JPSZone::switchVisibility()
{
    qDebug("Enter JPSZone::switchVisibility. visible=<%d>", visible);
    if(!visible)
    {
        QColor color(0,0,0,0);
        QPen pen(color);

        for (int i=0; i<wall_list.size(); i++)
        {
            wall_list[i]->get_line()->setPen(pen);
            wall_list[i]->set_defaultColor("white");
        }
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
    }
    qDebug("Exit JPSZone::switchVisibility. visible=<%d>", visible);
}


ZoneType JPSZone::getType() const
{
    return zoneType;
}

void JPSZone::setType(const ZoneType &type)
{
    zoneType=type;
}

QList<QPointF> JPSZone::GetDoorVertices() const
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

void JPSZone::AddDoor(jpsCrossing *door)
{
    if (!crossing_list.contains(door))
        crossing_list.push_back(door);
    //outer_polygon.push_back(door->get_cLine()->get_line()->line());
}

void JPSZone::AddInnerDoor(jpsCrossing *door, int id_polygon)
{

    if (!crossing_list.contains(door))
    {
        crossing_list.push_back(door);
        if (id_polygon >= inner_polygons.size())
            inner_polygons.push_back(QVector<QLineF>{});

        inner_polygons[id_polygon].push_back(door->get_cLine()->get_line()->line());
    }
}

//QPolygonF JPSZone::RoomAsPolygon() const
//{
//    return QPolygonF(get_vertices());
//}

QVector<QPointF> JPSZone::RoomAsSortedPolygon(const QVector<QLineF>& lines) const
{
     qDebug("Enter JPSZone::RoomAsSortedPolygon");
    QVector<QLineF> clines=lines;
    QVector<QPointF> points = {};

    if(lines.empty())
    {
         qDebug("\t empty lines!!");
         qDebug("Leave JPSZone::RoomAsSortedPolygon with ERROR");
         return points;
         
    }

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

    qDebug("Leave JPSZone::RoomAsSortedPolygon");
    return points;
}

const QVector<QLineF> &JPSZone::GetOuterPolygon() const
{
    return outer_polygon;
}

qreal JPSZone::GetArea() const
{
    return area_;
}

void JPSZone::IdentifyInnerOuter()
{
    qDebug() << "Enter JPSZone::IdentifyInnerOuter";
    QList<QLineF> lines;


    if (!wall_list.isEmpty()){
        for (jpsLineItem* lineItem:wall_list)
        {
            lines.append(lineItem->get_line()->line());
        }
        for (jpsCrossing* crossing:crossing_list)
        {
            lines.append(crossing->get_cLine()->get_line()->line());
        }
    } else{
        outer_polygon = {};
        return;
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

    area_=std::fabs(maxArea)-(sumArea-std::fabs(maxArea));
    //std::cout << area_ << std::endl;
    outer_polygon=polygons[maxN];
    polygons.remove(maxN);

    inner_polygons=polygons;

    qDebug() << "Leave JPSZone::IdentifyInnerOuter";
}


QRectF JPSZone::CalculateBoundingBox() const
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

qreal JPSZone::CalculateArea(const QVector<QLineF>& poly) const
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

    return area;
}


void JPSZone::set_id(int id_room)
{
    id=id_room;
}

int JPSZone::get_id()
{
    return id;
}
float JPSZone::get_ax()
{
     return A_x;
}
void JPSZone::set_ax(float AX)
{
     A_x=AX;
}

float JPSZone::get_by()
{
     return B_y;
}
void JPSZone::set_by(float BY)
{
     B_y=BY;
}

float JPSZone::get_cz()
{
     return C_z;
}
void JPSZone::set_cz(float CZ)
{
     C_z=CZ;
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

QPointF JPSZone::get_up()
{
     return up_;
}

void JPSZone::set_up(QPointF up)
{
     up_ = up;
}

QPointF JPSZone::get_down()
{
     return down_;
}

void JPSZone::set_down(QPointF down)
{
     down_ = down;
}

void JPSZone::switchHighlight()
{
    highlighted = !highlighted;
}

float JPSZone::get_elevation()
{
     return elevation_;
}

void JPSZone::set_elevation(float elevation)
{
     if(this->getType() != Stair) // only for horizontal floors
          for (auto crossing: crossing_list)
               crossing->set_elevation(elevation);

     elevation_ = elevation;
}

void JPSZone::correctPlaneCoefficients()
{
     qDebug("Enter correctPlaneCoefficients");
     qDebug("\t room=<%s> of type=<%s> has %d doors",
            this->getName().toStdString().c_str(),
            QString(this->getType()).toStdString().c_str(),
            (int)crossing_list.size());
    if(crossing_list.isEmpty() || this->getType() != Stair)
    {
        this->set_ax(0);
        this->set_by(0);
        this->set_cz(this->get_elevation());
        return;
    }

    QPointF P1(0,0), P2(0,0), P3(0,0); // plane is defined by three non-collinear points
    float elevation_1=0, elevation_2=0;
    // P1 P2 are the points on the first door of a room
    P1 = crossing_list[0]->get_cLine()->get_line()->line().p1();
    P2 = crossing_list[0]->get_cLine()->get_line()->line().p2();
    elevation_1 = crossing_list[0]->get_elevation();

    //from _doortList get one more point on a door with different elevation as P3
    for (int i=1; i < crossing_list.size(); i++)
    {
      if(crossing_list[i]->get_elevation() != crossing_list[0]->get_elevation()){
           P3 = crossing_list[i]->get_cLine()->get_line()->line().p1();
           elevation_2 = crossing_list[i]->get_elevation();
           break;
      }
    }

     // variables for convenience
     float P1_x = P1.x();
     float P1_y = P1.y();
     float P1_z = elevation_1;

     float P2_x = P2.x();
     float P2_y = P2.y();
     float P2_z = elevation_1;

     float P3_x = P3.x();
     float P3_y = P3.y();
     float P3_z = elevation_2;
     qDebug("\t P1=(%.2f, %.2f, %.2f), P2=(%.2f, %.2f, %.2f), P3=(%.2f, %.2f, %.2f)",
            P1_x, P1_y, P1_z,
            P2_x, P2_y, P2_z,
            P3_x, P3_y, P3_z
          );
     float d = 1.0;
     // Thanks M. Osterkamp
     float c = (((1-P3_x/P1_x*1)-((P3_y-P3_x/P1_x*P1_y)/(P2_y-P2_x/P1_x*P1_y))
                 *(1-P2_x/P1_x*1))/((P3_z-P3_x/P1_x*P1_z)
                                    -((P3_y-P3_x/P1_x*P1_y)/(P2_y-P2_x/P1_x*P1_y))
                                    *(P2_z-P2_x/P1_x*P1_z)))*d;

     float b = ((1-P3_x/P1_x*1)*d-(P3_z-P3_x/P1_x*P1_z)*c)/(P3_y-P3_x/P1_x*P1_y);
     float a = (d-P1_z*c-P1_y*b)/P1_x;
     qDebug("\t a = %.2f, b = %.2f, c= %.2f", a, b, c);
     set_ax(-a/c);
     set_by(-b/c);
     set_cz(d/c);
     qDebug("\t ax = %.2f, by = %.2f, cz= %.2f", -a/c, -b/c, d/c);
     qDebug("Leave correctPlaneCoefficients");
}

bool JPSZone::is_highlighted()
{
     return highlighted;
}

bool JPSZone::isVisible()
{
    return visible;
}

void JPSZone::setVisible(bool visibility)
{
    visible = visibility;
}

JPSZone *JPSZone::getFatherRoom()
{
    qDebug("Enter/Leave JPSZone::getFatherRoom");
    return father_room;
}

void JPSZone::setFatherRoom(JPSZone *room)
{
    qDebug("Enter JPSZone::setFatherRoom");
    father_room = room;
    qDebug("Leave JPSZone::setFatherRoom");
}

QString JPSZone::getName() const
{
    return name;
}

void JPSZone::setName(const QString &name) {
    qDebug("Enter JPSZone::setName");
    this->name = name;
    qDebug("Name changed. Leave JPSZone::setName");
}

QList<JPSTrack *> JPSZone::getTrackList() const
{
    qDebug("Enter/Leave JPSZone::getTrackList");
    return track_list;
}

void JPSZone::addTrack(jpsLineItem *line, QString number)
{
    qDebug("Enter JPSZone::addTrack");
    if(line == nullptr)
        return;

    auto *track = new JPSTrack(line);
    track->setType(number.toInt());

    if(!isInTrackList(track))
    {
        track_list.append(track);
    }
    qDebug("Leave JPSZone::addTrack");
}

/*
    Von v0.8.9

    Check the track if it's already added in the zone.
 */

bool JPSZone::isInTrackList(JPSTrack *track) {
    qDebug("Enter JPSZone::isInTrackList");
    if(track == nullptr)
        return false;

    foreach(JPSTrack *track_inList, track_list)
    {
        if(track->getLine() == track_inList->getLine()) //if same jpsLineItem, reture true;
            return true;
    }

    qDebug("Leave JPSZone::isInTrackList");
    return false;
}

const QList<JPSZone *> &JPSZone::getPlatfromList() const
{
    qDebug("Enter/Leave JPSZone::getPlatfromList");
    return platfrom_list;
}

void JPSZone::addZoneInList(JPSZone *zone)
{
    qDebug("Enter JPSZone::addZoneInList");
    ZoneType type = zone->getType();

    switch (type)
    {
        case Corridor:
            corridor_list.append(zone);
            break;
        case Platform:
            platfrom_list.append(zone);
            break;
        case Lobby:
            lobby_list.append(zone);
        case Office:
            office_list.append(zone);
        default:
            break;
    }
    qDebug("Leave JPSZone::addZoneInList");
}

void JPSZone::removeZoneFromList(JPSZone *zone)
{
    qDebug("Enter JPSZone::removeZoneFromList");
    ZoneType type = zone->getType();

    switch (type)
    {
        case Corridor:
            corridor_list.removeOne(zone);
            delete zone;
            zone = nullptr;
            break;
        case Platform:
            platfrom_list.removeOne(zone);
            delete zone;
            zone = nullptr;
            break;
        case Lobby:
            lobby_list.removeOne(zone);
            delete zone;
            zone = nullptr;
            break;
        case Office:
            office_list.removeOne(zone);
            delete zone;
            zone = nullptr;
            break;
        default:
            break;
    }
    qDebug("Leave JPSZone::removeZoneFromList");
}

const QList<JPSZone*> &JPSZone::getCorridorList() const
{
    qDebug("Enter/Leave JPSZone::getCorridorList");
    return corridor_list;
}

// Crossing
QList<jpsCrossing*> JPSZone::getCrossingList()
{
    qDebug("Enter/Leave JPSZone::getCrossingList");
    return crossing_list;
}

bool JPSZone::isInCrossingList(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::isInCrossingList");
    if(crossing == nullptr)
        return false;

    foreach(jpsCrossing *crossing_inList, crossing_list)
    {
        if(crossing->get_cLine() == crossing_inList->get_cLine())
            return true;
    }
    qDebug("Leave JPSZone::isInCrossingList");
    return false;
}

// Add crossing in room(crossing list)
void JPSZone::addCrossing(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::addCrossing");
    if(crossing == nullptr)
        return;

    if(!isInCrossingList(crossing))
        crossing_list.push_back(crossing);
    qDebug("Leave JPSZone::addCrossing");
}

void JPSZone::removeCrossing(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::removeCrossing");
    if(crossing == nullptr)
        return;

    crossing_list.removeOne(crossing);
    qDebug("Leave JPSZone::removeCrossing");
}

void JPSZone::addInEnterAndExitList(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::addInEnterAndExitList");
    if(crossing != nullptr && !isInEnterAndExitList(crossing) && crossing->get_roomList().size() < 2)
    {
        enterAndExitList.append(crossing);
        crossing->setSubroom(this); // Set this subroom as one side of crossing;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The crossing has been added for two subrooms.");
        msgBox.setInformativeText("Remove it from one subroom at first");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Ok:
                // Ok was clicked
                break;
            default:
                // should never be reached
                break;
        }
        qDebug("Crossing is problematic, Leave JPSZone::addInEnterAndExitList");
        return;
    }

    qDebug("Leave JPSZone::addInEnterAndExitList");
}

void JPSZone::removeEnterOrExit(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::removeEnterOrExit");
    if(crossing != nullptr && isInEnterAndExitList(crossing))
    {
        enterAndExitList.removeOne(crossing);
    }
    else
    {
        qDebug("Crossing is problematic, Leave JPSZone::addInEnterAndExitList");
        return;
    }
    qDebug("Leave JPSZone::removeEnterOrExit");
}

QList<jpsCrossing *> JPSZone::getEnterAndExitList()
{
    qDebug("Enter/Leave JPSZone::getEnterAndExitList()");
    return enterAndExitList;
}

bool JPSZone::isInEnterAndExitList(jpsCrossing *crossing)
{
    qDebug("Enter JPSZone::isInEnterAndExitList");
    if(crossing == nullptr)
        return false;

    foreach(jpsCrossing *crossing_inList, enterAndExitList)
    {
        if(crossing->get_cLine() == crossing_inList->get_cLine())
            return true;
    }
    qDebug("Leave JPSZone::isInEnterAndExitList");
    return false;
}

jpsCrossing *JPSZone::getCrossingFromList(jpsLineItem *line)
{
    // If jpsLineItem is already added into a room, it should be in crossing_list
    qDebug("Enter JPSZone::getCrossingFromList");
    foreach(jpsCrossing *crossing, crossing_list)
    {
        if(line == crossing->get_cLine())
        {
            qDebug("Leave JPSZone::getCrossingFromList");
            return crossing;
        } else
        {
            qDebug("Not in list, Leave JPSZone::getCrossingFromList");
            return nullptr;
        }
    }

    return nullptr;
}

const QList<JPSZone*> & JPSZone::getLobbyList() const
{
    qDebug("Enter JPSZone::getLobbyList()");
    return lobby_list;
    qDebug("Leave JPSZone::getLobbyList()");
}

const QList<JPSZone*> & JPSZone::getOfficeList() const
{
    qDebug("Enter JPSZone::getOfficeList()");
    return office_list;
    qDebug("Leave JPSZone::getOfficeList()");
}
