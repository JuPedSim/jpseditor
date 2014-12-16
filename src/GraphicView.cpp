
#include <QtGui>
#include <QGraphicsLineItem>
#include "GraphicView.h"
#include <iostream>


jpsGraphicsView::jpsGraphicsView(QWidget* parent, jpsDatamanager* dmanager):QGraphicsView(parent)
{

    datamanager=dmanager;
    current_line=0L;
    //current_line_mark=0L;
    midbutton_hold=false;
    translation_x=0;
    translation_y=0;
    gridmode=false;
    gl_scale_f=1.0;
    catch_radius=10*gl_scale_f;
    catch_line_distance=10*gl_scale_f;
    //Set-up the view
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    point_tracked=false;
    line_tracked=-1;
    current_rect=0L;
    objectsnap=false;
    statWall=false;
    statDoor=false;
    statExit=false;
    currentPen.setColor(Qt::black);

    gl_min_x=1e6;
    gl_min_y=1e6;

    //m_graphView->setFixedSize(1600, 900);
    //m_graphView->setScene(m_graphScen);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // setRenderHint(QPainter::Antialiasing);

    //m_graphView->setAlignment(0L);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCursor(Qt::CrossCursor);

    //Set-up the scene
    Scene = new QGraphicsScene(this);
    setScene(Scene);
    setSceneRect(0, 0, 1920, 1080);
}


void jpsGraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{

    QGraphicsView::mouseMoveEvent(mouseEvent);

    if (current_rect!=0L)

    {

       delete current_rect;
       current_rect=0L;

    }
    //setMouseTracking(true);
    //setResizeAnchor(this->AnchorUnderMouse);

    QPointF old_pos=pos;

    pos=mapToScene(mouseEvent->pos());

    if (current_line!=0L)
    {
        if (gridmode==true)
        {
                // gridmode
            use_gridmode();

        }
    }



    translated_pos.setX(pos.x()-translation_x);
    translated_pos.setY(pos.y()-translation_y);

    if (midbutton_hold==true)
    {
        translation_x+=pos.x()-old_pos.x();
        translation_y+=pos.y()-old_pos.y();

        if (current_line!=0L)
        {
            current_line->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
            //current_line->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());

        }

        for (int i=0; i<line_vector.size(); i++)
        {
            //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
            line_vector[i]->get_line()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);


        }
        //if (current_line_mark!=0L)
        //{
        //    current_line_mark->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        //}
        //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        //translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
       // setDragMode(ScrollHandDrag);

    }

    translated_pos.setX(pos.x()-translation_x);
    translated_pos.setY(pos.y()-translation_y);



    if (objectsnap==true)
    {

        catch_points();

    }

    if (current_line!=0L)
    {

        if (statWall==true || statDoor==true || statExit==true)
        {
            emit set_focus_textedit();
            current_line->setLine(current_line->line().x1(),current_line->line().y1(),translated_pos.x(),translated_pos.y());

        }
    }
    emit mouse_moved();
    update();

}


void jpsGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
{


    //pos=mapToScene(mouseEvent->pos());

    if (mouseEvent->button() == Qt::LeftButton)
    {

    // if the mouse was pressed first of two times
    // new JPGeoLine is inited
    // all two points of the line are inited with the cursorcoordinates
        if (statWall==true || statDoor==true || statExit==true)
        {
            if (current_line==0L)
            {
                current_line = Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
                //current_line->translate(translation_x,translation_y);
                current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);

            }

            // if the mouse was pressed secondly of two times
            else
            {
                if (current_line->line().p1().x()<gl_min_x)
                {
                    gl_min_x=current_line->line().p1().x();
                }

                if (current_line->line().p2().x()<gl_min_x)
                {
                    gl_min_x=current_line->line().p2().x();
                }

                if (current_line->line().p1().y()<gl_min_y)
                {
                    gl_min_y=current_line->line().p1().y();
                }

                if (current_line->line().p2().y()<gl_min_y)
                {
                    gl_min_y=current_line->line().p2().y();
                }
                jpsLineItem* lineItem= new jpsLineItem(current_line);

                // if there is already a drawn line
                if (line_vector.size()>=1)
                {
                    // Searching for intersection of the current line with all already drawn lines
                    for (signed int i=0; i<line_vector.size(); i++)
                    {
                        //this pointer is necessary due to the architecture of the method 'intersect'
                       QPointF* intersection_point = new QPointF;
                        // if 'intersect'==1 -> an intersection point exists
                        if (lineItem->get_line()->line().intersect(line_vector[i]->get_line()->line(),intersection_point)==1)
                        {
                            // attaching current intersection point
                            intersect_point_vector.push_back(intersection_point);
                            lineItem->add_intersectionPoint(intersection_point);
                            line_vector[i]->add_intersectionPoint(intersection_point);

                            // Free memory
                            //delete intersection_point;
                            // resetting pointer
                            intersection_point=0L;
                        }
                    }
                }
                // completed line is appended to "line_vector"


                //std::cout << current_line->line().x1() << std::endl;



                lineItem->set_type(statWall,statDoor,statExit);
                line_vector.push_back(lineItem);



                // pointer "current_line" is reset
                current_line=0L;
            }
        }
        else //if statWall==false
        {

            catch_lines();

        }
        //unmark selected line(s)
        if (line_tracked==-1)
        {
            unmark_all_lines();
        }
    }
    else if (mouseEvent->button()==Qt::MidButton)
    {
        midbutton_hold=true;

    }

    update();
}



