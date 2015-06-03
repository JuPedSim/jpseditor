
#include <QtGui>
#include "GraphicView.h"
#include <iostream>
#include <cmath>
#include <memory>
#include <QMessageBox>



jpsGraphicsView::jpsGraphicsView(QWidget* parent):QGraphicsView(parent)
{

    current_line=nullptr;
    _currentVLine=nullptr;
    current_caption=nullptr;
    //current_line_mark=nullptr;
    midbutton_hold=false;
    translation_x=0;
    translation_y=0;//this->height();
    gridmode=false;
    gl_scale_f=.01;
    //scale(10,10);
    catch_radius=10*gl_scale_f;
    catch_line_distance=10*gl_scale_f;
    //Set-up the view
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    point_tracked=false;
    line_tracked=-1;
    current_rect=nullptr;
    currentSelectRect=nullptr;
    objectsnap=false;
    statWall=false;
    statDoor=false;
    statExit=false;
    statLandmark=false;
    markedLandmark=nullptr;
    currentLandmarkRect=nullptr;
    currentPen.setColor(Qt::black);
    currentPen.setCosmetic(true);
    this->scale(1/gl_scale_f,-1/gl_scale_f);
    _currentTrackedPoint=nullptr;
    _statLineEdit=false;

    lines_collided=false;
    _assoDef=false;
    //gl_min_x=1e6;
    //gl_min_y=1e6;

    //m_graphView->setFixedSize(1600, 900);
    //m_graphView->setScene(m_graphScen);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);


    //setRenderHint(QPainter::NonCosmeticDefaultPen);
    // setRenderHint(QPainter::Antialiasing);

    //m_graphView->setAlignment(nullptr);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCursor(Qt::CrossCursor);


    //Set-up the scene
    Scene = new QGraphicsScene(this);
    setScene(Scene);
    setSceneRect(0, 0, 1920, 1080);

    //create_grid();
}

jpsGraphicsView::~jpsGraphicsView()
{
    delete_all(true);
    delete Scene;
}


void jpsGraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{

    QGraphicsView::mouseMoveEvent(mouseEvent);

    if (current_rect!=nullptr)
    {
       delete current_rect;
       current_rect=nullptr;

    }
    QPointF old_pos=pos;

    pos=mapToScene(mouseEvent->pos());

    if (gridmode==true)
    {
        if (current_line!=nullptr)
        {
            use_gridmode(current_line,5);
        }
    }

    if (_currentVLine!=nullptr)
    {
        /// vline will be deleted if it is not thrown horizontally or vertically
        if(!use_gridmode(_currentVLine,15))
        {
            delete _currentVLine;
            _currentVLine=nullptr;
        }

    }


    if (midbutton_hold==true)
    {
        translations(old_pos);
    }

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

        ///VLine
        if (point_tracked && (statWall==true || statDoor==true || statExit==true))
        {
            SetVLine();
        }
    }

    if (current_line!=nullptr)
    {
        //if (statWall==true || statDoor==true || statExit==true)
        //{
        //emit set_focus_textedit();
        current_line->setLine(current_line->line().x1(),current_line->line().y1(),translated_pos.x(),translated_pos.y());

        //}
    }
    if (_currentVLine!=nullptr)
    {
        emit set_focus_textedit();
        _currentVLine->setLine(_currentVLine->line().x1(),_currentVLine->line().y1(),translated_pos.x(),translated_pos.y());
    }

    /// see if two lines collided FIX ME!!!
    //line_collision();

    emit mouse_moved();
    update();

}


void jpsGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
{

    if (mouseEvent->button() == Qt::LeftButton)
    {

        if (statWall==true || statDoor==true || statExit==true)
        {
            drawLine();
        }
        else if (statLandmark==true)
        {
            addLandmark();
        }
        else
        {
            ///LineEdit
            if (_currentTrackedPoint!=nullptr && line_tracked==1)
            {
                EditLine(_currentTrackedPoint);
                _currentTrackedPoint=nullptr;
                line_tracked=-1;
            }
            else
            {
                ///Select_mode
                currentSelectRect=Scene->addRect(translated_pos.x(),translated_pos.y(),0,0,QPen(Qt::blue,0));
                currentSelectRect->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
                leftbutton_hold=true;
            }

        }

    }
    else if (mouseEvent->button()==Qt::MidButton)
    {
        midbutton_hold=true;
    }

    update();
}

void jpsGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
        emit no_drawing();

}



void jpsGraphicsView::unmark_all_lines()
{

    for (int i=0; i<marked_lines.size();i++)
    {
        marked_lines[i]->get_line()->setPen(QPen(QColor(marked_lines[i]->get_defaultColor()),0));
    }
    marked_lines.clear();
}

void jpsGraphicsView::addLandmark()
{
    QPixmap pixmap("../jupedsim/forms/statue.jpg");

    QGraphicsPixmapItem* pixmapItem = Scene->addPixmap(pixmap);
    pixmapItem->setScale(0.002);
    pixmapItem->setTransform(QTransform::fromTranslate(this->translated_pos.x()-pixmap.width()/1000.,this->translated_pos.y()
                          +pixmap.height()/1000.));
    pixmapItem->setTransform(QTransform::fromScale(1,-1),true);
    pixmapItem->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    QString name="Landmark"+QString::number(LLandmarks.size());
    jpsLandmark* landmark = new jpsLandmark(pixmapItem,name,pixmapItem->scenePos());

    LLandmarks.push_back(landmark);

    emit landmark_added();

}

void jpsGraphicsView::unmarkLandmark()
{

    if (markedLandmark!=nullptr)
    {
        delete currentLandmarkRect;
        currentLandmarkRect=nullptr;
        markedLandmark=nullptr;
    }

}

QList<jpsLandmark* > jpsGraphicsView::get_landmarks()
{
    return LLandmarks;
}

QGraphicsRectItem *jpsGraphicsView::GetCurrentSelectRect()
{
    return currentSelectRect;
}

void jpsGraphicsView::ShowWaypoints(QList<jpsWaypoint *> waypoints)
{
    ClearWaypoints();

    for (jpsWaypoint* waypoint:waypoints)
    {
        QGraphicsEllipseItem* ellipse = Scene->addEllipse(waypoint->GetRect(),QPen(Qt::blue,0));
        ellipse->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        _waypoints.push_back(ellipse);
    }


}



void jpsGraphicsView::ClearWaypoints()
{

    ///Waypoints
    for (QGraphicsEllipseItem* old_waypoint:_waypoints)
    {
        delete old_waypoint;
    }
    _waypoints.clear();

}


void jpsGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (midbutton_hold==false)
    {
        zoom(event->delta());
    }
}


void jpsGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        /// Select lines that are located within the rectangle
        if (!(statWall==true || statDoor==true || statExit==true || statLandmark==true))
        {
            leftbutton_hold=false;

            if (currentSelectRect!=nullptr)
            {
                if (_assoDef)
                {
                    ///Waypoint definition
                    emit AssoDefCompleted();
                }
                else
                {
                    /// Select lines by creating a rect with the cursor
                    catch_lines();

                    /// unmark Landmark is possible
                    unmarkLandmark();
                    /// Look for landmarks with position in the currentSelectRect
                    catch_landmark();
                }
                delete currentSelectRect;
                currentSelectRect=nullptr;

            }

            ///unmark selected line(s)
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

void jpsGraphicsView::delete_all(bool final)
{
    if (!final)
    {
        int ret = QMessageBox::warning(this,"Delete?", "Do you really want to delete all elements?", QMessageBox::Yes | QMessageBox::No);

        if (ret == QMessageBox::No)
        {
            return;
        }
    }

    emit remove_all();

    /// Delete all lines

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

    for (jpsLandmark* landmark:LLandmarks)
    {
        delete landmark->get_pixmap();
        delete landmark;
    }
    for (QGraphicsEllipseItem* waypoint:_waypoints)
    {
        delete waypoint;
    }

    intersect_point_vector.clear();
    marked_lines.clear();
    LLandmarks.clear();


    if (current_line!=nullptr)
    {
        delete current_line;
        current_line=nullptr;
    }

    if (_currentVLine!=nullptr)
    {
        delete _currentVLine;
        _currentVLine=nullptr;
    }

    line_tracked=-1;
    emit lines_deleted();
    update();
}

bool jpsGraphicsView::use_gridmode(QGraphicsLineItem* currentline, int accuracy)

{
    // if a current line is in the drawing procedure (i. e. starting point is determined but not the ending point)
    // the angles 0,90,180,270 will be catched if the cursor leads the current line to an area nearby.

    QLineF line(currentline->line().x1()+translation_x,currentline->line().y1()+translation_y,pos.x(),pos.y());

    if (line.angle()<=(180+accuracy) && line.angle()>=(180-accuracy)){

        pos.setY(currentline->line().y1()+translation_y);
        return true;
    }

    else if (line.angle()<=(0+accuracy) || line.angle()>=(360-accuracy)){

        pos.setY(currentline->line().y1()+translation_y);
        return true;
    }
    else if (line.angle()<=(90+accuracy) && line.angle()>=(90-accuracy)){

        pos.setX(currentline->line().x1()+translation_x);
        return true;
    }
    else if (line.angle()<=(270+accuracy) && line.angle()>=(270-accuracy)){

        pos.setX(currentline->line().x1()+translation_x);
        return true;
    }
    return false;
}

void jpsGraphicsView::catch_points()
{
    ///Searching for startpoints of all lines near the current cursor position
    for (signed int i=0; i<line_vector.size(); i++){

        // range chosen: 10 (-5:5) (has to be changed soon)
        if (line_vector[i]->get_line()->line().x1()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x1()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y1()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y1()<=(translated_pos.y()+catch_radius)){
            /// in this case the cursor is working with global coordinates. So the method 'mapToGlobal' must be used
            translated_pos.setX(line_vector[i]->get_line()->line().x1());
            translated_pos.setY(line_vector[i]->get_line()->line().y1());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x1()),translate_back_y(line_vector[i].y1()))));
            ///bool is used to tell paint device to draw a red rect if a point was tracked
            point_tracked=true;
            _currentTrackedPoint= &translated_pos;
            //QPen pen;
            //pen.setColor('red');
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
            /// if a point was tracked there is no need to look for further points ( only one point can be tracked)

            return;
        }

        ///Searching for endpoints of all lines near the current cursor position
        else if (line_vector[i]->get_line()->line().x2()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x2()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y2()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y2()<=(translated_pos.y()+catch_radius)){
            // see above
            translated_pos.setX(line_vector[i]->get_line()->line().x2());
            translated_pos.setY(line_vector[i]->get_line()->line().y2());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x2()),translate_back_y(line_vector[i].y2()))));
            point_tracked=true;
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));            
            _currentTrackedPoint= &translated_pos;
            return;
        }
    }
    /// if no start- or endpoint was tracked it is searched for intersections- Points

    /// see above
    for (int j=0; j<intersect_point_vector.size(); j++)
    {
        if (intersect_point_vector[j]->x()>=(translated_pos.x()-catch_radius) && intersect_point_vector[j]->x()<=(translated_pos.x()+catch_radius) && intersect_point_vector[j]->y()>=(translated_pos.y()-catch_radius) && intersect_point_vector[j]->y()<=(translated_pos.y()+catch_radius))
        {
            translated_pos.setX(intersect_point_vector[j]->x());
            translated_pos.setY(intersect_point_vector[j]->y());
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
            point_tracked=true;
            return;
        }
    }
    //Look for gridpoints

//    for (auto &gridpoint: grid_point_vector)
//    {
//        //qreal x = (gridpoint->line().x1()+gridpoint->line().x2())/2.0;
//        //qreal y = (gridpoint->line().y1()+gridpoint->line().y2())/2.0;
//        qreal x = gridpoint.x();
//        qreal y = gridpoint.y();
//        if (x>=(translated_pos.x()-catch_radius)
//                && x<=(translated_pos.x()+catch_radius)
//                && y>=(translated_pos.y()-catch_radius)
//                && y<=(translated_pos.y()+catch_radius))
//        {

