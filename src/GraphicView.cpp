/**
 * \file        GraphicView.cpp
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
 * This class is setting up the drawing area. It enables the user to draw walls and doors.
 *
 **/


#include <QtGui>
#include "GraphicView.h"
#include <iostream>
#include <cmath>
#include <memory>
#include <QMessageBox>
#include "datamanager.h"



jpsGraphicsView::jpsGraphicsView(QWidget* parent, jpsDatamanager *datamanager):QGraphicsView(parent)
{
    _datamanager=datamanager;
    current_line=nullptr;
    _currentVLine=nullptr;
    current_caption=nullptr;
    //current_line_mark=nullptr;
    midbutton_hold=false;
    translation_x=0;
    translation_y=0;//this->height();
    anglesnap=false;
    id_counter=0;
    _scaleFactor=1.0;
    _gridSize=1.0;
    gl_scale_f=.01*_scaleFactor;
    //scale(10,10);
    catch_radius=10*gl_scale_f;
    catch_line_distance=10*gl_scale_f;
    //Set-up the view
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    point_tracked=false;
    line_tracked=-1;
    current_rect=nullptr;
    currentSelectRect=nullptr;
    gridmap=nullptr;
    objectsnap=false;
    _gridmode=false;
    statWall=false;
    statDoor=false;
    statExit=false;
    _statHLine=false;
    _statCopy=0;
    statLandmark=false;
    markedLandmark=nullptr;
    currentLandmarkRect=nullptr;
    currentPen.setColor(Qt::black);
    currentPen.setCosmetic(true);
    currentPen.setWidth(2);
    this->scale(1/gl_scale_f,-1/gl_scale_f);
    _currentTrackedPoint=nullptr;
    _statLineEdit=false;
    intersection_point=nullptr;
    _statDefConnections=0;

    lines_collided=false;
    _posDef=false;
    _regionDef=false;


    //m_graphView->setFixedSize(1600, 900);
    //m_graphView->setScene(m_graphScen);

    //setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);


    //setRenderHint(QPainter::NonCosmeticDefaultPen);
    // setRenderHint(QPainter::Antialiasing);

    //m_graphView->setAlignment(nullptr);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCursor(Qt::CrossCursor);



    //Set-up the scene
    Scene = new GraphicScene(this);

    setScene(Scene);
    setSceneRect(0, 0, 1920, 1080);

}

jpsGraphicsView::~jpsGraphicsView()
{
    delete_all(true);
    delete Scene;
}

QGraphicsScene *jpsGraphicsView::GetScene()
{
    return Scene;
}

const qreal &jpsGraphicsView::GetTranslationX() const
{
    return translation_x;
}

const qreal &jpsGraphicsView::GetTranslationY() const
{
    return translation_y;
}

const qreal &jpsGraphicsView::GetScaleF() const
{
    return gl_scale_f;
}

void jpsGraphicsView::SetDatamanager(jpsDatamanager *datamanager)
{
    _datamanager=datamanager;
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

    if (anglesnap==true)
    {
        if (current_line!=nullptr)
        {
            use_anglesnap(current_line,5);
        }
    }

    if (_currentVLine!=nullptr)
    {
        // vline will be deleted if it is not thrown horizontally or vertically
        if(!use_anglesnap(_currentVLine,15))
        {
            delete _currentVLine;
            _currentVLine=nullptr;
        }

    }


    if (midbutton_hold)
    {
        translations(old_pos);
    }

    else if(leftbutton_hold && currentSelectRect)
    {
        currentSelectRect->setRect(QRectF(QPointF(currentSelectRect->rect().x(),currentSelectRect->rect().y())
                                         ,QPointF(translated_pos.x(),translated_pos.y())));
    }

    translated_pos.setX(pos.x()-translation_x);
    translated_pos.setY(pos.y()-translation_y);

    if (_gridmode)
        use_gridmode();

    if (objectsnap)
    {
        catch_points();

        //VLine
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
        if (current_line->line().isNull())
            current_line->setVisible(false);
        else
            current_line->setVisible(true);

        //}
        //As line length has changed
        emit LineLengthChanged();
    }
    if (_currentVLine!=nullptr)
    {
        emit set_focus_textedit();
        _currentVLine->setLine(_currentVLine->line().x1(),_currentVLine->line().y1(),translated_pos.x(),translated_pos.y());
    }

    // see if two lines collided FIX ME!!!
    //line_collision();

    emit mouse_moved();
    update();

}


void jpsGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
{

    if (mouseEvent->button() == Qt::LeftButton)
    {

        if (statWall || statDoor || statExit || _statHLine)
        {
            // If line is edited currently
            if (_statLineEdit)
            {
                for (jpsLineItem* line:line_vector)
                {
                    locate_intersection(marked_lines.first(),line);
                }
                current_line=nullptr;
                _statLineEdit=false;
                line_tracked=1;
                emit no_drawing();
            }
            else
            {
                drawLine();
            }
        }
        else if (statLandmark==true)
        {
            addLandmark();
        }
        else
        {
            if (_statDefConnections==1)
            {
                emit DefConnection1Completed();
            }
            //LineEdit
            else if (_currentTrackedPoint!=nullptr && line_tracked==1)
            {
                EditLine(_currentTrackedPoint);
                _currentTrackedPoint=nullptr;
                line_tracked=-1;
            }
            else if (_statCopy!=0)
            {
                if (_statCopy==1)
                {
                    _copyOrigin=return_Pos();
                    _statCopy += 1;
                }
                else
                    Copy_lines(return_Pos()-_copyOrigin);
            }
            else
            {
                //Select_mode
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
    {   /// To avoid deleting of edited line
        if (line_tracked==1)
        {
            current_line=nullptr;
        }
        emit no_drawing();
    }

}



void jpsGraphicsView::unmark_all_lines()
{
    QPen pen = QPen(Qt::black,2);
    pen.setCosmetic(true);

    for (int i=0; i<marked_lines.size();i++)
    {
        pen.setColor(QColor(marked_lines[i]->get_defaultColor()));
        marked_lines[i]->get_line()->setPen(pen);
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
    QString name="Landmark"+QString::number(_datamanager->GetLandmarkCounter());
    jpsLandmark* landmark = new jpsLandmark(pixmapItem,name,pixmapItem->scenePos());
    //text immediately under the pixmap
    QGraphicsTextItem* text = Scene->addText(name);
    text->setPos(QPointF(landmark->GetPos().x(),
                         landmark->GetPos().y()+0.2));// landmark->GetPos().x()+translation_x,landmark->GetPos().y()+translation_y);
    //text->setScale(gl_scale_f);

    text->setData(0,0.01);
    text->setTransform(QTransform::fromScale(0.01,-0.01),true);


    landmark->SetPixMapText(text);

    _datamanager->new_landmark(landmark);

}

void jpsGraphicsView::addLandmark(const QPointF &pos)
{
    QPixmap pixmap("../jupedsim/forms/statue.jpg");

    QGraphicsPixmapItem* pixmapItem = Scene->addPixmap(pixmap);
    pixmapItem->setScale(0.002);
    pixmapItem->setTransform(QTransform::fromTranslate(pos.x()-pixmap.width()/1000.,pos.y()
                          +pixmap.height()/1000.));
    pixmapItem->setTransform(QTransform::fromScale(1,-1),true);
    pixmapItem->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    QString name="Landmark"+QString::number(_datamanager->GetLandmarkCounter());
    jpsLandmark* landmark = new jpsLandmark(pixmapItem,name,pos);
    //text immediately under the pixmap
    QGraphicsTextItem* text = Scene->addText(name);
    text->setPos(QPointF(landmark->GetPos().x(),
                         landmark->GetPos().y()+0.2));// landmark->GetPos().x()+translation_x,landmark->GetPos().y()+translation_y);
    //text->setScale(gl_scale_f);

    text->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    text->setData(0,0.01);
    text->setTransform(QTransform::fromScale(0.01,-0.01),true);


    landmark->SetPixMapText(text);

    _datamanager->new_landmark(landmark);
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



QGraphicsRectItem *jpsGraphicsView::GetCurrentSelectRect()
{
    return currentSelectRect;
}

void jpsGraphicsView::SetStatDefConnections(const int &stat)
{
    _statDefConnections=stat;
}


void jpsGraphicsView::ShowYAHPointer(const QPointF &pos, const qreal &dir)
{
    for (QGraphicsLineItem* lineItem:_yahPointer)
    {
        delete lineItem;
    }
    _yahPointer.clear();

    _yahPointer.push_back(Scene->addLine(pos.x(),pos.y(),pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),QPen(Qt::blue,0)));
    _yahPointer.push_back(Scene->addLine(pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),pos.x()+0.2*std::cos(dir+M_PI/4.0),pos.y()+0.2*std::sin(dir+M_PI/4.0),QPen(Qt::blue,0)));
    _yahPointer.push_back(Scene->addLine(pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),pos.x()+0.2*std::cos(dir-M_PI/4.0),pos.y()+0.2*std::sin(dir-M_PI/4.0),QPen(Qt::blue,0)));

    for (QGraphicsLineItem* lineItem:_yahPointer)
    {
        lineItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    }


}


void jpsGraphicsView::ShowConnections(QList<ptrConnection> cons)
{
    ClearConnections();
    for (ptrConnection con:cons)
    {
        QGraphicsLineItem* line = Scene->addLine(QLineF(con->GetLandmarks().first->GetPos(),con->GetLandmarks().second->GetPos()),QPen(Qt::blue,0));
        _connections.push_back(line);


        line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);


    }
}

void jpsGraphicsView::ClearConnections()
{
    for (QGraphicsLineItem* line:_connections)
    {
        delete line;
    }
    _connections.clear();
}




void jpsGraphicsView::ActivateLineGrid()
{
    Scene->SetGrid("Line");

}

void jpsGraphicsView::ActivatePointGrid()
{
    Scene->SetGrid("Point");

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
        // Select lines that are located within the rectangle
        if (!(statWall==true || statDoor==true || statExit==true || statLandmark==true))
        {
            leftbutton_hold=false;

            //for connection setting
            if (_statDefConnections==2)
            {
                emit DefConnection2Completed();
            }
            else if (currentSelectRect!=nullptr)
            {
                if (_posDef)
                {
                    //Landmark position definition
                    emit PositionDefCompleted();
                    _posDef=false;
                }
                else if (_regionDef)
                {
                    emit RegionDefCompleted();
                    _regionDef=false;
                }
                else
                {
                    // Select lines by creating a rect with the cursor
                    catch_lines();

                    // unmark Landmark is possible
                    unmarkLandmark();
                    // Look for landmarks with position in the currentSelectRect
                    catch_landmark();
                }
                delete currentSelectRect;
                currentSelectRect=nullptr;

            }

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



const QPointF &jpsGraphicsView::return_Pos() const
{
    return translated_pos;
}

void jpsGraphicsView::delete_all(bool final)
{
    unmark_all_lines();
    unmarkLandmark();

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

    //delete landmarks

    _datamanager->remove_all_landmarks();


    intersect_point_vector.clear();
    marked_lines.clear();


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

bool jpsGraphicsView::use_anglesnap(QGraphicsLineItem* currentline, int accuracy)

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

void jpsGraphicsView::use_gridmode()
{
    if ((std::fmod(std::fabs(translated_pos.x()),_gridSize)<=_gridSize*0.1 || std::fmod(std::fabs(translated_pos.x()),_gridSize)>=_gridSize*0.9) &&
         (std::fmod(std::fabs(translated_pos.y()),_gridSize)<=_gridSize*0.1 || std::fmod(std::fabs(translated_pos.y()),_gridSize)>=_gridSize*0.9))
    {
        bool posx_positiv=true;
        bool posy_positiv=true;
        if (translated_pos.x()<0)
        {
            translated_pos.setX(std::fabs(translated_pos.x()));
            posx_positiv=false;
        }

        if (translated_pos.y()<0)
        {
            translated_pos.setY(std::fabs(translated_pos.y()));
            posy_positiv=false;
        }

        if (std::fmod(translated_pos.x(),_gridSize)<=_gridSize*0.1)
            translated_pos.setX(translated_pos.x()-std::fmod(translated_pos.x(),_gridSize));
        else
            translated_pos.setX(translated_pos.x()+(_gridSize-std::fmod(translated_pos.x(),_gridSize)));

        if (std::fmod(translated_pos.y(),_gridSize)<=_gridSize*0.1)
            translated_pos.setY(translated_pos.y()-std::fmod(translated_pos.y(),_gridSize));
        else
            translated_pos.setY(translated_pos.y()+(_gridSize-std::fmod(translated_pos.y(),_gridSize)));

        if (!posx_positiv)
            translated_pos.setX(translated_pos.x()*(-1));

        if (!posy_positiv)
            translated_pos.setY(translated_pos.y()*(-1));

        current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
        point_tracked=true;
        _currentTrackedPoint= &translated_pos;


    }
    else
        point_tracked=false;
}

void jpsGraphicsView::catch_points()
{
    //Searching for startpoints of all lines near the current cursor position
    for (int i=0; i<line_vector.size(); ++i){

        // range chosen: 10 (-5:5) (has to be changed soon)
        if (line_vector[i]->get_line()->line().x1()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x1()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y1()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y1()<=(translated_pos.y()+catch_radius)){
            // in this case the cursor is working with global coordinates. So the method 'mapToGlobal' must be used

            //to avoid the tracking of the coords of an edited line
            if (line_vector[i]->get_line()==current_line)
            {
                continue;
            }

            translated_pos.setX(line_vector[i]->get_line()->line().x1());
            translated_pos.setY(line_vector[i]->get_line()->line().y1());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x1()),translate_back_y(line_vector[i].y1()))));
            //bool is used to tell paint device to draw a red rect if a point was tracked
            point_tracked=true;
            _currentTrackedPoint= &translated_pos;
            //QPen pen;
            //pen.setColor('red');
            if (current_rect==nullptr)
                current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
            // if a point was tracked there is no need to look for further points ( only one point can be tracked)

            return;
        }

        //Searching for endpoints of all lines near the current cursor position
        else if (line_vector[i]->get_line()->line().x2()>=(translated_pos.x()-catch_radius) && line_vector[i]->get_line()->line().x2()<=(translated_pos.x()+catch_radius) && line_vector[i]->get_line()->line().y2()>=(translated_pos.y()-catch_radius) && line_vector[i]->get_line()->line().y2()<=(translated_pos.y()+catch_radius)){
            // see above

            //to avoid the tracking of the coords of an edited line
            if (line_vector[i]->get_line()==current_line)
            {
                continue;
            }


            translated_pos.setX(line_vector[i]->get_line()->line().x2());
            translated_pos.setY(line_vector[i]->get_line()->line().y2());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x2()),translate_back_y(line_vector[i].y2()))));
            point_tracked=true;
            if (current_rect==nullptr)
                current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
            _currentTrackedPoint= &translated_pos;
            return;
        }
    }
    // if no start- or endpoint was tracked it is searched for intersections- Points

    // see above
    for (int j=0; j<intersect_point_vector.size(); j++)
    {
        if (intersect_point_vector[j]->x()>=(translated_pos.x()-catch_radius) && intersect_point_vector[j]->x()<=(translated_pos.x()+catch_radius) && intersect_point_vector[j]->y()>=(translated_pos.y()-catch_radius) && intersect_point_vector[j]->y()<=(translated_pos.y()+catch_radius))
        {
            translated_pos.setX(intersect_point_vector[j]->x());
            translated_pos.setY(intersect_point_vector[j]->y());
            if (current_rect==nullptr)
            current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
                point_tracked=true;
            return;
        }
    }

    // Catch origin
    if (!_origin.isEmpty())
        if (std::fabs(translated_pos.x())<=catch_radius && std::fabs(translated_pos.y())<=catch_radius)
        {
            translated_pos.setX(0);
            translated_pos.setY(0);
            if (current_rect==nullptr)
                current_rect=Scene->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));

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

    // if no point was tracked bool is set back to false
    point_tracked=false;
    return;
}