void jpsGraphicsView::unmark_all_lines()
{

    for (int i=0; i<marked_lines.size();i++)
    {
        if (marked_lines[i]->is_Wall()==true)
        {
            marked_lines[i]->get_line()->setPen(QPen(Qt::black,0));
        }
        else if (marked_lines[i]->is_Door()==true)
        {
            marked_lines[i]->get_line()->setPen(QPen(Qt::blue,0));
        }
        else
        {
            marked_lines[i]->get_line()->setPen(QPen(Qt::darkMagenta,0));
        }
    }
    marked_lines.clear();
}


void jpsGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (midbutton_hold==false)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        // Scale the view / do the zoom
        double scaleFactor = 1.15;
        if(event->delta() > 0)
        {
            // Zoom in
            gl_scale_f*=1/1.15;
            scale(scaleFactor, scaleFactor);
            catch_radius=10*gl_scale_f;
            catch_line_distance=10*gl_scale_f;
        } else
        {
            // Zooming out
            gl_scale_f*=1.15;
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            catch_radius=10*gl_scale_f;
            catch_line_distance=10*gl_scale_f;
        }
    }
}


void jpsGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::MidButton)
    {
        midbutton_hold=false;
    }

}


QPointF jpsGraphicsView::return_Pos()
{
    return translated_pos;
}

void jpsGraphicsView::delete_all()
{
    datamanager->remove_all();

    // Delete all lines

    for (int i=0; i<line_vector.size(); i++)
    {

        delete line_vector[i]->get_line();
        delete line_vector[i];

    }

    //line_vector.erase(line_vector.begin(),line_vector.end());
    //intersect_point_vector.erase(intersect_point_vector.begin(),intersect_point_vector.end());
    line_vector.clear();

    intersect_point_vector.clear();

    line_tracked=-1;

    update();
}

void jpsGraphicsView::use_gridmode()

{
    // if a current line is in the drawing procedure (i. e. starting point is determined but not the ending point)
    // the angles 0,90,180,270 will be catched if the cursor leads the current line to an area nearby.

    QLineF line(current_line->line().x1()+translation_x,current_line->line().y1()+translation_y,pos.x(),pos.y());

    if (line.angle()<=185 && line.angle()>=175){

        pos.setY(current_line->line().y1()+translation_y);
        //current_line->line().setAngle(180.0);
        //std::cout << current_line->line().angle() << std::endl;
        //current_line->setLine(current_line->line());
    }

    else if (line.angle()<=5 || line.angle()>=355){

        pos.setY(current_line->line().y1()+translation_y);
    }
    else if (line.angle()<=95 && line.angle()>=85){

        pos.setX(current_line->line().x1()+translation_x);
    }
    else if (line.angle()<=275 && line.angle()>=265){

        pos.setX(current_line->line().x1()+translation_x);
    }
}