//            translated_pos.setX(x);
//            translated_pos.setY(y);
//            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
//                                        translated_pos.y()+translation_y-10*gl_scale_f,
//                                        20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
//            point_tracked=true;
//            return;
//        }
//    }

    /// if no point was tracked bool is set back to false
    point_tracked=false;
    return;
}

void jpsGraphicsView::catch_lines()
{
    ///catch lines (only possible if wall is disabled)
    /// if current rect was build up moving the cursor to the left ->
    /// whole line has to be within the rect to select the line
    line_tracked=-1;
    if (currentSelectRect->rect().width()<0)
    {
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
    /// if current rect was build up moving the cursor to the right ->
    /// throwing the select rect only over a part of a line is sufficent to select it
    else if (currentSelectRect->rect().width()>0)
    {
        for (auto &item:line_vector)
        {
            if (currentSelectRect->collidesWithItem(item->get_line()))
            {
                select_line(item);
                line_tracked=1;

            }
        }
    }
}

void jpsGraphicsView::drawLine()
{
    if (current_line==nullptr) /// if the mouse was pressed first of two times
    {
        ///Determining first point of line

        /// all two points of the line are inited with the cursorcoordinates
        current_line = Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        //current_line->translate(translation_x,translation_y);
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        emit set_focus_textedit();

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

        ///reset pointer
        current_line=nullptr;

        if (_statLineEdit)
        {

            select_line(lineItem);
            _statLineEdit=false;
            line_tracked=1;
            emit no_drawing();
        }
        else
            drawLine();


    }

    ///Vline
    if (_currentVLine!=nullptr)
    {
        delete _currentVLine;
        _currentVLine=nullptr;
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
    statLandmark=false;
    _statLineEdit=false;
    /// if drawing was canceled by pushing ESC
    if (current_line!=nullptr)
    {
        ///not completed line will be deleted
        delete current_line;
        current_line=nullptr;
    }
    if (_currentVLine!=nullptr)
    {
        ///VLine will be deleted
        delete _currentVLine;
        _currentVLine=nullptr;
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
            current_line=nullptr;
            return line_vector[i];
        }
    }
    line_vector.push_back(newLine);

    for (int i=0; i<line_vector.size(); i++)
    {
        locate_intersection(newLine,line_vector[i]);
    }

    current_line=nullptr;

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
        intersection_point=nullptr;
    }
    else
    {
        // Free memory
        delete intersection_point;
        intersection_point=nullptr;
    }

}

void jpsGraphicsView::SetVLine()
{
    if (_currentVLine==nullptr)
    {
        if (current_line==nullptr)
        {
            _currentVLine=Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),
                                         QPen(Qt::black,0,Qt::DashLine));
            _currentVLine->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        }
        else if (current_line->line().p1()!=translated_pos)
        {
            _currentVLine=Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),
                                         QPen(Qt::black,0,Qt::DashLine));
            _currentVLine->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        }
    }
}