void jpsGraphicsView::catch_lines()
{
    //catch lines (only possible if wall is disabled)
    // if current rect was build up moving the cursor to the left ->
    // whole line has to be within the rect to select the line
    line_tracked=-1;
    if (currentSelectRect->rect().width()<0)
    {
        for (auto &item:line_vector)
        {
            if (currentSelectRect->contains(item->get_line()->line().p1())
                    && currentSelectRect->contains(item->get_line()->line().p2()))
            {
                select_line(item);



            }
        }
    }
    // if current rect was build up moving the cursor to the right ->
    // throwing the select rect only over a part of a line is sufficent to select it
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
    if (current_line==nullptr) // if the mouse was pressed first of two times
    {
        //Determining first point of line

        // all two points of the line are inited with the cursorcoordinates
        current_line = Scene->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setVisible(false);
        //current_line->translate(translation_x,translation_y);
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        emit set_focus_textedit();

    }

    // if the mouse was pressed secondly of two times
    else
    {

        jpsLineItem* lineItem= new jpsLineItem(current_line);
        lineItem->set_id(id_counter);
        id_counter++;

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

        lineItem->set_type(statWall,statDoor,statExit,_statHLine);
        line_vector.push_back(lineItem);

        //reset pointer
        current_line=nullptr;

        //Undo
        RecordUndoLineAction("LineAdded",lineItem->GetType(),lineItem->get_id(),lineItem->get_line()->line());

        //drawLine();



    }

    //Vline
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
        QPen pen = QPen(Qt::red,2);
        pen.setCosmetic(true);
        mline->get_line()->setPen(pen);
        marked_lines.push_back(mline);
        line_tracked=1;
    }
    else
    {
        QPen pen = QPen(Qt::black,2);
        pen.setCosmetic(true);
        mline->get_line()->setPen(pen);
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
    _statCopy=0;
    // if drawing was canceled by pushing ESC
    if (current_line!=nullptr)
    {
        //not completed line will be deleted
        delete current_line;
        current_line=nullptr;
    }
    if (_currentVLine!=nullptr)
    {
        //VLine will be deleted
        delete _currentVLine;
        _currentVLine=nullptr;
    }

}

