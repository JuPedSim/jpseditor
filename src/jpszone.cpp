#include <QtGui>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QGraphicsLineItem>

#include "jpszone.h"
#include "jpsLineItem.h"
#include "jpstransition.h"

JPSZone::JPSZone(int id_zone, ZoneType type)
    : track_list(QList<JPSTrack *>()), wall_list(QList<jpsLineItem *>())
{
    qDebug("Enter JPSZone::JPSZone");
    id=id_zone;
    zoneType = type;

    name = getTypeInString() + QString::number(id_zone);

    highlighted=false;
    area_=0.0;

    A_x=0.0; //plane equation: A_x . x + B_y . y + C_z = z
    B_y=0.0;
    C_z=0.0;
    elevation_=0;
    visible=true;
    qDebug("Return JPSZone::JPSZone");
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
    qDebug("Enter JPSZone::addinnerWall");
    for (int i=0; i<newWalls.size(); ++i)
    {
        addinnerWall(newWalls[i],id_polygon);
    }
    qDebug("Leave JPSZone::addinnerWall");
}

void JPSZone::addinnerWall(jpsLineItem *newWall, int id_polygon)
{
    qDebug("Enter JPSZone::addinnerWall");
    if (newWall->is_Wall() && !wall_list.contains(newWall))
    {
        if (id_polygon >= inner_polygons.size())
            inner_polygons.push_back(QVector<QLineF>{});

        wall_list.push_back(newWall);
        inner_polygons[id_polygon].push_back(newWall->get_line()->line());
    }
    qDebug("Leave JPSZone::addinnerWall");
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

    Note: WallMode won't be deleted in graphic view after removing
*/
void JPSZone::removeWall(jpsLineItem *wall)
{
    qDebug("Enter JPSZone::removeWall");
    if (wall == nullptr)
    {
        qDebug("WallMode is nullptr, Leave JPSZone::removeWall");
        return;
    }

    bool result = wall_list.removeOne(wall);

    if(result)
        qDebug("JPSZone::removeWall Deleted wall %d", wall->get_id());

    qDebug("Leave JPSZone::removeWall");
}

void JPSZone::removeTrack(JPSTrack *track)
{
    qDebug("Enter JPSZone::removeTrack");
    track_list.removeOne(track);
    qDebug("Leave JPSZone::removeTrack");
}

QList<jpsLineItem *> JPSZone::get_listWalls()
{
    qDebug("Enter/Return JPSZone::get_listWalls");
    return wall_list;
}

QVector<QPointF> JPSZone::get_vertices() const
{
    qDebug("Enter JPSZone::get_vertices");
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
    qDebug("Leave JPSZone::get_vertices");
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
    qDebug("Enter/Return ZoneType JPSZone::getType");
    return zoneType;
}

void JPSZone::setType(const ZoneType &type)
{
    qDebug("Enter JPSZone::setType");
    zoneType=type;
    qDebug("Leave JPSZone::setType");
}

QList<QPointF> JPSZone::GetDoorVertices() const
{
    qDebug("Enter JPSZone::GetDoorVertices");
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
    qDebug("Leave JPSZone::GetDoorVertices");
    return vertices;
}

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
    qDebug("Enter/Return JPSZone::GetOuterPolygon");
    return outer_polygon;
}

qreal JPSZone::GetArea() const
{
    qDebug("Enter/Return JPSZone::GetArea");
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
    qDebug("Enter JPSZone::CalculateBoundingBox");
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
    qDebug("Leave JPSZone::CalculateBoundingBox");
    return QRectF(QPointF(x_min,y_min),QPointF(x_max,y_max));
}

qreal JPSZone::CalculateArea(const QVector<QLineF>& poly) const
{
    qDebug("Enter JPSZone::CalculateArea");
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
    qDebug("Leave JPSZone::CalculateArea");
    return area;
}


void JPSZone::set_id(int id_room)
{
    qDebug("Enter JPSZone::set_id");
    id=id_room;
    qDebug("Leave JPSZone::set_id");
}

int JPSZone::get_id()
{
    qDebug("Enter/Return JPSZone::get_id");
    return id;
}
float JPSZone::get_ax()
{
    qDebug("Enter/Return JPSZone::get_ax");
     return A_x;
}
void JPSZone::set_ax(float AX)
{
    qDebug("Enter JPSZone::set_ax");
     A_x=AX;
}

float JPSZone::get_by()
{
    qDebug("Enter/Return JPSZone::get_by");
     return B_y;
}
void JPSZone::set_by(float BY)
{
    qDebug("Enter JPSZone::set_by");
    B_y=BY;
    qDebug("Leave JPSZone::set_by");
}

float JPSZone::get_cz()
{
    qDebug("Enter/Retrun JPSZone::get_cz");
     return C_z;
}
void JPSZone::set_cz(float CZ)
{
    qDebug("Enter JPSZone::set_cz");
     C_z=CZ;
     qDebug("Leave JPSZone::set_cz");
}