void jpsGraphicsView::EditLine(QPointF* point)
{
    if (marked_lines.size()==1)
    {

        if (marked_lines.first()->get_line()->line().p1()==*point)
        {
            current_line=Scene->addLine(marked_lines[0]->get_line()->line().p2().x(),
                    marked_lines[0]->get_line()->line().p2().y(),translated_pos.x(),translated_pos.y(),QPen(Qt::red,0));
            current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            emit set_focus_textedit();
        }

        else if (marked_lines.first()->get_line()->line().p2()==*point)
        {
            current_line=Scene->addLine(marked_lines[0]->get_line()->line().p1().x(),
                    marked_lines[0]->get_line()->line().p1().y(),translated_pos.x(),translated_pos.y(),QPen(Qt::red,0));
            current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            emit set_focus_textedit();
        }

        else
            return;

        //line_tracked=1;
        _statLineEdit=true;
        delete_marked_lines();
        en_disableWall();

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
    current_caption=nullptr;

    return true;
}

void jpsGraphicsView::line_collision() ///FIX ME!!!
{
    /// if no lines collided yet
    if (!lines_collided && current_line!=nullptr)
    {
        for (auto& line:line_vector)
        {
            /// if two lines collided
            QPointF* intersectPoint = new QPointF();
            if (current_line->line().intersect(line->get_line()->line(),intersectPoint)==1)
            {
                //lines_collided=true;
                /// if cursor is in the nearer area of the intersection point
                if (intersectPoint->x()>=(translated_pos.x()-catch_radius)
                        && intersectPoint->x()<=(translated_pos.x()+catch_radius)
                        && intersectPoint->y()>=(translated_pos.y()-catch_radius)
                        && intersectPoint->y()<=(translated_pos.y()+catch_radius))
                {
                    /// set end point of line to intersection point
                    current_line->setLine(current_line->line().x1(),current_line->line().y1(),
                                          intersectPoint->x(),intersectPoint->y());
                    current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                                        translated_pos.y()+translation_y-10*gl_scale_f,
                                                        20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
                }
                break;


            }

        }
    }
}

void jpsGraphicsView::create_grid() /// FIX ME!!!!
{
    //QPointF sceneViewBottomLeft = mapToScene(QPoint(this->sceneRect().bottomLeft()));
    //QPointF sceneViewTopRight = mapToScene(QPoint(this->sceneRect().topRight()));
    //QRectF rect = this->sceneRect();
    //for (QGraphicsLineItem *gridpoint: grid_point_vector)
    //{
     //   delete gridpoint;
    //}
    grid_point_vector.clear();

    //std::cout << rect.bottomRight().y() << std::endl;

    //QPointF leftdown = mapToScene(this->sceneRect().bottomLeft().x(),this->sceneRect().bottomLeft().y());

    //QPointF leftup = mapToScene(Scene->sceneRect().bottomLeft().x(),Scene->sceneRect().bottomLeft().y());
    //std::cout << leftup.x()-translation_x << std::endl;
    qreal rightx = translated_pos.x()+1920*gl_scale_f;
    qreal upy = translated_pos.y()+1080*gl_scale_f;
    qreal leftx = translated_pos.x()-1920*gl_scale_f;
    qreal downy = translated_pos.y()-1080*gl_scale_f;
    //QGraphicsEllipseItem* point = Scene->addEllipse(0,0,10,10,QPen(Qt::red));
    //point->setTransform(Scene->sceneRect().);
    //point->setTransform(QTransform::fromTranslate(translation_x,translation_y));
    //grid_point_vector.push_back(point);
    //std::cout << translation_y << std::endl;
    //std::cout << this->height() << std::endl;
    //qreal leftdownx = leftdown.x();//+translation_x;
    //qreal leftdowny = leftdown.y();//+translation_y;

    //QPointF topRight = mapToScene(this->sceneRect().topRight().x(),this->sceneRect().topRight().y());
    //qreal rightupx = topRight.x();//+translation_x;
    //qreal rightupy = topRight.y();//+translation_y;

    //std::cout << rightupx << std::endl;
    //std::cout << rightupy << std::endl;
    int intervall=100*gl_scale_f;
    int ileftx = leftx/intervall;
    ileftx*=intervall;
    int idowny = downy/intervall;
    idowny*=intervall;
    //int ileftdowny = std::floor(leftdowny);
    //int irightupx = std::floor(rightupx);
    //int irightupy = std::floor(rightupy);
    //std::cout << ileftupx << std::endl;
    //std::cout << rightdownx << std::endl;
    //QGraphicsTextItem* point = Scene->addText("Hallo Welt");
    //point->setX(ileftupx);
    //point->setY(ileftupy);
    //std::cout << ileftdownx << std::endl;
    //std::cout << irightupy << std::endl;

    for (int i=ileftx; i<rightx; i+=intervall)
    {
        for (int j=idowny; j<upy; j+=intervall)
        {
            //std::cout << rightdownx << std::endl;
            /*
            QGraphicsLineItem* line1 = Scene->addLine(i,j-5*gl_scale_f,i,j+5*gl_scale_f,QPen(Qt::red,0));
            line1->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            grid_point_vector.push_back(line1);
            QGraphicsLineItem* line2 = Scene->addLine(i-5*gl_scale_f,j,i+5*gl_scale_f,j,QPen(Qt::red,0));
            line2->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            grid_point_vector.push_back(line2);
            */
            QPointF gridpoint(i,j);
            grid_point_vector.push_back(gridpoint);
        }
    }

    //std::cout << leftup-translation_x << std::endl;
}

void jpsGraphicsView::zoom(int delta)
{

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(delta > 0)
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
        //create_grid();
    }
    else
    {
        // Zooming out

        gl_scale_f*=1.15;
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        catch_radius=10*gl_scale_f;
        catch_line_distance=10*gl_scale_f;
        //create_grid();
    }

}