jpsLineItem* jpsGraphicsView::addLineItem(const qreal &x1,const qreal &y1,const qreal &x2,const qreal &y2,const QString &type)
{
    QPen pen = QPen(Qt::black,0);
    pen.setCosmetic(true);

    current_line=Scene->addLine(x1,y1,x2,y2,pen);
    current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    jpsLineItem* newLine = new jpsLineItem(current_line);
    newLine->set_id(id_counter);
    id_counter++;

    if (type=="Door")
    {
        newLine->set_Door();
    }
    else if (type=="Exit")
    {
        newLine->set_Exit();
    }
    else if (type=="HLine")
    {
        newLine->SetHLine();
    }
    else
    {
        newLine->set_Wall();
    }

    pen.setColor(newLine->get_defaultColor());
    newLine->get_line()->setPen(pen);
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

jpsLineItem *jpsGraphicsView::addLineItem(const QLineF &line, const QString &type)
{
    return addLineItem(line.p1().x(),line.p1().y(),line.p2().x(),line.p2().y(),type);
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

        delete current_rect;
        current_rect=nullptr;

        point_tracked=false;

        if (marked_lines.first()->get_line()->line().p1()==*point)
        {
            RecordUndoLineAction("LineEdited",marked_lines.first()->GetType(),marked_lines.first()->get_id(),marked_lines.first()->get_line()->line());
            current_line=marked_lines[0]->get_line();
            marked_lines[0]->get_line()->setLine(marked_lines[0]->get_line()->line().x2(),marked_lines[0]->get_line()->line().y2(),translated_pos.x(),translated_pos.y());
            //current_line=Scene->addLine(marked_lines[0]->get_line()->line().p2().x(),
             //       marked_lines[0]->get_line()->line().p2().y(),translated_pos.x(),translated_pos.y(),QPen(Qt::red,0));
            //current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            emit set_focus_textedit();
        }

        else if (marked_lines.first()->get_line()->line().p2()==*point)
        {
            RecordUndoLineAction("LineEdited",marked_lines.first()->GetType(),marked_lines.first()->get_id(),marked_lines.first()->get_line()->line());
            current_line=marked_lines[0]->get_line();
            marked_lines[0]->get_line()->setLine(marked_lines[0]->get_line()->line().x1(),marked_lines[0]->get_line()->line().y1(),translated_pos.x(),translated_pos.y());

                    //->get_line()->line().p1().x(),
                    //marked_lines[0]->get_line()->line().p1().y(),translated_pos.x(),translated_pos.y(),QPen(Qt::red,0);
            //current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            emit set_focus_textedit();
        }

        else
            return;

        //line_tracked=1;
        RemoveIntersections(marked_lines.first());
        _statLineEdit=true;

        //delete_marked_lines();
        en_disableWall();

    }
}

