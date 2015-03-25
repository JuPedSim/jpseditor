
#include <QtGui>
#include "GraphicView.h"
#include <iostream>
#include <cmath>


jpsGraphicsView::jpsGraphicsView(QWidget* parent):QGraphicsView(parent)
{

    current_line=0L;
    current_caption=0L;
    //current_line_mark=0L;
    midbutton_hold=false;
    translation_x=0;
    translation_y=this->height();
    gridmode=false;
    gl_scale_f=1.0;
    //scale(10,10);
    catch_radius=10*gl_scale_f;
    catch_line_distance=10*gl_scale_f;
    //Set-up the view
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    point_tracked=false;
    line_tracked=-1;
    current_rect=0L;
    currentSelectRect=nullptr;
    objectsnap=false;
    statWall=false;
    statDoor=false;
    statExit=false;
    currentPen.setColor(Qt::black);
    currentPen.setCosmetic(true);
    this->scale(1,-1);

    //gl_min_x=1e6;
    //gl_min_y=1e6;

    //m_graphView->setFixedSize(1600, 900);
    //m_graphView->setScene(m_graphScen);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);


    //setRenderHint(QPainter::NonCosmeticDefaultPen);
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

jpsGraphicsView::~jpsGraphicsView()
{
    delete Scene;
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
            //current_line->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
            current_line->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);

        }

        for (int i=0; i<line_vector.size(); i++)
        {
            //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
            line_vector[i]->get_line()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);

        }
        for (int i=0; i<caption_list.size(); i++)
        {
            //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
            qreal scalef = caption_list[i]->data(0).toReal();
            caption_list[i]->setTransform(QTransform::fromScale(1.0/scalef,1.0/scalef),true); // without this line translations won't work
            caption_list[i]->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);
            caption_list[i]->setTransform(QTransform::fromScale(scalef,scalef),true);

        }
        //if (current_line_mark!=0L)
        //{
        //    current_line_mark->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        //}
        //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        //translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
       // setDragMode(ScrollHandDrag);

    }
    //
    else if(leftbutton_hold==true)
    {

        currentSelectRect->setRect(QRectF(QPointF(currentSelectRect->rect().x(),currentSelectRect->rect().y())
                                         ,QPointF(translated_pos.x(),translated_pos.y())));

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
                jpsLineItem* lineItem= new jpsLineItem(current_line);

                // if there is already a drawn line
                if (line_vector.size()>=1)
                {
                    // Searching for intersection of the current line with all already drawn lines
                    for (int i=0; i<line_vector.size(); i++)
                    {
                        // locate possible intersection between lines
                        // if there are some, intersectionPoint will be added to container
                        locate_intersection(lineItem,line_vector[i]);
                    }
                }

                lineItem->set_type(statWall,statDoor,statExit);
                line_vector.push_back(lineItem);

                // pointer "current_line" is reset
                current_line=0L;
            }
        }
        else //if statWall==false
        {
            currentSelectRect=Scene->addRect(translated_pos.x(),translated_pos.y(),0,0,QPen(Qt::blue,0));
            currentSelectRect->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            leftbutton_hold=true;
            //catch_lines();

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

        marked_lines[i]->get_line()->setPen(QPen(QColor(marked_lines[i]->get_defaultColor()),0));

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

            // for (int i=0; i<caption_list.size(); i++)
            // {
            //    caption_list[i]->setTransform(QTransform::fromScale(1/gl_scale_f,1/gl_scale_f));
            //}
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

    if (event->button() == Qt::LeftButton)
    {
        // Select lines that are located within the rectangle
        if (!(statWall==true || statDoor==true || statExit==true))
        {
            leftbutton_hold=false;
            catch_lines();
            delete currentSelectRect;
            currentSelectRect=nullptr;

            //unmark selected line(s)
            if (line_tracked==-1)
            {
                unmark_all_lines();
            }
        }
    }

    if (event->button() == Qt::MidButton)
        midbutton_hold=false;

}



QPointF jpsGraphicsView::return_Pos()
{
    return translated_pos;
}