void jpsGraphicsView::translations(QPointF old_pos)
{
    translation_x+=pos.x()-old_pos.x();
    translation_y+=pos.y()-old_pos.y();

    if (current_line!=nullptr)
    {
        //current_line->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        current_line->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);

    }
    if (_currentVLine!=nullptr)
    {
        //current_line->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        _currentVLine->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);

    }

    for (int i=0; i<line_vector.size(); ++i)
    {
        //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        line_vector[i]->get_line()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);

    }
    for (int i=0; i<caption_list.size(); ++i)
    {
        //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        qreal scalef = caption_list[i]->data(0).toReal();
        caption_list[i]->setTransform(QTransform::fromScale(1.0/scalef,1.0/scalef),true); // without this line translations won't work
        caption_list[i]->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);
        caption_list[i]->setTransform(QTransform::fromScale(scalef,scalef),true);

    }
    for (int i=0; i<grid_point_vector.size(); ++i)
    {
        //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        //grid_point_vector[i]->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
        grid_point_vector[i].setX(pos.x());
        grid_point_vector[i].setY(pos.y());
    }

    for (jpsLandmark* landmark:LLandmarks)
    {
        landmark->get_pixmap()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);

    }
    if (currentLandmarkRect!=nullptr)
    {
        currentLandmarkRect->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }
    for (QGraphicsEllipseItem* ellipse:_waypoints)
    {
        ellipse->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

}

void jpsGraphicsView::AutoZoom()
{
    QPointF min(line_vector[0]->get_line()->line().p1().x(),
            line_vector[0]->get_line()->line().p1().y());
    QPointF max(line_vector[0]->get_line()->line().p1().x(),
            line_vector[0]->get_line()->line().p1().y());
    for (jpsLineItem* line:line_vector)
    {
        ///x
        ///p1
        if (line->get_line()->line().p1().x()<min.x())
        {
            min.setX(line->get_line()->line().p1().x());
        }
        else if (line->get_line()->line().p1().x()>max.x())
        {
            max.setX(line->get_line()->line().p1().x());
        }

        ///p2
        if (line->get_line()->line().p2().x()<min.x())
        {
            min.setX(line->get_line()->line().p2().x());
        }
        else if (line->get_line()->line().p2().x()>max.x())
        {
            max.setX(line->get_line()->line().p2().x());
        }

        ///y
        ///p1
        if (line->get_line()->line().p1().y()<min.y())
        {
            min.setY(line->get_line()->line().p1().y());
        }
        else if (line->get_line()->line().p1().y()>max.y())
        {
            max.setY(line->get_line()->line().p1().y());
        }

        ///p2
        if (line->get_line()->line().p2().y()<min.y())
        {
            min.setY(line->get_line()->line().p2().y());
        }
        else if (line->get_line()->line().p2().y()>max.y())
        {
            max.setY(line->get_line()->line().p2().y());
        }
    }
    QPointF center((min.x()+max.x())/2.0,(min.y()+max.y())/2.0);


    ///scaling
    qreal width = (max.x()-min.x());
    qreal height = (max.y()-min.y());
    this->fitInView(min.x(),min.y(),width,height,Qt::KeepAspectRatio);
    ///adapting gl_scale_f
    gl_scale_f=1/this->transform().m11();

    ///translations
    QPointF old_pos;
    old_pos.setX(pos.x()+translation_x);
    old_pos.setY(pos.y()+translation_y);
    translations(old_pos);

}