qreal jpsGraphicsView::ReturnLineLength()
{
    return current_line->line().length();
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

void jpsGraphicsView::RecordUndoLineAction(const QString& name, const QString& type, const int& itemID, const QLineF &oldLine)
{
    _undoStack.PushNewAction(LineAction(name,type,itemID,oldLine));
}

void jpsGraphicsView::RecordRedoLineAction(const QString &name, const QString &type, const int& itemID, const QLineF &oldLine)
{
    _redoStack.PushNewAction(LineAction(name,type,itemID,oldLine));
}

void jpsGraphicsView::UndoLineEdit(const int& lineID, const QLineF& old_line)
{
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_id()==lineID)
        {
           RecordRedoLineAction("LineEdited",lineItem->GetType(),lineItem->get_id(),lineItem->get_line()->line());
           lineItem->get_line()->setLine(old_line);
           break;
        }
    }
}

void jpsGraphicsView::RedoLineEdit(const int &lineID, const QLineF &old_line)
{
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_id()==lineID)
        {
           RecordUndoLineAction("LineEdited",lineItem->GetType(),lineItem->get_id(),lineItem->get_line()->line());
           lineItem->get_line()->setLine(old_line);
           break;
        }
    }
}

void jpsGraphicsView::Undo()
{
    if (!_undoStack.IsEmpty())
    {
        const LineAction recentAction = _undoStack.GetRecentAction();

        if (recentAction.GetName()=="LineDeleted")
        {
            addLineItem(recentAction.GetOldLine().p1().x(),recentAction.GetOldLine().p1().y(),recentAction.GetOldLine().p2().x(),
                        recentAction.GetOldLine().p2().y(),recentAction.GetType());

            RecordRedoLineAction("LineAdded",recentAction.GetType(),id_counter-1,QLineF(0,0,0,0));
        }
        else if (recentAction.GetName()=="LineAdded")
        {
            RecordRedoLineAction("LineDeleted",line_vector.back()->GetType(),line_vector.back()->get_id(),
                                 line_vector.back()->get_line()->line());
            RemoveLineItem(line_vector.back());
        }
        else if (recentAction.GetName()=="LineEdited")
        {
            UndoLineEdit(recentAction.GetItemID(),recentAction.GetOldLine());
        }
    }

}