void jpsGraphicsView::delete_all()
{
    emit remove_all();

    // Delete all lines

    for (int i=0; i<line_vector.size(); i++)
    {

        delete line_vector[i]->get_line();
        delete line_vector[i];

    }

    //line_vector.erase(line_vector.begin(),line_vector.end());
    //intersect_point_vector.erase(intersect_point_vector.begin(),intersect_point_vector.end());
    line_vector.clear();

    for (int i=0; i<intersect_point_vector.size(); i++)
    {

        delete intersect_point_vector[i];

    }


    intersect_point_vector.clear();

    line_tracked=-1;
    emit lines_deleted();
    update();
}

void jpsGraphicsView::use_gridmode()

{
    // if a current line is in the drawing procedure (i. e. starting point is determined but not the ending point)
    // the angles 0,90,180,270 will be catched if the cursor leads the current line to an area nearby.

    QLineF line(current_line->line().x1()+translation_x,current_line->line().y1()+translation_y,pos.x(),pos.y());

    if (line.angle()<=185 && line.angle()>=175){

        pos.setY(current_line->line().y1()+translation_y);
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
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
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
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
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

    line_tracked=-1;

    for (auto &item:line_vector)
    {
        if (currentSelectRect->contains(item->get_line()->line().p1())
                && currentSelectRect->contains(item->get_line()->line().p2()))
        {
            select_line(item);
            line_tracked=1;

        }
    }
}

void jpsGraphicsView::select_line(jpsLineItem *mline)
{

    if (!marked_lines.contains(mline))
    {
        mline->get_line()->setPen(QPen(Qt::red,0));
        marked_lines.push_back(mline);
    }
    else
    {
        mline->get_line()->setPen(QPen(Qt::black,0));
        marked_lines.removeOne(mline);
    }
}


void jpsGraphicsView::disable_drawing()
{
    statWall=false;
    statDoor=false;
    statExit=false;
    // if drawing was canceled by pushing ESC
    if (current_line!=0L)
    {
        //not completed line will be deleted
        delete current_line;
        current_line=0L;
    }

}

jpsLineItem* jpsGraphicsView::addLineItem(const qreal &x1,const qreal &y1,const qreal &x2,const qreal &y2,const QString &type)
{

    current_line=Scene->addLine(x1,y1,x2,y2);
    current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    jpsLineItem* newLine = new jpsLineItem(current_line);

    if (type=="Door")
    {
        newLine->set_Door();
    }
    else if (type=="Exit")
    {
        newLine->set_Exit();
    }
    else
    {
        newLine->set_Wall();
    }

    current_line->setPen(QPen(QColor(newLine->get_defaultColor()),0));
    // if line has already been added before (from another room)

    for (int i=0; i<line_vector.size(); i++)
    {
        if (newLine->get_line()->line()==line_vector[i]->get_line()->line())
        {
            delete current_line;
            current_line=0L;
            return line_vector[i];
        }
    }
    line_vector.push_back(newLine);

    for (int i=0; i<line_vector.size(); i++)
    {
        locate_intersection(newLine,line_vector[i]);
    }

    current_line=0L;

    return newLine;

}

void jpsGraphicsView::locate_intersection(jpsLineItem *item1, jpsLineItem *item2)
{

    //this pointer is necessary due to the architecture of the method 'intersect'
    QPointF* intersection_point = new QPointF;
    // if 'intersect'==1 -> an intersection point exists
    if (item1->get_line()->line().intersect(item2->get_line()->line(),intersection_point)==1)
    {
        // attaching current intersection point
        intersect_point_vector.push_back(intersection_point);
        // LineItem remember intersectionPoint
        item1->add_intersectionPoint(intersection_point);
        item2->add_intersectionPoint(intersection_point);
        //LineItem remembers line which it has an intersection with
        item1->add_intersectLine(item2);
        item2->add_intersectLine(item1);

        // resetting pointer
        intersection_point=0L;
    }
    else
    {
        // Free memory
        delete intersection_point;
        intersection_point=0L;
    }

}

bool jpsGraphicsView::show_hide_roomCaption(QString name, qreal x, qreal y)
{
    // if caption exits, it is supposed to be hided:
    for (int i=0; i<caption_list.size(); i++)
    {
        if (caption_list[i]->toPlainText()==name)
        {
            delete caption_list[i];
            caption_list.removeOne(caption_list[i]);
            return false;
        }
    }
    // if caption does not exit yet:


    current_caption=Scene->addText(name);

    current_caption->setX(x+translation_x);
    current_caption->setY(y+translation_y);

    //Since the scene itself is mirrored (scale=1, scale=-1):

    // adjust captionsize depending on zoomgrade
    current_caption->setData(0,gl_scale_f);
    current_caption->setTransform(QTransform::fromScale(gl_scale_f,-gl_scale_f),true);

    //current_caption->adjustSize();
    caption_list.push_back(current_caption);
    current_caption=0L;

    return true;
}
/*
void jpsGraphicsView::create_grid()
{
    //QPointF sceneViewBottomLeft = mapToScene(QPoint(this->sceneRect().bottomLeft()));
    //QPointF sceneViewTopRight = mapToScene(QPoint(this->sceneRect().topRight()));
    //QRectF rect = this->sceneRect();

    //std::cout << rect.bottomRight().y() << std::endl;

    QPointF leftdown = mapToScene(this->sceneRect().bottomLeft().x(),this->sceneRect().bottomLeft().y());
    qreal leftdownx = leftdown.x();
    qreal leftdowny = leftdown.y();

    QPointF topRight = mapToScene(this->sceneRect().bottomRight().x(),this->sceneRect().bottomRight().y());
    qreal rightupx = topRight.x();
    qreal rightupy = -topRight.y();

    std::cout << rightupx << std::endl;
    std::cout << rightupy << std::endl;
    int ileftdownx = std::floor(leftdownx);
    int ileftdowny = std::floor(leftdowny);
    int irightupx = std::floor(rightupx);
    int irightupy = std::floor(rightupy);
    //std::cout << ileftupx << std::endl;
    //std::cout << rightdownx << std::endl;
    //QGraphicsTextItem* point = Scene->addText("Hallo Welt");
    //point->setX(ileftupx);
    //point->setY(ileftupy);
    //std::cout << ileftdownx << std::endl;
    //std::cout << irightupy << std::endl;
    for (int i=0; i<irightupx; i+=100)
    {
        for (int j=0; j<irightupy; j+=100)
        {
            //std::cout << rightdownx << std::endl;
            QGraphicsEllipseItem* point = Scene->addEllipse(i,j,1,1,QPen(Qt::red));

            //grid_point_vector.push_back(point);
        }
    }

    //std::cout << leftup-translation_x << std::endl;
}

*/


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
        emit remove_marked_lines();

        for (int i=0; i<marked_lines.size(); i++)
        {
            QList<QPointF *> points = marked_lines[i]->get_intersectionVector();

            for (int j=0; j<points.size(); j++)
            {

                intersect_point_vector.removeOne(points[j]);

                for (int k=0; k<marked_lines[i]->get_intersectLineVector().size(); k++)
                {
                    // removing the intersectionPoint pointer from all lines which includes the point
                    marked_lines[i]->get_intersectLineVector()[k]->remove_intersectionPoint(points[j]);

                    // as marked_lines is removed it is has no intersections with any other line anymore
                    // so pointers of possible intersectionsLine have to be removed

                }

                delete points[j];
            }

            for (int k=0; k<marked_lines[i]->get_intersectLineVector().size(); k++)
            {
                marked_lines[i]->get_intersectLineVector()[k]->remove_interLine(marked_lines[i]);
            }

            line_vector.removeOne(marked_lines[i]);

            delete marked_lines[i]->get_line();
            marked_lines[i]->set_line(0L);
            delete marked_lines[i];
            marked_lines.removeAt(i);
            i--;
        }
        //marked_lines.clear();
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
    if (statWall==false)
    {
        emit no_drawing();
    }
    else
    {
        currentPen.setColor(Qt::black);
    }

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
    if (statDoor==false)
    {
        emit no_drawing();
    }
    else
    {
        currentPen.setColor(Qt::blue);
    }

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
    if (statExit==false)
    {
        emit no_drawing();
    }
    else
    {
        currentPen.setColor(Qt::darkMagenta);
    }
}

bool jpsGraphicsView::statusExit()
{
    return statExit;
}