void jpsGraphicsView::StatAssoDef()
{
   _assoDef=!_assoDef;
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
        emit remove_marked_lines();

        for (int i=0; i<marked_lines.size(); ++i)
        {
            QList<QPointF *> points = marked_lines[i]->get_intersectionVector();

            for (int j=0; j<points.size(); ++j)
            {
                delete points[j];
                intersect_point_vector.removeOne(points[j]);
                for (int k=0; k<marked_lines[i]->get_intersectLineVector().size(); ++k)
                {
                    // removing the intersectionPoint pointer from all lines which includes the point
                    marked_lines[i]->get_intersectLineVector()[k]->remove_intersectionPoint(points[j]);

                    // as marked_lines is removed it is has no intersections with any other line anymore
                    // so pointers of possible intersectionsLine have to be removed

                }


            }


            for (int k=0; k<marked_lines[i]->get_intersectLineVector().size(); ++k)
            {
                marked_lines[i]->get_intersectLineVector()[k]->remove_interLine(marked_lines[i]);
            }


            delete marked_lines[i]->get_line();
            //marked_lines[i]->set_line(nullptr);
            delete marked_lines[i];
            line_vector.removeOne(marked_lines[i]);


        }
        marked_lines.clear();

        //intersect_point_vector.clear();
        line_tracked=-1;
        emit lines_deleted();
        update();
    }

}

void jpsGraphicsView::delete_landmark()
{
    if (markedLandmark!=nullptr)
    {
        LLandmarks.removeOne(markedLandmark);
        delete markedLandmark->get_pixmap();
        delete markedLandmark;
        markedLandmark=nullptr;
        delete currentLandmarkRect;
        currentLandmarkRect=nullptr;
    }

}

void jpsGraphicsView::catch_landmark()
{
    if (currentSelectRect!=nullptr)
    {
        for (jpsLandmark* landmark:LLandmarks)
        {
            if (currentSelectRect->contains(QPointF(landmark->get_pixmap()->scenePos().x()-translation_x,
                                                    landmark->get_pixmap()->scenePos().y()-translation_y)))
            {
                select_landmark(landmark);
                return;
            }
        }
    }
}

void jpsGraphicsView::select_landmark(jpsLandmark* landmark)
{
    currentLandmarkRect=Scene->addRect(
                landmark->get_pixmap()->mapRectToScene(landmark->get_pixmap()->pixmap().rect()),QPen(Qt::red,0));
    markedLandmark=landmark;
}

void jpsGraphicsView::take_l_from_lineEdit(const qreal &length)
{
    if (current_line!=nullptr)
    {
        QLineF line(current_line->line());
        line.setLength(length);
        current_line->setLine(line);
        jpsLineItem* jpsline = new jpsLineItem(current_line);
        jpsline->set_type(statWall,statDoor,statExit);
        line_vector.push_back(jpsline);
        current_line=nullptr;
    }
    else if (_currentVLine!=nullptr)
    {
        QLineF line(_currentVLine->line());
        line.setLength(length);
        _currentVLine->setLine(line);
        //jpsLineItem* jpsline = new jpsLineItem(current_line);
        //jpsline->set_type(statWall,statDoor,statExit);
        //line_vector.push_back(jpsline);
        translated_pos.setX(_currentVLine->line().p2().x());
        translated_pos.setY(_currentVLine->line().p2().y());
        current_line = Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        emit set_focus_textedit();
        //translated_pos.setX(_currentVLine->line().p2().x());
        //translated_pos.setY(_currentVLine->line().p2().y());
        delete _currentVLine;
        _currentVLine=nullptr;
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
    statLandmark=false;
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
    statLandmark=false;
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
    statLandmark=false;
    if (statExit==false)
    {
        emit no_drawing();
    }
    else
    {
        currentPen.setColor(Qt::darkMagenta);
    }
}

bool jpsGraphicsView::statusLandmark()
{
    return statLandmark;
}

void jpsGraphicsView::en_disableLandmark()
{
    statLandmark=!statLandmark;
    statDoor=false;
    statWall=false;
    statExit=false;
    if (statLandmark==false)
    {
        emit no_drawing();
    }
}

bool jpsGraphicsView::statusExit()
{
    return statExit;
}