void jpsGraphicsView::Redo()
{
    if (!_redoStack.IsEmpty())
    {
        const LineAction recentAction = _redoStack.GetRecentAction();

        if (recentAction.GetName()=="LineDeleted")
        {
            addLineItem(recentAction.GetOldLine().p1().x(),recentAction.GetOldLine().p1().y(),recentAction.GetOldLine().p2().x(),
                        recentAction.GetOldLine().p2().y(),recentAction.GetType());

            RecordUndoLineAction("LineAdded",recentAction.GetType(),id_counter-1,QLineF(0,0,0,0));
        }

        else if (recentAction.GetName()=="LineAdded")
        {
            RecordUndoLineAction("LineDeleted",line_vector.back()->GetType(),line_vector.back()->get_id(),
                                 line_vector.back()->get_line()->line());
            RemoveLineItem(line_vector.back());

        }
        else if (recentAction.GetName()=="LineEdited")
        {
            RedoLineEdit(recentAction.GetItemID(),recentAction.GetOldLine());

        }


    }
}


void jpsGraphicsView::line_collision() //FIX ME!!!
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


void jpsGraphicsView::zoom(int delta)
{

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //exclude origin from scaling
    ShowOrigin();

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(delta > 0)
    {
        // Zoom in

        // for (int i=0; i<caption_list.size(); i++)
        // {
        //    caption_list[i]->setTransform(QTransform::fromScale(1/gl_scale_f,1/gl_scale_f));
        //}
        _scaleFactor*=1/1.15;
        gl_scale_f*=1/1.15;
        scale(scaleFactor, scaleFactor);
        catch_radius=10*gl_scale_f;
        catch_line_distance=10*gl_scale_f;
        Scene->ChangeGridSize(this->CalcGridSize());
        //create_grid();
    }
    else
    {
        // Zooming out

        _scaleFactor*=1.15;
        gl_scale_f*=1.15;
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        catch_radius=10*gl_scale_f;
        catch_line_distance=10*gl_scale_f;
        Scene->ChangeGridSize(this->CalcGridSize());
        //create_grid();
    }

    //set origin back to the scene
    ShowOrigin();

}