bool EqualsPoint(const QPointF& point1, const QPointF& point2, double eps)
{
    qDebug("Enter EqualsPoint");
    if (std::fabs(point1.x()-point2.x())<eps && std::fabs(point1.y()-point2.y())<eps){
        qDebug("Leave EqualsPoint");
        return true;
    }
    else
        qDebug("Leave EqualsPoint");
        return false;
}

inline QColor rndColors(int seed){
    qDebug("Enter rndColors");
    QColor color;
    float currentHue = 0.0;
    for (int i = 0; i < seed; i++){
        color= QColor::fromHslF(currentHue, 1.0, 0.5);
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    qDebug("Leave rndColors");
    return color;
}

QPointF JPSZone::get_up()
{
    qDebug("Enter/Return JPSZone::get_up");
     return up_;
}

void JPSZone::set_up(QPointF up)
{
    qDebug("Enter JPSZone::set_up");
     up_ = up;
     qDebug("Leave JPSZone::set_up");
}

QPointF JPSZone::get_down()
{
    qDebug("Enter JPSZone::get_down");
     return down_;
     qDebug("Leave JPSZone::get_down");
}

void JPSZone::set_down(QPointF down)
{
    qDebug("Enter JPSZone::set_down");
     down_ = down;
     qDebug("Leave JPSZone::set_down");
}

void JPSZone::switchHighlight()
{
    qDebug("Enter JPSZone::switchHighlight");
    highlighted = !highlighted;
    qDebug("Leave JPSZone::switchHighlight");
}

float JPSZone::get_elevation()
{
    qDebug("Enter/Return JPSZone::get_elevation");
     return elevation_;
}

void JPSZone::set_elevation(float elevation)
{
    qDebug("Enter JPSZone::set_elevation");

     elevation_ = elevation;

     qDebug("Leave JPSZone::set_elevation");
}

void JPSZone::correctPlaneCoefficients(QList<jpsTransition *> transitions)
{
     qDebug("Enter correctPlaneCoefficients");

     qDebug("\t room=<%s> of type=<%s> has %d transitions",
            this->getName().toStdString().c_str(),
            QString(this->getType()).toStdString().c_str(),
            (int)transitions.size());

    if(transitions.isEmpty() || this->getType() != Stair)
    {
        this->set_ax(0);
        this->set_by(0);
        this->set_cz(this->get_elevation());
        return;
    }

    QPointF P1(0,0), P2(0,0), P3(0,0); // plane is defined by three non-collinear points
    float elevation_1=0, elevation_2=0;
    // P1 P2 are the points on the first door of a room
    P1 = transitions[0]->get_cLine()->get_line()->line().p1();
    P2 = transitions[0]->get_cLine()->get_line()->line().p2();
    elevation_1 = transitions[0]->getElevation();

    // Get one more point on a door with different elevation as P3
    for (int i=1; i < transitions.size(); i++)
    {
      if(transitions[i]->getElevation() != transitions[0]->getElevation()){
           P3 = transitions[i]->get_cLine()->get_line()->line().p1();
           elevation_2 = transitions[i]->getElevation();
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
    qDebug("Enter/Return JPSZone::is_highlighted");
     return highlighted;
}

bool JPSZone::isVisible()
{
    qDebug("Enter/Return JPSZone::isVisible");
    return visible;
}

void JPSZone::setVisible(bool visibility)
{
    qDebug("Enter JPSZone::setVisible");
    visible = visibility;
    qDebug("Leave JPSZone::setVisible");
}

QString JPSZone::getName() const
{
    qDebug("Enter/Return JPSZone::getName");
    return name;
}

void JPSZone::setName(const QString &name) {
    qDebug("Enter JPSZone::setName");
    this->name = name;
    qDebug("Name changed. Leave JPSZone::setName");
}

QList<JPSTrack *> JPSZone::getTrackList() const
{
    qDebug("Enter/Return JPSZone::getTrackList");
    return track_list;
}

void JPSZone::addTrack(jpsLineItem *line, QString number)
{
    qDebug("Enter JPSZone::addTrack");
    if(line == nullptr)
    {
        qDebug("var line ist nullptr");
        qDebug("Leave JPSZone::addTrack");
        return;
    }

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
        qDebug("var track is nullptr");
        qDebug("Leave JPSZone::isInTrackList");
        return false;

    foreach(JPSTrack *track_inList, track_list)
    {
        if(track->getLine() == track_inList->getLine()) //if same jpsLineItem, reture true;
            return true;
    }

    qDebug("Leave JPSZone::isInTrackList(false)");
    return false;
}

QString JPSZone::getTypeInString() const
{
    qDebug("Enter JPSZone::getZoneList");
    switch (zoneType)
    {
        case Stair:
            return "stair";
        case Platform:
            return "platform";
        default:
            return "subroom";
    }
}