void jpsGraphicsView::catch_points()
{
    //std::cout << "sagwas" << std::endl;

    //Searching for startpoints of all lines near the current cursor position
    for (signed int i=0; i<line_vector.size(); i++){

        // range chosen: 10 (-5:5) (has to be changed soon)
        if (line_vector[i]->get_line()->line().x1()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x1()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y1()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y1()<=(translated_pos.y()+catch_radius)){
            // in this case the cursor is working with global coordinates. So the method 'mapToGlobal' must be used
            translated_pos.setX(line_vector[i]->get_line()->line().x1());
            translated_pos.setY(line_vector[i]->get_line()->line().y1());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x1()),translate_back_y(line_vector[i].y1()))));
            //bool is used to tell paint device to draw a red rect if a point was tracked
            point_tracked=true;
            //QPen pen;
            //pen.setColor('red');
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red));
            // if a point was tracked there is no need to look for further points ( only one point can be tracked)
            break;
        }

        //Searching for endpoints of all lines near the current cursor position
        else if (line_vector[i]->get_line()->line().x2()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x2()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y2()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y2()<=(translated_pos.y()+catch_radius)){
            // see above
            translated_pos.setX(line_vector[i]->get_line()->line().x2());
            translated_pos.setY(line_vector[i]->get_line()->line().y2());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x2()),translate_back_y(line_vector[i].y2()))));
            point_tracked=true;
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red));
            break;
        }
        // if no point was tracked bool is set back to false
        point_tracked=false;
    }
    // if no start- or endpoint was tracked it is searched for intersections- Points

    if (point_tracked==false)
    {
        // see above
        for (int j=0; j<intersect_point_vector.size(); j++)
        {

            if (intersect_point_vector[j]->x()>=(translated_pos.x()-catch_radius) && intersect_point_vector[j]->x()<=(translated_pos.x()+catch_radius) && intersect_point_vector[j]->y()>=(translated_pos.y()-catch_radius) && intersect_point_vector[j]->y()<=(translated_pos.y()+catch_radius))
            {
                translated_pos.setX(intersect_point_vector[j]->x());
                translated_pos.setY(intersect_point_vector[j]->y());
                current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red));
                point_tracked=true;
                break;
            }
        }
    }
}

void jpsGraphicsView::catch_lines()
{
    //catch lines (only possible if wall is disabled)

    //std::vector<JPGeoLine> candidates;
    qreal min=catch_line_distance;
    int index=-1;
    int current_d=0;
    for (signed int i=0; i<line_vector.size(); i++)
    {
        if ((line_vector[i]->get_line()->line().x1()>=(translated_pos.x()-catch_line_distance) && line_vector[i]->get_line()->line().x2()<=(translated_pos.x()+catch_line_distance))
                || (line_vector[i]->get_line()->line().x1()<=(translated_pos.x()+catch_line_distance) && line_vector[i]->get_line()->line().x2()>=(translated_pos.x()-catch_line_distance)))
        {
            if ((line_vector[i]->get_line()->line().y1()>=(translated_pos.y()-catch_line_distance) && line_vector[i]->get_line()->line().y2()<=(translated_pos.y()+catch_line_distance))
                    || (line_vector[i]->get_line()->line().y1()<=(translated_pos.y()+catch_line_distance) && line_vector[i]->get_line()->line().y2()>=(translated_pos.y()-catch_line_distance)))
            {
                //candidates.push_back(line_vector[i]);
                    current_d=this->calc_d_point(line_vector[i]->get_line()->line(),translated_pos.x(),translated_pos.y());
                if (current_d<min)
                {
                    min=current_d;
                    index=i;
                }
            }

        }
    }
    if (index>=0)
    {
        line_tracked=index;
        select_line(line_vector[line_tracked]);

    }
    else
    {
        line_tracked=-1;

    }

}

void jpsGraphicsView::select_line(jpsLineItem *mline)
{
    mline->get_line()->setPen(QPen(Qt::red,3*gl_scale_f));
    marked_lines.push_back(mline);
}

qreal jpsGraphicsView::get_gl_min_x()
{
    return gl_min_x;
}

qreal jpsGraphicsView::get_gl_min_y()
{
    return gl_min_y;
}


qreal jpsGraphicsView::calc_d_point(const QLineF &line,const qreal &x, const qreal &y)

{
    // using hessian normal term
    qreal m = (line.y2()-line.y1())/(line.x2()-line.x1());
    qreal n = line.y2()-m*line.x2();
    // if m!=0 (to avoid zero division)
    if (m>=0.001 || m<=-0.001 )
    {
        return fabs((y-m*x-n)/m);
    }
    else
    // if m==0 the radius is calculated by the distance between the y-values
    {
        return fabs(y-line.y2());
    }

}


// Delete single line

void jpsGraphicsView::delete_marked_lines()