void jpsGraphicsView::translations(QPointF old_pos)
{
    translation_x+=pos.x()-old_pos.x();
    translation_y+=pos.y()-old_pos.y();

    /// translate the background grid
    Scene->ChangeTranslation(translation_x,translation_y);

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
        /// To avoid double sized translation of edited line
        if (current_line==line_vector[i]->get_line())
        {
            continue;
        }
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
//    for (int i=0; i<grid_point_vector.size(); ++i)
//    {
//        //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
//        //grid_point_vector[i]->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
//        grid_point_vector[i].setX(pos.x());
//        grid_point_vector[i].setY(pos.y());
//    }

    for (jpsLandmark* landmark:_datamanager->get_landmarks())
    {


    }
    if (currentLandmarkRect!=nullptr)
    {
        currentLandmarkRect->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }
    if (gridmap!=nullptr)
    {
        gridmap->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }
    for (QGraphicsLineItem* lineItem:_yahPointer)
    {
        lineItem->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }
    for (jpsLandmark* landmark:_datamanager->get_landmarks())
    {
        if (landmark->GetTextItem()!=nullptr)
        {
            qreal scalef = landmark->GetTextItem()->data(0).toReal();
            landmark->GetTextItem()->setTransform(QTransform::fromScale(1.0/scalef,1.0/scalef),true); // without this line translations won't work
            landmark->GetTextItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);
            landmark->GetTextItem()->setTransform(QTransform::fromScale(scalef,scalef),true);
        }

        if (landmark->GetPixmapTextItem()!=nullptr)
        {
            qreal scalef = landmark->GetPixmapTextItem()->data(0).toReal();
            landmark->GetPixmapTextItem()->setTransform(QTransform::fromScale(1.0/scalef,1.0/scalef),true); // without this line translations won't work
            landmark->GetPixmapTextItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);
            landmark->GetPixmapTextItem()->setTransform(QTransform::fromScale(scalef,scalef),true);
        }

        landmark->GetPixmap()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);

        if (landmark->GetEllipseItem()!=nullptr)
            landmark->GetEllipseItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    for (jpsConnection* connection:_datamanager->GetAllConnections())
    {
        connection->GetLineItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    for (jpsRegion* region:_datamanager->GetRegions())
    {
        if (region->GetTextItem()!=nullptr)
        {
            qreal scalef = region->GetTextItem()->data(0).toReal();
            region->GetTextItem()->setTransform(QTransform::fromScale(1.0/scalef,1.0/scalef),true); // without this line translations won't work
            region->GetTextItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),-pos.y()+old_pos.y()), true);
            region->GetTextItem()->setTransform(QTransform::fromScale(scalef,scalef),true);
        }

        if (region->GetEllipseItem()!=nullptr)
            region->GetEllipseItem()->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    for (QGraphicsLineItem* lineItem:_connections)
    {
        lineItem->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    for (QGraphicsLineItem* lineItem:_origin)
    {
       lineItem->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
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
        //x
        //p1
        if (line->get_line()->line().p1().x()<min.x())
        {
            min.setX(line->get_line()->line().p1().x());
        }
        else if (line->get_line()->line().p1().x()>max.x())
        {
            max.setX(line->get_line()->line().p1().x());
        }

        //p2
        if (line->get_line()->line().p2().x()<min.x())
        {
            min.setX(line->get_line()->line().p2().x());
        }
        else if (line->get_line()->line().p2().x()>max.x())
        {
            max.setX(line->get_line()->line().p2().x());
        }

        //y
        //p1
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
    //QPointF center((min.x()+max.x())/2.0,(min.y()+max.y())/2.0);


    //scaling
    qreal width = (max.x()-min.x());
    qreal height = (max.y()-min.y());
    // To ensure the functionality of fitInView
    this->setSceneRect(min.x(),min.y(),width,height);
    this->fitInView(min.x(),min.y(),width,height,Qt::KeepAspectRatio);

    //adapting gl_scale_f
    gl_scale_f=1/this->transform().m11();

    //translations
    QPointF old_pos;
    old_pos.setX(pos.x()+translation_x);
    old_pos.setY(pos.y()+translation_y);
    translations(old_pos);

}

qreal jpsGraphicsView::CalcGridSize()
{
    int cFactor;
    if (_scaleFactor<1.0)
    {
        cFactor = std::round(1/_scaleFactor);//std::round(_scaleFactor);
    }
    else
        cFactor = std::round(_scaleFactor);

    int n=0;
    while (cFactor>std::pow(2,n))
    {
        n++;
    }
    qreal gridSize;
    if (_scaleFactor<1.0)
        gridSize=1/std::pow(2,n);
    else
        gridSize=std::pow(2,n);

    _gridSize=gridSize;
    return gridSize;

}

void jpsGraphicsView::ShowOrigin()
{
    if (_origin.isEmpty())
    {
        //Scene->DrawOrigin();
        _origin.push_back(Scene->addLine(0,0,0,0+gl_scale_f*100,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0,0+gl_scale_f*100,0-gl_scale_f*10,0+gl_scale_f*100-gl_scale_f*10,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0,0+gl_scale_f*100,0+gl_scale_f*10,0+gl_scale_f*100-gl_scale_f*10,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0,0,0+gl_scale_f*100,0,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0+gl_scale_f*100,0,gl_scale_f*100-gl_scale_f*10,-gl_scale_f*10,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0+gl_scale_f*100,0,gl_scale_f*100-gl_scale_f*10,+gl_scale_f*10,QPen(Qt::black,gl_scale_f*2)));
        //Y
        _origin.push_back(Scene->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*5,gl_scale_f*100-gl_scale_f*45,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*15,gl_scale_f*100-gl_scale_f*45,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*60,QPen(Qt::black,gl_scale_f*2)));
        //X
        _origin.push_back(Scene->addLine(0+gl_scale_f*40,0-gl_scale_f*15,gl_scale_f*50,-gl_scale_f*5,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(Scene->addLine(0+gl_scale_f*40,0-gl_scale_f*5,gl_scale_f*50,-gl_scale_f*15,QPen(Qt::black,gl_scale_f*2)));

        for (QGraphicsLineItem* lineItem:_origin)
        {
           lineItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        }
    }
    else
    {
        for (QGraphicsLineItem* line:_origin)
        {
            delete line;
        }
        _origin.clear();
    }
}

void jpsGraphicsView::StatPositionDef()
{
    _posDef=!_posDef;
}

void jpsGraphicsView::ChangeRegionStatDef()
{
    _regionDef=!_regionDef;
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

            RecordUndoLineAction("LineDeleted",marked_lines[i]->GetType(),marked_lines[i]->get_id(),marked_lines[i]->get_line()->line());

            RemoveIntersections(marked_lines[i]);


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

void jpsGraphicsView::RemoveLineItem(jpsLineItem *mline)
{
    RemoveIntersections(mline);
    line_vector.removeOne(mline);
    delete mline->get_line();
    delete mline;
    emit lines_deleted();

}

void jpsGraphicsView::RemoveLineItem(const QLineF &line)
{
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_line()->line()==line)
        {
            unmark_all_lines();
            select_line(lineItem);
            delete_marked_lines();
        }
    }

}

void jpsGraphicsView::RemoveIntersections(jpsLineItem *lineItem)
{
    QList<QPointF *> points = lineItem->get_intersectionVector();

    for (int j=0; j<points.size(); ++j)
    {
        delete points[j];
        intersect_point_vector.removeOne(points[j]);
        for (int k=0; k<lineItem->get_intersectLineVector().size(); ++k)
        {
            // removing the intersectionPoint pointer from all lines which includes the point
            lineItem->get_intersectLineVector()[k]->remove_intersectionPoint(points[j]);

            // as marked_lines is removed it has no intersections with any other line anymore
            // so pointers of possible intersectionsLine have to be removed

        }
    }


    for (int k=0; k<lineItem->get_intersectLineVector().size(); ++k)
    {
        lineItem->get_intersectLineVector()[k]->remove_interLine(lineItem);
    }
}

void jpsGraphicsView::SelectAllLines()
{
    marked_lines.clear();
    for (jpsLineItem* line:line_vector)
    {
        select_line(line);
    }
}

void jpsGraphicsView::delete_landmark()
{
    if (markedLandmark!=nullptr)
    {
        _datamanager->remove_landmark(markedLandmark);
        markedLandmark=nullptr;
        delete currentLandmarkRect;
        currentLandmarkRect=nullptr;
    }

}

void jpsGraphicsView::catch_landmark()
{
    if (currentSelectRect!=nullptr)
    {
        for (jpsLandmark* landmark:_datamanager->get_landmarks())
        {
            if (currentSelectRect->contains(QPointF(landmark->GetPixmap()->scenePos().x()-translation_x,
                                                    landmark->GetPixmap()->scenePos().y()-translation_y)))
            {
                select_landmark(landmark);
                return;
            }
        }
    }
}

void jpsGraphicsView::select_landmark(jpsLandmark* landmark)
{
    unmarkLandmark();
    currentLandmarkRect=Scene->addRect(
                landmark->GetPixmap()->mapRectToScene(landmark->GetPixmap()->pixmap().rect()),QPen(Qt::red,0));
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
        jpsline->set_id(id_counter);
        id_counter++;
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

void jpsGraphicsView::change_gridmode()
{
    _gridmode=!_gridmode;
    Scene->ChangeGridmode(_gridmode);
    Scene->update();
}

void jpsGraphicsView::en_disableWall()
{
    statWall=!statWall;
    statDoor=false;
    statExit=false;
    _statHLine=false;
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

void jpsGraphicsView::change_stat_anglesnap()
{
    anglesnap=!anglesnap;
}

bool jpsGraphicsView::get_stat_anglesnap()
{
    return anglesnap;
}

void jpsGraphicsView::en_disableDoor()
{
    statDoor=!statDoor;
    statExit=false;
    statWall=false;
    statLandmark=false;
    _statHLine=false;
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
    _statHLine=false;
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

bool jpsGraphicsView::statusHLine()
{
    return _statHLine;
}

void jpsGraphicsView::en_disableHLine()
{
    _statHLine=!_statHLine;
    statExit=false;
    statDoor=false;
    statWall=false;
    statLandmark=false;
    _statCopy=0;
    if (_statHLine==false)
    {
        emit no_drawing();
    }
    else
    {
        currentPen.setColor(Qt::darkCyan);
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
    _statCopy=0;
    if (statLandmark==false)
    {
        emit no_drawing();
    }
}

bool jpsGraphicsView::statusExit()
{
    return statExit;
}

void jpsGraphicsView::start_Copy_function()
{
    _statCopy=1;
}

void jpsGraphicsView::Copy_lines(const QPointF& delta)
{
    for (jpsLineItem* line:marked_lines)
    {
       addLineItem(line->get_line()->line().x1()+delta.x(),line->get_line()->line().x2()+delta.x(),
                   line->get_line()->line().y1()+delta.y(),line->get_line()->line().y2()+delta.y(),
                   line->GetType());
    }
    _statCopy=0;
}