{
    if (line_tracked!=-1)
    {
        for (int i=0; i<marked_lines.size(); i++)
        {
            QList<QPointF *> points = marked_lines[i]->get_intersectionVector();

            for (int j=0; j<points.size(); j++)
            {
                intersect_point_vector.removeOne(points[i]);
            }

            line_vector.removeOne(marked_lines[i]);

            if (marked_lines[i]->is_Wall()==true)
            {
                QList<jpsRoom* > cList= datamanager->get_roomlist();
                for (int j=0; j<cList.size(); j++)
                {   QList<jpsLineItem* > delete_candidates;
                    for (int k=0; k<cList[j]->get_listWalls().size(); k++)
                    {
                        if (marked_lines[i]==cList[j]->get_listWalls()[k])
                        {
                            delete_candidates.push_back(cList[j]->get_listWalls()[k]);

                        }
                    }
                    cList[j]->removeWall(delete_candidates);
                }
            }

            else if (marked_lines[i]->is_Door()==true)
            {
                QList<jpsCrossing* > cList= datamanager->get_crossingList();
                for (int j=0; j<cList.size(); j++)
                {
                    if (marked_lines[i]==cList[j]->get_cLine())
                    {
                        datamanager->remove_crossing(cList[i]);
                        break;
                    }
                }

            }
            else
            {
                QList<jpsExit* > cList= datamanager->get_exitList();
                for (int j=0; j<cList.size(); j++)
                {
                    if (marked_lines[i]==cList[j]->get_cLine())
                    {
                        datamanager->remove_exit(cList[i]);
                        break;
                    }
                }
            }

            delete marked_lines[i]->get_line();
            delete marked_lines[i];
            marked_lines.removeAt(i);
            i--;
        }
        marked_lines.clear();
        line_tracked=-1;
        emit lines_deleted();
        update();
    }

}

void jpsGraphicsView::take_l_from_lineEdit(const qreal &length)
{
    if (current_line!=0L)
    {
        QLineF line(current_line->line());
        line.setLength(length);
        current_line->setLine(line);
        jpsLineItem* jpsline = new jpsLineItem(current_line);
        jpsline->set_type(statWall,statDoor,statExit);
        line_vector.push_back(jpsline);
        current_line=0L;
    }
}

QString jpsGraphicsView::build_coordString()
{
    QString coordString;
    for (int i=0; i<this->line_vector.size(); i++)
    {
        coordString.append("ID:");
        coordString.append(QString::number(i));
        coordString.append(" Wall:");
        coordString.append(" x1:");
        coordString.append(QString::number(this->line_vector[i]->get_line()->line().x1()));
        coordString.append(" x2:");
        coordString.append(QString::number(this->line_vector[i]->get_line()->line().x2()));
        coordString.append(" y1:");
        coordString.append(QString::number(this->line_vector[i]->get_line()->line().y1()));
        coordString.append(" y2:");
        coordString.append(QString::number(this->line_vector[i]->get_line()->line().y2()));
        coordString.append("\n");
    }
    return coordString;

}

QList<jpsLineItem *> jpsGraphicsView::get_markedLines()
{
    return marked_lines;
}

QList<jpsLineItem *> jpsGraphicsView::get_line_vector()
{
    return line_vector;
}

qreal jpsGraphicsView::get_scale_f()
{
    return gl_scale_f;
}


void jpsGraphicsView::change_objectsnap()
{
    objectsnap=!objectsnap;
}

bool jpsGraphicsView::get_objectsnap()
{
    return objectsnap;
}

void jpsGraphicsView::en_disableWall()
{
    statWall=!statWall;
    statDoor=false;
    statExit=false;
    currentPen.setColor(Qt::black);
}

bool jpsGraphicsView::statusWall()
{
    return statWall;
}

void jpsGraphicsView::change_gridmode()
{
    gridmode=!gridmode;
}

bool jpsGraphicsView::get_gridmode()
{
    return gridmode;
}

void jpsGraphicsView::en_disableDoor()
{
    statDoor=!statDoor;
    statExit=false;
    statWall=false;
    currentPen.setColor(Qt::blue);
}

bool jpsGraphicsView::statusDoor()
{
    return statDoor;
}

void jpsGraphicsView::en_disableExit()
{
    statExit=!statExit;
    statDoor=false;
    statWall=false;
    currentPen.setColor(Qt::darkMagenta);
}

bool jpsGraphicsView::statusExit()
{
    return statExit;
}











