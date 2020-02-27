/**
 * \file        GraphicView.cpp
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
#include "elementtypes.h"

jpsGraphicsView::jpsGraphicsView(QWidget* parent, jpsDatamanager *datamanager):QGraphicsView(parent)
{
    qDebug("Enter jpsGraphicsView::jpsGraphicsView");
    //Set-up data container
    _datamanager=datamanager;

    //Set-up parameters
    current_line=nullptr;
    _currentVLine=nullptr;
    current_caption=nullptr;
    current_rect=nullptr;

    currentSelectRect=nullptr;
    id_counter=0;
    point_tracked=false;
    line_tracked=-1;
    _statCopy=0;
    markedLandmark=nullptr;
    currentLandmarkRect=nullptr;
    _currentTrackedPoint=nullptr;
    _statLineEdit=false;
    intersection_point=nullptr;
    _statDefConnections=0;
    stat_break_ = false;

    //current_line_mark=nullptr;

    //Set-up viewing
    translation_x=0;
    translation_y=0;//this->height();
    anglesnap=false;
    _scaleFactor=1.0;
    _gridSize=1.0;
    gl_scale_f=.01*_scaleFactor;
    //scale(10,10);
    catch_radius=10*gl_scale_f;
    catch_line_distance=10*gl_scale_f;
    //Set-up the view
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    gridmap=nullptr;
    objectsnap= true;
    start_endpoint_snap=false;
    intersectionspoint_snap=false;
    centerpoint_snap=false;
    linepoint_snap=false;
    _gridmode=false;
    drawingMode = SelectMode;
    statzoomwindows=false;
    currentPen.setColor(Qt::black);
    currentPen.setCosmetic(true);
    currentPen.setWidth(2);
    this->scale(1/gl_scale_f,-1/gl_scale_f);
    lines_collided=false;
    _posDef=false;
    _regionDef=false;
    midbutton_hold=false;
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
    setCursor(Qt::CrossCursor);

    //Grid Mode
    _translationX=0.0;
    _translationY=0.0;
    _gridmode=false;
    _statgrid="Line";
    _gridSize=1.0;

    //SourceMode
//    sourceGroup = new QGraphicsItemGroup;
    currentSource = nullptr;

    //GoalMode
    currentGoal = nullptr;
    qDebug("Leave GraphicsView::jpsGraphicsView");

    // Backgound
    background = nullptr;
}

jpsGraphicsView::~jpsGraphicsView()
{
    qDebug("Enter jpsGraphicsView::~jpsGraphicsView");
    // When exit the programm, marked lines and landmarks are already deleted by ~datamanger,
    // Here just delete the contents in the view.
    removeContents();
    qDebug("Leave jpsGraphicsView::~jpsGraphicsView");
}

QGraphicsScene *jpsGraphicsView::GetScene()
{
    qDebug("Enter/Return jpsGraphicsView::GetScene");
    return this->scene();
}

const qreal &jpsGraphicsView::GetTranslationX() const
{
    qDebug("Enter/Return jpsGraphicsView::GetTranslationX");
    return translation_x;
}

const qreal &jpsGraphicsView::GetTranslationY() const
{
    qDebug("Enter/Return qreal &jpsGraphicsView::GetTranslationY");
    return translation_y;
}

const qreal &jpsGraphicsView::GetScaleF() const
{
    qDebug("Enter/Return jpsGraphicsView::GetScaleF");
    return gl_scale_f;
}

void jpsGraphicsView::SetDatamanager(jpsDatamanager *datamanager)
{
    qDebug("Enter/Leave jpsGraphicsView::SetDatamanager");
    _datamanager=datamanager;
}


void jpsGraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mouseMoveEvent(mouseEvent);

    switch (drawingMode){
        case SelectMode:
            break;
        case SourceMode:
            if(currentSource != nullptr)
            {

                currentSource->setRect(QRectF(QPointF(currentSource->rect().x(),currentSource->rect().y()),
                        QPointF(translated_pos.x(),translated_pos.y())));
            }
            break;
        case GoalMode:
            if(currentGoal != nullptr)
            {
                currentGoal->setRect(QRectF(QPointF(currentGoal->rect().x(),currentGoal->rect().y()),
                        QPointF(translated_pos.x(),translated_pos.y())));
            }
            break;
        case LandmarkMode:
            break;
        default:
            // draw wall, door, exit, HLine
            if (current_line!=nullptr)
            {
                current_line->setLine(current_line->line().x1(),current_line->line().y1(),translated_pos.x(),translated_pos.y());
                if (current_line->line().isNull())
                    current_line->setVisible(false);
                else
                    current_line->setVisible(true);

                //}
                //As line length has changed
                emit LineLengthChanged();
            }
    }

    if (current_rect!=nullptr)
    {
       delete current_rect;
       current_rect=nullptr;
    }

    // Pass last positon of mouse to old_ps
    QPointF old_pos=pos;

    // Get position of mouse in scene coordinate
    pos = mapToScene(mouseEvent->pos());

    if (anglesnap)
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

    // If the view was moving, minus the translation of view
    translated_pos.setX(pos.x()-translation_x);
    translated_pos.setY(pos.y()-translation_y);

    if (_gridmode)
        use_gridmode();

    if (objectsnap)
    {
        if(start_endpoint_snap)
        {
            catch_start_endpoints();
        }

        if(intersectionspoint_snap)
        {
            catch_intersections_point();
        }

        if(centerpoint_snap)
        {
            catch_center_point();
        }

        if(linepoint_snap)
        {
            catch_line_point();
        }

        //VLine
        if (point_tracked && (drawingMode==WallMode || drawingMode==TransitionMode))
        {
//            SetVLine();
        }
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
    qDebug("Enter jpsGraphicsView::mousePressEvent");
    if (mouseEvent->button() == Qt::LeftButton)
    {
        switch (drawingMode){
            case LandmarkMode:
                drawLandmark();
                break;
            case SourceMode:
                drawSource();
                break;
            case GoalMode:
                drawGoal();
                break;
            case MeasureMode:
                drawMeasureLengthLine();
                break;
            case SelectMode:
                if (_statDefConnections==1)
                {
                    emit DefConnection1Completed();
                    break;
                }
                else if (_currentTrackedPoint!=nullptr && line_tracked==1 && _statCopy==0)
                {
                    EditLine(_currentTrackedPoint);
                    _currentTrackedPoint=nullptr;
                    line_tracked=-1;
                    break;
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
                    break;
                }
                else
                {
                    //Select_mode
                    currentSelectRect=scene()->addRect(translated_pos.x(),translated_pos.y(),0,0,QPen(Qt::blue,0));
                    currentSelectRect->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
                    leftbutton_hold=true;
                    break;
                }
            default:
                if (_statLineEdit)
                {
                    // Edit line
                    for (jpsLineItem* line:line_vector)
                    {
                        locate_intersection(marked_lines.first(),line);
                    }
                    current_line=nullptr;
                    _statLineEdit=false;
                    line_tracked=1;
                    emit no_drawing();
                    break;
                }
                else
                {
                    // Draw wall, transition, hline, track
                    drawLine();
                    break;
                }
        }
    }
    else if (mouseEvent->button()==Qt::MidButton)
    {
        midbutton_hold=true;
    }
    else if (mouseEvent->button()==Qt::RightButton)
    {
        disable_drawing();
        emit no_drawing();
    }

    update();
    qDebug("Leave jpsGraphicsView::mousePressEvent");
}

void jpsGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug("Enter jpsGraphicsView::mouseDoubleClickEvent");
    if (event->button()==Qt::LeftButton)
    {   /// To avoid deleting of edited line
        if (line_tracked==1)
        {
            current_line=nullptr;
        }
        emit no_drawing();
    }
    qDebug("Leave jpsGraphicsView::mouseDoubleClickEvent");
}

void jpsGraphicsView::unmark_all_lines()
{
    qDebug("Enter jpsGraphicsView::unmark_all_lines");
    QPen pen = QPen(Qt::black,2);
    pen.setCosmetic(true);

    for (int i=0; i<marked_lines.size();i++)
    {
        if(marked_lines[i] != nullptr)
        {
            pen.setColor(QColor(marked_lines[i]->get_defaultColor()));
            marked_lines[i]->get_line()->setPen(pen);
        }
    }
    marked_lines.clear();
    qDebug("Leave jpsGraphicsView::unmark_all_lines");
}

void jpsGraphicsView::drawLandmark()
{
    qDebug("Enter jpsGraphicsView::drawLandmark");
    QPixmap pixmap("../jupedsim/forms/statue.jpg");

    QGraphicsPixmapItem* pixmapItem = this->scene()->addPixmap(pixmap);
    pixmapItem->setScale(0.002);
    pixmapItem->setTransform(QTransform::fromTranslate(this->translated_pos.x()-pixmap.width()/1000.,this->translated_pos.y()
                          +pixmap.height()/1000.));
    pixmapItem->setTransform(QTransform::fromScale(1,-1),true);
    pixmapItem->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    QString name="LandmarkMode"+QString::number(_datamanager->GetLandmarkCounter());
    jpsLandmark* landmark = new jpsLandmark(pixmapItem,name,pixmapItem->scenePos());
    //text immediately under the pixmap
    QGraphicsTextItem* text = this->scene()->addText(name);
    text->setPos(QPointF(landmark->GetPos().x(),
                         landmark->GetPos().y()+0.2));// landmark->GetPos().x()+translation_x,landmark->GetPos().y()+translation_y);
    //text->setScale(gl_scale_f);

    text->setData(0,0.01);
    text->setTransform(QTransform::fromScale(0.01,-0.01),true);

    landmark->SetPixMapText(text);

    _datamanager->new_landmark(landmark);
    qDebug("Leave jpsGraphicsView::drawLandmark");

}

void jpsGraphicsView::drawLandmark(const QPointF &pos)
{
    qDebug("Enter jpsGraphicsView::drawLandmark");
    QPixmap pixmap("../jupedsim/forms/statue.jpg");

    QGraphicsPixmapItem* pixmapItem = this->scene()->addPixmap(pixmap);
    pixmapItem->setScale(0.002);
    pixmapItem->setTransform(QTransform::fromTranslate(pos.x()-pixmap.width()/1000.,pos.y()
                          +pixmap.height()/1000.));
    pixmapItem->setTransform(QTransform::fromScale(1,-1),true);
    pixmapItem->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    QString name="LandmarkMode"+QString::number(_datamanager->GetLandmarkCounter());
    jpsLandmark* landmark = new jpsLandmark(pixmapItem,name,pos);
    //text immediately under the pixmap
    QGraphicsTextItem* text = this->scene()->addText(name);
    text->setPos(QPointF(landmark->GetPos().x(),
                         landmark->GetPos().y()+0.2));// landmark->GetPos().x()+translation_x,landmark->GetPos().y()+translation_y);
    //text->setScale(gl_scale_f);

    text->setTransform(QTransform::fromTranslate(translation_x,-translation_y), true);
    text->setData(0,0.01);
    text->setTransform(QTransform::fromScale(0.01,-0.01),true);


    landmark->SetPixMapText(text);

    _datamanager->new_landmark(landmark);
    qDebug("Leave jpsGraphicsView::drawLandmark");
}


void jpsGraphicsView::unmarkLandmark()
{
    qDebug("Enter jpsGraphicsView::unmarkLandmark");
    if (markedLandmark!=nullptr)
    {
        delete currentLandmarkRect;
        currentLandmarkRect=nullptr;
        markedLandmark=nullptr;
    }
    qDebug("Leave jpsGraphicsView::unmarkLandmark");
}



QGraphicsRectItem *jpsGraphicsView::GetCurrentSelectRect()
{
    qDebug("Enter/Return jpsGraphicsView::GetCurrentSelectRect");
    return currentSelectRect;
}

void jpsGraphicsView::SetStatDefConnections(const int &stat)
{
    qDebug("Enter/Leave jpsGraphicsView::SetStatDefConnections");
    _statDefConnections=stat;
}


void jpsGraphicsView::ShowYAHPointer(const QPointF &pos, const qreal &dir)
{
    qDebug("Enter jpsGraphicsView::ShowYAHPointer");
    for (QGraphicsLineItem* lineItem:_yahPointer)
    {
        delete lineItem;
    }
    _yahPointer.clear();

    _yahPointer.push_back(this->scene()->addLine(pos.x(),pos.y(),pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),QPen
    (Qt::blue,0)));
    _yahPointer.push_back(this->scene()->addLine(pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),pos.x()+0.2*std::cos(dir+M_PI/4.0),pos.y()+0.2*std::sin(dir+M_PI/4.0),QPen(Qt::blue,0)));
    _yahPointer.push_back(this->scene()->addLine(pos.x()+0.5*std::cos(dir),pos.y()+0.5*std::sin(dir),pos.x()+0.2*std::cos(dir-M_PI/4.0),pos.y()+0.2*std::sin(dir-M_PI/4.0),QPen(Qt::blue,0)));

    for (QGraphicsLineItem* lineItem:_yahPointer)
    {
        lineItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    }
    qDebug("Leave jpsGraphicsView::ShowYAHPointer");

}


void jpsGraphicsView::ShowConnections(QList<ptrConnection> cons)
{
    qDebug("Enter jpsGraphicsView::ShowConnections");
    ClearConnections();
    for (ptrConnection con:cons)
    {
        QGraphicsLineItem* line = this->scene()->addLine(QLineF(con->GetLandmarks().first->GetPos(),con->GetLandmarks().second->GetPos()),QPen(Qt::blue,0));
        _connections.push_back(line);


        line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);


    }
    qDebug("Leave jpsGraphicsView::ShowConnections");
}

void jpsGraphicsView::ClearConnections()
{
    qDebug("Enter jpsGraphicsView::ClearConnections");
    for (QGraphicsLineItem* line:_connections)
    {
        delete line;
    }
    _connections.clear();
    qDebug("Leave jpsGraphicsView::ClearConnections");
}




void jpsGraphicsView::ActivateLineGrid()
{
    qDebug("Enter jpsGraphicsView::ActivateLineGrid");
    this->SetGrid("Line");
    qDebug("Leave jpsGraphicsView::ActivateLineGrid");
}

void jpsGraphicsView::ActivatePointGrid()
{
    qDebug("Enter jpsGraphicsView::ActivatePointGrid");
    this->SetGrid("Point");
    qDebug("Leave jpsGraphicsView::ActivatePointGrid");

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
    qDebug("Enter jpsGraphicsView::mouseReleaseEvent");
    if (event->button() == Qt::LeftButton)
    {
        // Select lines that are located within the rectangle
        if (drawingMode == SelectMode)
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
                    //LandmarkMode position definition
                    emit PositionDefCompleted();
                    _posDef=false;
                }
                else if (_regionDef)
                {
                    emit RegionDefCompleted();
                    _regionDef=false;
                }
                else if(statzoomwindows)
                {
                    this->fitInView(currentSelectRect->rect(),Qt::KeepAspectRatio);
                    gl_scale_f=1/this->transform().m11();

                    //translations
                    QPointF old_pos;
                    old_pos.setX(pos.x()+translation_x);
                    old_pos.setY(pos.y()+translation_y);
                    translations(old_pos);

                    statzoomwindows=false;
                }
                else
                {
                    // Select lines by creating a rect with the cursor
                    catch_lines();

                    // unmark LandmarkMode is possible
                    unmarkLandmark();
                    // Look for landmarks with position in the currentSelectRect
                    catch_landmark();
                }
                delete currentSelectRect;
                currentSelectRect = nullptr;
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
    qDebug("Leave jpsGraphicsView::mouseReleaseEvent");

}



const QPointF &jpsGraphicsView::return_Pos() const
{
    return translated_pos;
}

void jpsGraphicsView::delete_all(bool final)
{
    qDebug("Enter jpsGraphicsView::delete_all");
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

    removeContents();

    update();
    qDebug("Leave jpsGraphicsView::delete_all");
}

bool jpsGraphicsView::use_anglesnap(QGraphicsLineItem* currentline, int accuracy)
{
    qDebug("Enter jpsGraphicsView::use_anglesnap");
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
    qDebug("Leave jpsGraphicsView::use_anglesnap");
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

        current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
        point_tracked=true;
        _currentTrackedPoint= &translated_pos;


    }
    else
        point_tracked=false;
}

void jpsGraphicsView::catch_points()
{
    qDebug("Enter jpsGraphicsView::catch_points");
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
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
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
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
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
            current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
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
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));

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
    qDebug("Leave jpsGraphicsView::catch_points");
    return;
}

void jpsGraphicsView::catch_start_endpoints()
{
    qDebug("Enter jpsGraphicsView::catch_start_endpoints");
    //Searching for startpoints of all lines near the current cursor position
    for (int i=0; i<line_vector.size(); ++i){

        // range chosen: 10 (-5:5) (has to be changed soon)
        if (line_vector[i]->get_line()->line().x1()>=(translated_pos.x()-catch_radius)
                && line_vector[i]->get_line()->line().x1()<=(translated_pos.x()+catch_radius)
                && line_vector[i]->get_line()->line().y1()>=(translated_pos.y()-catch_radius)
                && line_vector[i]->get_line()->line().y1()<=(translated_pos.y()+catch_radius)){
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
//            point_tracked=true;
//            _currentTrackedPoint= &translated_pos;
            //QPen pen;
            //pen.setColor('red');
            if (current_rect==nullptr)
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                            translated_pos.y()+translation_y-10*gl_scale_f,
                                            20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
            // if a point was tracked there is no need to look for further points ( only one point can be tracked)

            return;
        }

        //Searching for endpoints of all lines near the current cursor position
        else if (line_vector[i]->get_line()->line().x2()>=(translated_pos.x()-catch_radius)
                 && line_vector[i]->get_line()->line().x2()<=(translated_pos.x()+catch_radius)
                 && line_vector[i]->get_line()->line().y2()>=(translated_pos.y()-catch_radius)
                 && line_vector[i]->get_line()->line().y2()<=(translated_pos.y()+catch_radius)){
            // see above

            //to avoid the tracking of the coords of an edited line
            if (line_vector[i]->get_line()==current_line)
            {
                continue;
            }


            translated_pos.setX(line_vector[i]->get_line()->line().x2());
            translated_pos.setY(line_vector[i]->get_line()->line().y2());
            //cursor.setPos(mapToGlobal(QPoint(translate_back_x(line_vector[i].x2()),translate_back_y(line_vector[i].y2()))));
//            point_tracked=true;
            if (current_rect==nullptr)
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,translated_pos.y()+translation_y-10*gl_scale_f,20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
//            _currentTrackedPoint= &translated_pos;
            return;
        }
    }

    point_tracked=false;
    qDebug("Leave jpsGraphicsView::catch_start_endpoints");
    return;
}

void jpsGraphicsView::catch_intersections_point()
{
    qDebug("Enter jpsGraphicsView::catch_intersections_point");
    // see above
    for (int j=0; j<intersect_point_vector.size(); j++)
    {
        if (intersect_point_vector[j]->x()>=(translated_pos.x()-catch_radius)
                && intersect_point_vector[j]->x()<=(translated_pos.x()+catch_radius)
                && intersect_point_vector[j]->y()>=(translated_pos.y()-catch_radius)
                && intersect_point_vector[j]->y()<=(translated_pos.y()+catch_radius))
        {
            translated_pos.setX(intersect_point_vector[j]->x());
            translated_pos.setY(intersect_point_vector[j]->y());
            if (current_rect==nullptr)
            current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                        translated_pos.y()+translation_y-10*gl_scale_f,
                                        20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
//                point_tracked=true;
            return;
        }
    }


    point_tracked=false;
    qDebug("Leave jpsGraphicsView::catch_intersections_point");
    return;
}

void jpsGraphicsView::catch_center_point()
{
    qDebug("Enter jpsGraphicsView::catch_intersections_point");
    for(int i=0; i<line_vector.size(); i++)
    {
        QPointF center = line_vector[i]->get_line()->line().center();
        if(center.x()>=(translated_pos.x()-catch_radius)
                && center.x()<=(translated_pos.x()+catch_radius)
                && center.y()>=(translated_pos.y()-catch_radius)
                && center.y()<=(translated_pos.y()+catch_radius))
        {
            translated_pos.setX(center.x());
            translated_pos.setY(center.y());
            if (current_rect==nullptr)
            {
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                            translated_pos.y()+translation_y-10*gl_scale_f,
                                            20*gl_scale_f,20*gl_scale_f,QPen(Qt::green,0));

            }
        }

    }

    point_tracked=false;
    qDebug("Leave jpsGraphicsView::catch_intersections_point");
    return;
}

void jpsGraphicsView::catch_line_point()
{
    qDebug("Enter jpsGraphicsView::catch_line_point");
    for(int i=0; i<marked_lines.size(); i++)
    {
        QPointF point = getNearstPointOnLine(marked_lines[i]);

        if(point.x()>=(translated_pos.x()-catch_radius)
                && point.x()<=(translated_pos.x()+catch_radius)
                && point.y()>=(translated_pos.y()-catch_radius)
                && point.y()<=(translated_pos.y()+catch_radius))
        {
            translated_pos.setX(point.x());
            translated_pos.setY(point.y());
            if (current_rect==nullptr)
            {
                current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                            translated_pos.y()+translation_y-10*gl_scale_f,
                                            20*gl_scale_f,20*gl_scale_f,QPen(Qt::blue,0));

            }
        }

    }

    point_tracked=false;
    qDebug("Leave jpsGraphicsView::catch_line_point");
    return;
}

void jpsGraphicsView::catch_lines()
{
    qDebug("Enter jpsGraphicsView::catch_lines");
    // Catch wall, transition, track, hline
    // If current rect was build up moving the cursor to the left ->
    // Whole line has to be within the rect to select the line
    line_tracked=-1;

    if (currentSelectRect->rect().width()<0)
    {
        for (auto &item:line_vector)
        {
            if (currentSelectRect->contains(item->get_line()->line().p1())
                && currentSelectRect->contains(item->get_line()->line().p2())
                && item->get_defaultColor()!="white")
            {
                markLine(item);
            }
        }

        // select transitions
        for (auto item:_datamanager->getTransitionList())
        {
            if (currentSelectRect->contains(item->get_cLine()->get_line()->line().p1())
                && currentSelectRect->contains(item->get_cLine()->get_line()->line().p2()))
            {
                markLine(item->get_cLine());
            }
        }

    }
        // Ff current rect was build up moving the cursor to the right ->
        // Throwing the select rect only over a part of a line is sufficent to select it
    else if (currentSelectRect->rect().width()>0)
    {
        for (auto &item : line_vector)
        {
            if (currentSelectRect->collidesWithItem(item->get_line()) && item->get_defaultColor()!="white")
            {
                markLine(item);
                line_tracked=1;
            }
        }

        // select transitions
        for (auto item:_datamanager->getTransitionList())
        {
            if (currentSelectRect->collidesWithItem(item->get_cLine()->get_line()))
            {
                markLine(item->get_cLine());
                line_tracked=1;
            }
        }
    }
    qDebug("Leave jpsGraphicsView::catch_lines");
}

void jpsGraphicsView::drawLine()
{
    qDebug("Enter jpsGraphicsView::drawLine");
    if (current_line==nullptr) // if the mouse was pressed first of two times
    {
        //Determining first point of line

        // all two points of the line are inited with the cursorcoordinates
        current_line = this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setVisible(false); // For now current line is only a point
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);

        emit set_focus_textedit();
    }

    // if the mouse was pressed secondly of two times
    else
    {
        auto* lineItem= new jpsLineItem(current_line);
        lineItem->set_id(id_counter);
        id_counter++;

        // if there is already a drawn line
        if (!line_vector.isEmpty())
        {
            // Searching for intersection of the current line with all already drawn lines
            for (int i=0; i<line_vector.size(); i++)
            {
                // locate possible intersection between lines
                // if there are some, intersectionPoint will be added to container
                locate_intersection(lineItem,line_vector[i]);
            }
        }

        switch (drawingMode){
            case WallMode:
                lineItem->setWall();
                break;
            case HlineMode:
                lineItem->setHline();
                break;
            case TransitionMode:
                lineItem->setTransition();
                // Transion won't be added in any zone, so should be created here
                _datamanager->newTransition(lineItem);
                emit transitonsChanged();
                break;
            case TrackMode:
                lineItem->setTrack();
                break;
            default:
                break;
        }

        // line_vector saves all lineItem, no matter if it's assigned or not
        line_vector.push_back(lineItem);

        //reset pointer
        current_line = nullptr;

        //Undo
        RecordUndoLineAction("LineAdded", lineItem->getType(),lineItem->get_id(),lineItem->get_line()->line());
    }

    //Vline
    if (_currentVLine!=nullptr)
    {
        delete _currentVLine;
        _currentVLine=nullptr;
    }
    qDebug("Leave jpsGraphicsView::drawLine");

}

void jpsGraphicsView::markLine(jpsLineItem *mline)
{
    qDebug("Enter jpsGraphicsView::markLine");
    if (mline == nullptr)
        return;

    if (!marked_lines.contains(mline))
    {
        QPen pen = QPen(Qt::red,4);
        pen.setCosmetic(true);
        mline->get_line()->setPen(pen);
        marked_lines.append(mline);
        line_tracked=1;
    }
    else
    {
        QPen pen = QPen(Qt::black,2);
        pen.setCosmetic(true);
        mline->get_line()->setPen(pen);
        marked_lines.removeOne(mline);
    }
    qDebug("Leave jpsGraphicsView::markLine");
}


void jpsGraphicsView::disable_drawing()
{
    qDebug("Enter jpsGraphicsView::disable_drawing");
    _statCopy=0;

    drawingMode = SelectMode;

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
    if (currentSource != nullptr)
    {
        delete currentSource;
        currentSource = nullptr;
    }
    if (currentGoal != nullptr)
    {
        delete currentGoal;
        currentGoal = nullptr;
    }
    qDebug("Leave jpsGraphicsView::disable_drawing");
}

jpsLineItem* jpsGraphicsView::addLineItem(const qreal &x1,const qreal &y1,const qreal &x2,const qreal &y2,const QString &type)
{
    qDebug("Enter jpsGraphicsView::addLineItem");
    /*
     * add Lineitem when prase a XML file
     */
    QPen pen = QPen(Qt::black,2);

    pen.setCosmetic(true);

    current_line=this->scene()->addLine(x1,y1,x2,y2,pen);
    current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);

    auto* newLine = new jpsLineItem(current_line);
    newLine->set_id(id_counter);
    id_counter++;

    if (type=="transition")
    {
        newLine->setTransition();
        // TransitionMode isn't a normal JPSLineItem, rather JPSTransition class
        _datamanager->newTransition(newLine);
        emit transitonsChanged();
    }
    else if (type=="hline")
    {
        newLine->setHline();
    }
    else if (type=="track")
    {
        newLine->setTrack();
    }
    else if (type=="wall")
    {
        newLine->setWall();
    }
    else
    {
        newLine->setUndefined();
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

    qDebug("Leave jpsGraphicsView::addLineItem");
    return newLine;
}

jpsLineItem *jpsGraphicsView::addLineItem(const QLineF &line, const QString &type)
{
    qDebug("Enter/return jpsGraphicsView::addLineItem");
    return addLineItem(line.p1().x(),line.p1().y(),line.p2().x(),line.p2().y(),type);
}

void jpsGraphicsView::addSourceItem(const QList<QPointF> &points)
{
    qDebug("Enter jpsGraphicsView::addSourceItem");
    QGraphicsRectItem *rectItem = this->scene()->addRect(QRectF(points.first(), points.last()),currentPen);
    auto *sourceItem = new JPSSource(rectItem);
    sourceItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    this->scene()->addItem(sourceItem);
    this->scene()->removeItem(rectItem);

    this->scene()->update();

    emit sourcesChanged();
    qDebug("Leave jpsGraphicsView::addSourceItem");
}

void jpsGraphicsView::addGoalItem(const QList<QPointF> &points)
{
    qDebug("Enter jpsGraphicsView::addGoalItem");
    QGraphicsRectItem *rectItem = this->scene()->addRect(QRectF(points.first(), points.last()),currentPen);
    auto *goalItem = new JPSGoal(rectItem);
    goalItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    this->scene()->addItem(goalItem);
    this->scene()->removeItem(rectItem);

    this->scene()->update();

    emit goalsChanged();
    qDebug("Leave jpsGraphicsView::addGoalItem");
}

void jpsGraphicsView::locate_intersection(jpsLineItem *item1, jpsLineItem *item2)
{
    qDebug("Enter jpsGraphicsView::locate_intersection");
    // this pointer is necessary due to the architecture of the method 'intersect'
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
    qDebug("Leave jpsGraphicsView::locate_intersection");
}

void jpsGraphicsView::SetVLine()
{
    qDebug("Enter jpsGraphicsView::SetVLine");
    if (_currentVLine==nullptr)
    {
        if (current_line==nullptr)
        {
            _currentVLine=this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),
                                         QPen(Qt::black,0,Qt::DashLine));
            _currentVLine->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        }
        else if (current_line->line().p1()!=translated_pos)
        {
            _currentVLine=this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),
                                         QPen(Qt::black,0,Qt::DashLine));
            _currentVLine->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        }
    }
    qDebug("Leave jpsGraphicsView::SetVLine");
}

void jpsGraphicsView::EditLine(QPointF* point)
{
    qDebug("Enter jpsGraphicsView::EditLine");
    if (marked_lines.size()==1)
    {

        delete current_rect;
        current_rect=nullptr;

        point_tracked=false;

        if (marked_lines.first()->get_line()->line().p1()==*point)
        {
            RecordUndoLineAction("LineEdited", marked_lines.first()->getType(),marked_lines.first()->get_id(),marked_lines.first()->get_line()->line());
            current_line=marked_lines[0]->get_line();
            marked_lines[0]->get_line()->setLine(marked_lines[0]->get_line()->line().x2(),marked_lines[0]->get_line()->line().y2(),translated_pos.x(),translated_pos.y());
            //current_line=this->scene()->addLine(marked_lines[0]->get_line()->line().p2().x(),
             //       marked_lines[0]->get_line()->line().p2().y(),translated_pos.x(),translated_pos.y(),QPen(Qt::red,0));
            //current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
            emit set_focus_textedit();
        }

        else if (marked_lines.first()->get_line()->line().p2()==*point)
        {
            RecordUndoLineAction("LineEdited", marked_lines.first()->getType(),marked_lines.first()->get_id(),marked_lines.first()->get_line()->line());
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
    qDebug("Leave jpsGraphicsView::EditLine");
}

qreal jpsGraphicsView::ReturnLineLength()
{
    return current_line->line().length();
}

bool jpsGraphicsView::is_hide_roomCaption(QString name)
{
    qDebug("Enter jpsGraphicsView::is_hide_roomCaption");
    for (int i=0; i<caption_list.size(); i++)
    {
        if (caption_list[i]->toPlainText()==name)
        {
            return false;
        }
    }
    qDebug("Leave jpsGraphicsView::is_hide_roomCaption");
    return true;
}



bool jpsGraphicsView::show_hide_roomCaption(QString name, qreal x, qreal y)
{
    qDebug("Enter jpsGraphicsView::show_hide_roomCaption");
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
    current_caption=this->scene()->addText(name);

    current_caption->setX(x+translation_x);
    current_caption->setY(y+translation_y);

    //Since the scene itself is mirrored (scale=1, scale=-1):

    // adjust captionsize depending on zoomgrade
    current_caption->setData(0,gl_scale_f);
    current_caption->setTransform(QTransform::fromScale(gl_scale_f,-gl_scale_f),true);

    //current_caption->adjustSize();
    caption_list.push_back(current_caption);
    current_caption=nullptr;

    qDebug("Leave jpsGraphicsView::show_hide_roomCaption");
    return true;
}

void jpsGraphicsView::RecordUndoLineAction(const QString& name, const QString& type, const int& itemID, const QLineF &oldLine)
{
    qDebug("Enter jpsGraphicsView::RecordUndoLineAction");
    _undoStack.PushNewAction(LineAction(name,type,itemID,oldLine));
    qDebug("Leave jpsGraphicsView::RecordUndoLineAction");
}

void jpsGraphicsView::RecordRedoLineAction(const QString &name, const QString &type, const int& itemID, const QLineF &oldLine)
{
    qDebug("Enter jpsGraphicsView::RecordRedoLineAction");
    _redoStack.PushNewAction(LineAction(name,type,itemID,oldLine));
    qDebug("Leave jpsGraphicsView::RecordRedoLineAction");
}

void jpsGraphicsView::UndoLineEdit(const int& lineID, const QLineF& old_line)
{
    qDebug("Enter jpsGraphicsView::UndoLineEdit");
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_id()==lineID)
        {
           RecordRedoLineAction("LineEdited", lineItem->getType(),lineItem->get_id(),lineItem->get_line()->line());
           lineItem->get_line()->setLine(old_line);
           break;
        }
    }
    qDebug("Leave jpsGraphicsView::UndoLineEdit");
}

void jpsGraphicsView::RedoLineEdit(const int &lineID, const QLineF &old_line)
{
    qDebug("Enter jpsGraphicsView::RedoLineEdit");
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_id()==lineID)
        {
           RecordUndoLineAction("LineEdited", lineItem->getType(),lineItem->get_id(),lineItem->get_line()->line());
           lineItem->get_line()->setLine(old_line);
           break;
        }
    }
    qDebug("Leave jpsGraphicsView::RedoLineEdit");
}

void jpsGraphicsView::Undo()
{
    qDebug("Enter jpsGraphicsView::Undo");
    if (!_undoStack.IsEmpty())
    {
        emit no_drawing();

        const LineAction recentAction = _undoStack.GetRecentAction();

        if (recentAction.GetName()=="LineDeleted")
        {
            addLineItem(recentAction.GetOldLine().p1().x(),recentAction.GetOldLine().p1().y(),recentAction.GetOldLine().p2().x(),
                        recentAction.GetOldLine().p2().y(),recentAction.GetType());

            RecordRedoLineAction("LineAdded",recentAction.GetType(),id_counter-1,QLineF(0,0,0,0));
        }
        else if (recentAction.GetName()=="LineAdded")
        {
            RecordRedoLineAction("LineDeleted", line_vector.back()->getType(),line_vector.back()->get_id(),
                                 line_vector.back()->get_line()->line());
            RemoveLineItem(line_vector.back());
        }
        else if (recentAction.GetName()=="LineEdited")
        {
            UndoLineEdit(recentAction.GetItemID(),recentAction.GetOldLine());
        }
    }
    qDebug("Leave jpsGraphicsView::Undo");

}

void jpsGraphicsView::Redo()
{
    qDebug("Enter jpsGraphicsView::Redo");
    if (!_redoStack.IsEmpty())
    {
        emit no_drawing();

        const LineAction recentAction = _redoStack.GetRecentAction();

        if (recentAction.GetName()=="LineDeleted")
        {
            addLineItem(recentAction.GetOldLine().p1().x(),recentAction.GetOldLine().p1().y(),recentAction.GetOldLine().p2().x(),
                        recentAction.GetOldLine().p2().y(),recentAction.GetType());

            RecordUndoLineAction("LineAdded",recentAction.GetType(),id_counter-1,QLineF(0,0,0,0));
        }

        else if (recentAction.GetName()=="LineAdded")
        {
            RecordUndoLineAction("LineDeleted", line_vector.back()->getType(),line_vector.back()->get_id(),
                                 line_vector.back()->get_line()->line());
            RemoveLineItem(line_vector.back());

        }
        else if (recentAction.GetName()=="LineEdited")
        {
            RedoLineEdit(recentAction.GetItemID(),recentAction.GetOldLine());

        }
    }
    qDebug("Leave jpsGraphicsView::Redo");
}

void jpsGraphicsView::line_collision()
{
    qDebug("Enter jpsGraphicsView::line_collision");
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
                    current_rect=this->scene()->addRect(translated_pos.x()+translation_x-10*gl_scale_f,
                                                        translated_pos.y()+translation_y-10*gl_scale_f,
                                                        20*gl_scale_f,20*gl_scale_f,QPen(Qt::red,0));
                }
                break;


            }

        }
    }
    qDebug("Leave jpsGraphicsView::line_collision");
}


void jpsGraphicsView::zoom(int delta)
{
    qDebug("Enter jpsGraphicsView::zoom");
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
        this->ChangeGridSize(this->CalcGridSize());
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
        this->ChangeGridSize(this->CalcGridSize());
        //create_grid();
    }

    //set origin back to the scene
    ShowOrigin();
    qDebug("Leave jpsGraphicsView::zoom");
}

void jpsGraphicsView::translations(QPointF old_pos)
{
    qDebug("Enter jpsGraphicsView::translations");

    // Range of translation
    translation_x+=pos.x()-old_pos.x();
    translation_y+=pos.y()-old_pos.y();

    // Transform the background grid
    this->ChangeTranslation(translation_x,translation_y);

    // Transform QGraphicsitem when view moving
    if (current_line!=nullptr)
    {
        current_line->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    if (currentGoal!= nullptr)
    {
        currentGoal->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    if (currentSource != nullptr)
    {
        currentSource->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    if (_currentVLine!=nullptr)
    {
        //current_line->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        _currentVLine->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    // Transform saved elements
    for (int i=0; i<line_vector.size(); ++i)
    {
        //line_vector[i]->get_line()->translate(pos.x()-old_pos.x(),pos.y()-old_pos.y());
        // Avoid double sized translation of edited line
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

    for (JPSSource *source : getSources())
    {
        if(currentSource == source->getRectItem())
        {
            continue;
        }
        source->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    for (JPSGoal *goal : getGoals())
    {
        if(currentGoal == goal->getRectItem())
        {
            continue;
        }
        goal->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
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

    // Background
    if(background != nullptr)
    {
        background->setTransform(QTransform::fromTranslate(pos.x()-old_pos.x(),pos.y()-old_pos.y()), true);
    }

    qDebug("Leave jpsGraphicsView::translations");
}

void jpsGraphicsView::AutoZoom()
{
    qDebug("Enter jpsGraphicsView::translations");
    if(line_vector.size()==0)
        return;

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
    qDebug("Leave jpsGraphicsView::translations");
}

qreal jpsGraphicsView::CalcGridSize()
{
    qDebug("Enter jpsGraphicsView::CalcGridSize");
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
    qDebug("Leave jpsGraphicsView::CalcGridSize");
}

void jpsGraphicsView::ShowOrigin()
{
    qDebug("Enter jpsGraphicsView::CalcGridSize");
    if (_origin.isEmpty())
    {
        //Scene->DrawOrigin();
        _origin.push_back(this->scene()->addLine(0,0,0,0+gl_scale_f*100,QPen(Qt::red,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0,0+gl_scale_f*100,0-gl_scale_f*10,0+gl_scale_f*100-gl_scale_f*10,QPen(Qt::red,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0,0+gl_scale_f*100,0+gl_scale_f*10,0+gl_scale_f*100-gl_scale_f*10,QPen(Qt::red,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0,0,0+gl_scale_f*100,0,QPen(Qt::red,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0+gl_scale_f*100,0,gl_scale_f*100-gl_scale_f*10,-gl_scale_f*10,QPen(Qt::red,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0+gl_scale_f*100,0,gl_scale_f*100-gl_scale_f*10,+gl_scale_f*10,QPen(Qt::red,gl_scale_f*2)));
        //Y
        _origin.push_back(this->scene()->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*5,gl_scale_f*100-gl_scale_f*45,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*15,gl_scale_f*100-gl_scale_f*45,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*50,0-gl_scale_f*10,gl_scale_f*100-gl_scale_f*60,QPen(Qt::black,gl_scale_f*2)));
        //X
        _origin.push_back(this->scene()->addLine(0+gl_scale_f*40,0-gl_scale_f*15,gl_scale_f*50,-gl_scale_f*5,QPen(Qt::black,gl_scale_f*2)));
        _origin.push_back(this->scene()->addLine(0+gl_scale_f*40,0-gl_scale_f*5,gl_scale_f*50,-gl_scale_f*15,QPen(Qt::black,gl_scale_f*2)));

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
    qDebug("Leave jpsGraphicsView::CalcGridSize");

}

void jpsGraphicsView::StatPositionDef()
{
    qDebug("Enter/Return jpsGraphicsView::StatPositionDef");
    _posDef=!_posDef;
}

void jpsGraphicsView::ChangeRegionStatDef()
{
    qDebug("Enter jpsGraphicsView::ChangeRegionStatDef");
    _regionDef=!_regionDef;
    qDebug("Leave jpsGraphicsView::ChangeRegionStatDef");
}



qreal jpsGraphicsView::calc_d_point(const QLineF &line,const qreal &x, const qreal &y)

{
    qDebug("Enter jpsGraphicsView::calc_d_point");
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
    qDebug("Leave jpsGraphicsView::calc_d_point");

}

// Delete all marked lines
void jpsGraphicsView::delete_marked_lines()
{
    qDebug("Enter jpsGraphicsView::delete_marked_lines");
    //TODO: Orgnize the flow for delete marked lines
    if (line_tracked!=-1)
    {
        for(int i=0; i<marked_lines.size(); ++i)
        {
            RecordUndoLineAction("LineDeleted", marked_lines[i]->getType(),marked_lines[i]->get_id(),marked_lines[i]->get_line()->line());
            RemoveIntersections(marked_lines[i]);

            delete marked_lines[i]->get_line(); // Delete in scene

            line_vector.removeOne(marked_lines[i]);
        }

        //intersect_point_vector.clear();
        line_tracked=-1;

        marked_lines.clear();

        update();
    }
    qDebug("Leave jpsGraphicsView::delete_marked_lines");
}

void jpsGraphicsView::RemoveLineItem(jpsLineItem *mline)
{
    qDebug("Enter jpsGraphicsView::RemoveLineItem");
    RemoveIntersections(mline);
    line_vector.removeOne(mline);
    delete mline->get_line();
    delete mline;
    emit lines_deleted();
    qDebug("Leave jpsGraphicsView::RemoveLineItem");

}

void jpsGraphicsView::RemoveLineItem(const QLineF &line)
{
    qDebug("Enter jpsGraphicsView::RemoveLineItem");
    for (jpsLineItem* lineItem:line_vector)
    {
        if (lineItem->get_line()->line()==line)
        {
            unmark_all_lines();
            markLine(lineItem);
            delete_marked_lines();
        }
    }
    qDebug("Leave jpsGraphicsView::RemoveLineItem");

}

void jpsGraphicsView::RemoveIntersections(jpsLineItem *lineItem)
{
    qDebug("Enter jpsGraphicsView::RemoveIntersections");
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
    qDebug("Leave jpsGraphicsView::RemoveIntersections");
}

void jpsGraphicsView::SelectAllLines()
{
    qDebug("Enter jpsGraphicsView::SelectAllLines");
    marked_lines.clear();
    for (jpsLineItem* line:line_vector)
    {
        markLine(line);
    }
    qDebug("Leave jpsGraphicsView::SelectAllLines");
}

void jpsGraphicsView::deleteMarkedLandmark()
{
    qDebug("Enter jpsGraphicsView::deleteMarkedLandmark");
    if (markedLandmark!=nullptr)
    {
        _datamanager->remove_landmark(markedLandmark);
        markedLandmark=nullptr;
        delete currentLandmarkRect;
        currentLandmarkRect=nullptr;
    }
    qDebug("Leave jpsGraphicsView::deleteMarkedLandmark");
}

void jpsGraphicsView::catch_landmark()
{
    qDebug("Enter jpsGraphicsView::catch_landmark");
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
    qDebug("Leave jpsGraphicsView::catch_landmark");
}

void jpsGraphicsView::select_landmark(jpsLandmark* landmark)
{
    qDebug("Enter jpsGraphicsView::select_landmark");
    unmarkLandmark();
    currentLandmarkRect=this->scene()->addRect(
                landmark->GetPixmap()->mapRectToScene(landmark->GetPixmap()->pixmap().rect()),QPen(Qt::red,0));
    markedLandmark=landmark;
    qDebug("Leave jpsGraphicsView::select_landmark");
}

void jpsGraphicsView::take_l_from_lineEdit(const qreal &length)
{
    qDebug("Enter jpsGraphicsView::take_l_from_lineEdit");
    if (current_line!=nullptr)
    {
        QLineF line(current_line->line());
        line.setLength(length);
        current_line->setLine(line);
        jpsLineItem* jpsline = new jpsLineItem(current_line);
        jpsline->set_id(id_counter);
        id_counter++;
//        jpsline->setType(statWall,statDoor,statExit);
        switch (drawingMode){
            case WallMode:
                jpsline->setWall();
                break;
            case TransitionMode:
                jpsline->setTransition();
                break;
            default:
                break;
        }

        line_vector.push_back(jpsline);
        current_line=nullptr;
    }
    else if (_currentVLine!=nullptr)
    {
        QLineF line(_currentVLine->line());
        line.setLength(length);
        _currentVLine->setLine(line);
        //jpsLineItem* jpsline = new jpsLineItem(current_line);
        //jpsline->setType(statWall,statDoor,statExit);
        //line_vector.push_back(jpsline);
        translated_pos.setX(_currentVLine->line().p2().x());
        translated_pos.setY(_currentVLine->line().p2().y());
        current_line = this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        emit set_focus_textedit();
        //translated_pos.setX(_currentVLine->line().p2().x());
        //translated_pos.setY(_currentVLine->line().p2().y());
        delete _currentVLine;
        _currentVLine=nullptr;
    }
    qDebug("Leave jpsGraphicsView::take_l_from_lineEdit");
}

void jpsGraphicsView::take_endpoint_from_xyEdit(const QPointF &endpoint)
{
    qDebug("Enter jpsGraphicsView::take_endpoint_from_xyEdit");
    if (current_line!=nullptr)
    {
        QLineF line(current_line->line());
        line.setP2(endpoint);
        current_line->setLine(line);
        jpsLineItem* jpsline = new jpsLineItem(current_line);
        jpsline->set_id(id_counter);
        id_counter++;
//        jpsline->setType(statWall,statDoor,statExit);
        switch (drawingMode){
            case WallMode:
                jpsline->setWall();
                break;
            case TransitionMode:
                jpsline->setTransition();
                break;
            default:
                break;
        }
        line_vector.push_back(jpsline);
        current_line=nullptr;
    }
    else if (_currentVLine!=nullptr)
    {
        QLineF line(_currentVLine->line());
        line.setP2(endpoint);
        _currentVLine->setLine(line);
        //jpsLineItem* jpsline = new jpsLineItem(current_line);
        //jpsline->setType(statWall,statDoor,statExit);
        //line_vector.push_back(jpsline);
        translated_pos.setX(_currentVLine->line().p2().x());
        translated_pos.setY(_currentVLine->line().p2().y());
        current_line = this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        emit set_focus_textedit();
        //translated_pos.setX(_currentVLine->line().p2().x());
        //translated_pos.setY(_currentVLine->line().p2().y());
        delete _currentVLine;
        _currentVLine=nullptr;
    }
    qDebug("Leave jpsGraphicsView::take_endpoint_from_xyEdit");
}



QList<jpsLineItem *> jpsGraphicsView::get_markedLines()
{
    qDebug("Enter/Return jpsGraphicsView::get_markedLines");
    return marked_lines;
}

QList<jpsLineItem *> jpsGraphicsView::get_line_vector()
{
    qDebug("Enter/Return jpsGraphicsView::get_line_vector");
    return line_vector;
}

void jpsGraphicsView::removeLineFromLine_vector(jpsLineItem *line)
{
    qDebug("Enter jpsGraphicsView::removeLineFromLine_vector");
    if (line_vector.contains(line))
        line_vector.removeOne(line);
    qDebug("Leave jpsGraphicsView::removeLineFromLine_vector");
}

qreal jpsGraphicsView::get_scale_f()
{
    qDebug("Enter/Return jpsGraphicsView::get_scale_f");
    return gl_scale_f;
}


void jpsGraphicsView::change_objectsnap()
{
    qDebug("Enter/Return jpsGraphicsView::change_objectsnap");
    objectsnap=!objectsnap;
}

bool jpsGraphicsView::get_objectsnap()
{
    qDebug("Enter/Return jpsGraphicsView::get_objectsnap");
    return objectsnap;
}

void jpsGraphicsView::change_gridmode()
{
    qDebug("Enter jpsGraphicsView::change_gridmode");
    _gridmode=!_gridmode;
    ChangeGridmode(_gridmode);
    this->scene()->update();
    qDebug("Leave jpsGraphicsView::change_gridmode");
}

void jpsGraphicsView::en_disableWall()
{
    qDebug("Enter jpsGraphicsView::en_disableWall");
    drawingMode = WallMode;

    if(drawingMode != WallMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkGray);
    }
    qDebug("Leave jpsGraphicsView::en_disableWall");
}


bool jpsGraphicsView::statusWall()
{
    qDebug("Enter jpsGraphicsView::statusWall");
    if(drawingMode == WallMode)
    {
        return true;
    } else
        return false;
    qDebug("Leave jpsGraphicsView::statusWall");
}

void jpsGraphicsView::change_stat_anglesnap()
{
    qDebug("Enter jpsGraphicsView::change_stat_anglesnap");
    anglesnap=!anglesnap;
    qDebug("Leave jpsGraphicsView::change_stat_anglesnap");
}

bool jpsGraphicsView::get_stat_anglesnap()
{
    qDebug("Enter/Return jpsGraphicsView::get_stat_anglesnap");
    return anglesnap;
}

void jpsGraphicsView::enableTransition()
{
    qDebug("Enter jpsGraphicsView::enableTransition");
    drawingMode = TransitionMode;

    if(drawingMode != TransitionMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkBlue);
    }
    qDebug("Leave jpsGraphicsView::enableTransition");
}

void jpsGraphicsView::enableTrack()
{
    qDebug("Enter jpsGraphicsView::enableTrack");
    drawingMode = TrackMode;

    if(drawingMode != TrackMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkGreen);
    }
    qDebug("Leave jpsGraphicsView::enableTrack");
}

bool jpsGraphicsView::statusHLine()
{
    qDebug("Enter jpsGraphicsView::statusHLine");
//    return _statHLine;
    if(drawingMode == HlineMode)
    {
        return true;
    } else
        return false;
    qDebug("Leave jpsGraphicsView::statusHLine");
}

void jpsGraphicsView::en_disableHLine()
{
    qDebug("Enter jpsGraphicsView::en_disableHLine");
    _statCopy=0;
    drawingMode = HlineMode;

    if(drawingMode != HlineMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkCyan);
    }
    qDebug("Leave jpsGraphicsView::en_disableHLine");
}

bool jpsGraphicsView::statusLandmark()
{
    qDebug("Enter jpsGraphicsView::en_disableHLine");
//    return statLandmark;
    if(drawingMode == LandmarkMode)
    {
        qDebug("Leave jpsGraphicsView::en_disableHLine");
        return true;
    } else
        qDebug("Leave jpsGraphicsView::en_disableHLine");
        return false;
}

void jpsGraphicsView::en_disableLandmark()
{
    qDebug("Enter jpsGraphicsView::en_disableLandmark");
    _statCopy=0;
    drawingMode = LandmarkMode;

    if(drawingMode != LandmarkMode)
    {
        emit no_drawing();
    }
    qDebug("Leave jpsGraphicsView::en_disableLandmark");
}

void jpsGraphicsView::start_Copy_function()
{
    qDebug("Enter jpsGraphicsView::start_Copy_function");
    _statCopy=1;
    qDebug("Leave jpsGraphicsView::start_Copy_function");
}

void jpsGraphicsView::Copy_lines(const QPointF& delta)
{
    qDebug("Enter jpsGraphicsView::Copy_lines");
    for (jpsLineItem* line:marked_lines)
    {
        addLineItem(line->get_line()->line().p1().x()+delta.x(),
                    line->get_line()->line().p1().y()+delta.y(),
                    line->get_line()->line().p2().x()+delta.x(),
                    line->get_line()->line().p2().y()+delta.y(),
                    line->getType());
    }
    _statCopy=0;
    qDebug("Leave jpsGraphicsView::Copy_lines");
}

void jpsGraphicsView::ScaleLines(const double &factor)   
{
    qDebug("Enter jpsGraphicsView::ScaleLines");
    for (jpsLineItem* lineItem:line_vector)
    {
        lineItem->get_line()->setLine(QLineF(lineItem->get_line()->line().p1()*factor,lineItem->get_line()->line().p2()*factor));
    }
  qDebug("Leave jpsGraphicsView::ScaleLines");
}

void jpsGraphicsView::selectedWindows()
{
    qDebug("Enter jpsGraphicsView::selectedWindows");
    statzoomwindows=true;
    qDebug("Leave jpsGraphicsView::selectedWindows");
}

void jpsGraphicsView::changeStart_endpoint(bool state)
{
    qDebug("Enter jpsGraphicsView::changeStart_endpoint");
    start_endpoint_snap=state;
    qDebug("Leave jpsGraphicsView::changeStart_endpoint");
}

void jpsGraphicsView::changeIntersections_point(bool state)
{
    qDebug("Enter jpsGraphicsView::changeIntersections_point");
    intersectionspoint_snap=state;
    qDebug("Leave jpsGraphicsView::changeIntersections_point");
}

void jpsGraphicsView::changeCenter_point(bool state)
{
    qDebug("Enter jpsGraphicsView::changeCenter_point");
    centerpoint_snap=state;
    qDebug("Leave jpsGraphicsView::changeCenter_point");
}

void jpsGraphicsView::changeLine_point(bool state)
{
    qDebug("Enter jpsGraphicsView::changeLine_point");
    linepoint_snap=state;
    qDebug("Leave jpsGraphicsView::changeLine_point");
}

QPointF jpsGraphicsView::getNearstPointOnLine(jpsLineItem* selected_line)
{
    qDebug("Enter jpsGraphicsView::getNearstPointOnLine");
    QPointF mouse_p1 = selected_line->get_line()->line().p1();
    QPointF mouse_p2 = selected_line->get_line()->line().p2();

    double APx=translated_pos.x() - mouse_p1.x();
    double APy=translated_pos.y() - mouse_p1.y();
    double ABx = mouse_p2.x() - mouse_p1.x();
    double ABy = mouse_p2.y() - mouse_p1.y();

    double magAB2 = ABx*ABx + ABy*ABy;
    double ABdotAP = ABx*APx + ABy*APy;
    double t = ABdotAP / magAB2;

    QPointF newPoint;

    if ( t < 0) {
        newPoint = mouse_p1;
    }else if (t > 1){
        newPoint = mouse_p2;
    }else{
        newPoint.setX(mouse_p1.x() + ABx*t);
        newPoint.setY(mouse_p1.y() + ABy*t);
    }
    qDebug("Leave jpsGraphicsView::getNearstPointOnLine");
    return newPoint;

}

/*
    since 0.8.8

    Draw background
 */
void jpsGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (_gridmode)
    {
        if (_statgrid=="Line")
            DrawLineGrid(painter,rect);
        else
            DrawPointGrid(painter,rect);
    }
}

void jpsGraphicsView::DrawLineGrid(QPainter *painter, const QRectF &rect)
{
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
}

void jpsGraphicsView::DrawPointGrid(QPainter *painter, const QRectF &rect)
{
    qDebug("Enter jpsGraphicsView::DrawPointGrid");
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
    qDebug("Leave jpsGraphicsView::DrawPointGrid");
}

void jpsGraphicsView::setDrawingMode(DrawingMode mode) {
    qDebug("Enter jpsGraphicsView::setDrawingMode");
    drawingMode = mode;
    qDebug("Leave jpsGraphicsView::setDrawingMode");
}

//Grid mode
void jpsGraphicsView::ChangeGridmode(const bool &stat)
{
    qDebug("Enter jpsGraphicsView::ChangeGridmode");
    _gridmode=stat;
    qDebug("Leave jpsGraphicsView::ChangeGridmode");
}

bool jpsGraphicsView::GetGridmode() const
{
    qDebug("Enter/return jpsGraphicsView::GetGridmode");
    return _gridmode;
}

void jpsGraphicsView::ChangeTranslation(qreal x, qreal y)
{
    qDebug("Enter jpsGraphicsView::ChangeTranslation");
    _translationX=x;
    _translationY=y;
    qDebug("Leave jpsGraphicsView::ChangeTranslation");
}

void jpsGraphicsView::SetGrid(QString grid)
{
    qDebug("Enter jpsGraphicsView::SetGrid");
    _statgrid=grid;
    qDebug("Leave jpsGraphicsView::SetGrid");
}

void jpsGraphicsView::ChangeGridSize(const qreal &gridSize)
{
    qDebug("Enter jpsGraphicsView::ChangeGridSize");
    _gridSize=gridSize;
    qDebug("Leave jpsGraphicsView::ChangeGridSize");
}


// For source
void jpsGraphicsView::enableSourceMode()
{
    qDebug("Enter jpsGraphicsView::enableSourceMode");
    setDrawingMode(SourceMode);

    if(drawingMode != SourceMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkRed);
    }
    qDebug("Leave jpsGraphicsView::enableSourceMode");
}

void jpsGraphicsView::drawSource()
{
    qDebug("Enter jpsGraphicsView::drawSource");
    if(currentSource == nullptr) // if the mouse was pressed first of two times
    {
        //Determining first point of source
        currentSource = this->scene()->addRect(translated_pos.x(),translated_pos.y(),0,0,currentPen);
        currentSource->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);

    } else
    {
        // if the mouse was pressed secondly of two times
        auto *sourceItem = new JPSSource(currentSource);
        sourceItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        this->scene()->addItem(sourceItem);

        emit sourcesChanged();

        // currentSource shouldn't be kept in scene, when source is saved
        this->scene()->removeItem(currentSource);
        delete currentSource;
        currentSource = nullptr;
    }
    qDebug("Leave jpsGraphicsView::drawSource");
}

/*
    since 0.8.8

    Will be used for showing sources in widget, sources list is saved in datamanager
 */
QList<JPSSource *> jpsGraphicsView::getSources() {
    qDebug("Enter jpsGraphicsView::getSources");

    QList<JPSSource *> sources;

        foreach(QGraphicsItem *item, items())
        {
            switch (item->type()) {
                case  SourceElementType:
                {
                    auto *source = qgraphicsitem_cast<JPSSource *>(item);
                    sources.append(source);
                    break;
                }
                default:
                    break;
            }
        }
    qDebug("Leave jpsGraphicsView::getSources");
    return sources;
}

void jpsGraphicsView::deleteSource(int index)
{
    qDebug("Enter jpsGraphicsView::deleteSource");
    if(getSources().at(index) != nullptr)
        scene()->removeItem(getSources().at(index));
    qDebug("Leave jpsGraphicsView::deleteSource");
}

void jpsGraphicsView::changeSource(int index)
{
    qDebug("Enter jpsGraphicsView::changeSource");
    if(getSources().at(index) != nullptr)
    {
        scene()->update();
    }
    qDebug("Leave jpsGraphicsView::changeSource");
}

void jpsGraphicsView::seleteSource(const QModelIndex &index)
{
    qDebug("Enter jpsGraphicsView::seleteSource");
    for(int i=0; i<getSources().size(); i++)
    {
        if(i==index.row())
        {
            getSources().at(i)->setSelected(true);
        }
        else
        {
            getSources().at(i)->setSelected(false);
        }
    }

//    if(getSources().at(index.row()) != nullptr)
//        getSources().at(index.row())->setSelected(true);


    this->scene()->update();
    qDebug("Leave jpsGraphicsView::seleteSource");
}

//QGraphicsItemGroup *jpsGraphicsView::getSourceGroup() const {
//    return sourceGroup;
//}

// GoalMode Mode
/*
    since 0.8.8

    Change drawing mode to goal
 */

void jpsGraphicsView::enableGoalMode()
{
    qDebug("Enter jpsGraphicsView::enableGoalMode");
    setDrawingMode(GoalMode);

    if(drawingMode != GoalMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::darkGreen);
    }
    qDebug("Leave jpsGraphicsView::enableGoalMode");
}

/*
    since 0.8.8

    Responds when mouse click
 */

void jpsGraphicsView::drawGoal()
{
    qDebug("Enter jpsGraphicsView::drawGoal");
    if(currentGoal == nullptr) // if the mouse was pressed first of two times
    {
        //Determining first point of source
        currentGoal = this->scene()->addRect(translated_pos.x(),translated_pos.y(),0,0,currentPen);
        currentGoal->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    } else
    {
        // if the mouse was pressed secondly of two times
        auto *goalItem = new JPSGoal(currentGoal);
        goalItem->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
        this->scene()->addItem(goalItem);

        emit goalsChanged();

        // currentGoal shouldn't be kept in scene, when source is saved
        this->scene()->removeItem(currentGoal);
        delete currentGoal;
        currentGoal = nullptr;
    }
    qDebug("Leave jpsGraphicsView::drawGoal");
}

/*
    since 0.8.8

    Will be used for showing goals in widget, goals list is saved in datamanager
 */
QList<JPSGoal *> jpsGraphicsView::getGoals()
{
    qDebug("Enter jpsGraphicsView::getGoals");
    QList<JPSGoal *> goals;

    foreach(QGraphicsItem *item, items())
    {
        switch (item->type()) {
            case GoalElementType:
            {
                auto *goal = qgraphicsitem_cast<JPSGoal *>(item);
                goals.append(goal);
                break;
            }
            default:
                break;
        }
    }
    qDebug("Leave jpsGraphicsView::getGoals");
    return goals;
}

void jpsGraphicsView::changeGoal(int index)
{
    qDebug("Enter jpsGraphicsView::changeGoal");
    if(getGoals().at(index) != nullptr)
    {
        scene()->update();
    }
    qDebug("Leave jpsGraphicsView::changeGoal");
}

void jpsGraphicsView::deleteGoal(int index)
{
    qDebug("Enter jpsGraphicsView::deleteGoal");
    if(getGoals().at(index) != nullptr)
        scene()->removeItem(getGoals().at(index));
    qDebug("Leave jpsGraphicsView::deleteGoal");
}


void jpsGraphicsView::seleteGoal(const QModelIndex &index)
{
    qDebug("Enter jpsGraphicsView::seleteGoal");
    for(int i=0; i<getGoals().size(); i++)
    {
        if(i==index.row())
        {
            getGoals().at(i)->setSelected(true);
        }
        else
        {
            getGoals().at(i)->setSelected(false);
        }
    }

    this->scene()->update();
    qDebug("Leave jpsGraphicsView::seleteGoal");
}

void jpsGraphicsView::drawMeasureLengthLine()
{
    qDebug("Enter jpsGraphicsView::drawMeasureLengthLine");
    if (current_line==nullptr) // if the mouse was pressed first of two times
    {
        //Determining first point of line
        current_line = this->scene()->addLine(translated_pos.x(),translated_pos.y(),translated_pos.x(),translated_pos.y(),currentPen);
        current_line->setVisible(false);
    }
    else // if the mouse was pressed secondly of two times
    {
        QString length = QString::number(current_line->line().length());
        QString Msg = "Length: " + length;
        emit sendMsgToStatusBar(Msg);

        //reset pointer
        delete current_line;
        current_line = nullptr;


    }
    qDebug("Leave jpsGraphicsView::drawMeasureLengthLine");
}

void jpsGraphicsView::enableMeasureLengthMode()
{
    qDebug("Enter jpsGraphicsView::enableMeasureLengthMode");
    setDrawingMode(MeasureMode);

    if(drawingMode != MeasureMode)
    {
        emit no_drawing();
    } else
    {
        currentPen.setColor(Qt::green);
    }
    qDebug("Leave jpsGraphicsView::enableMeasureLengthMode");
}

void jpsGraphicsView::addLayer()
{
    qDebug("Enter jpsGraphicsView::addLayer");
    auto *layer = new Layer();

    emit layersChanged();

    this->scene()->addItem(layer); // Add layer into scene

    layer_list.append(layer);
    qDebug("Leave jpsGraphicsView::addLayer");
}

void jpsGraphicsView::deleteLayer(Layer *deletedLayer)
{
    qDebug("Enter jpsGraphicsView::deleteLayer");
    layer_list.removeOne(deletedLayer);

    emit layersChanged();
    qDebug("Leave jpsGraphicsView::deleteLayer");
}

QList<Layer *> jpsGraphicsView::getLayerList() const
{
    qDebug("Enter jpsGraphicsView::getLayerList");
    return layer_list;
    qDebug("Leave jpsGraphicsView::getLayerList");
}

void jpsGraphicsView::setBackground(QString filename)
{
    qDebug("Enter jpsGraphicsView::setBackground");
    if(background != nullptr)
    {
        delete background;
        background = nullptr;
    }

    QPixmap image(filename);

    // Get a rotated pixmap
    QGraphicsPixmapItem importedImage(image);
    QTransform transform(importedImage.transform());
    transform.rotate(180, Qt::XAxis);
    QPixmap transformedImage = image.transformed(transform);

    background = new QGraphicsPixmapItem(transformedImage);
    this->scene()->addItem(background);

    // Moving by holding middle button
    background->setTransform(QTransform::fromTranslate(translation_x,translation_y), true);
    qDebug("Leave jpsGraphicsView::setBackground");
}

void jpsGraphicsView::showHideBackground()
{
    qDebug("Enter jpsGraphicsView::showHideBackground");
    if(background->isVisible())
    {
        background->setVisible(false);
    } else
    {
        background->setVisible(true);
    }
    qDebug("Leave jpsGraphicsView::showHideBackground");
}

void jpsGraphicsView::scaleUpBackground()
{
    qDebug("Enter jpsGraphicsView::scaleUpBackground");
    if(background != nullptr)
    {
        background->setScale(background->scale()+0.05);
    }
    qDebug("Leave jpsGraphicsView::scaleUpBackground");
}

void jpsGraphicsView::scaleDownBackground()
{
    qDebug("Enter jpsGraphicsView::scaleDownBackground");
    if(background != nullptr && background->scale() > 0.1)
    {
        background->setScale(background->scale()-0.05);
    }
    qDebug("Leave jpsGraphicsView::scaleDownBackground");
}

void jpsGraphicsView::clearMarkedLineList()
{
    qDebug("Enter jpsGraphicsView::clearMarkedLineList");
    marked_lines.clear();
    qDebug("Leave jpsGraphicsView::clearMarkedLineList");
}

void jpsGraphicsView::removeContents()
{
    qDebug("Enter jpsGraphicsView::removeContents");
    // Delete all contents which are stored in GraphicView, but not in datamanger(landmarks and marked lines)

    // Delete items in scene (sources, goals, transitions and all QGraphicItems);
    // If a list contains QGraphicItems, it just need to be cleared with clear();
    this->scene()->clear();

    // Delete all intersect points
    for (int i=0; i<intersect_point_vector.size(); i++)
    {
        delete intersect_point_vector[i];
        intersect_point_vector[i] = nullptr;
    }

    intersect_point_vector.clear();

    // Delete all lines
    for (int i=0; i<line_vector.size(); i++)
    {
        delete line_vector[i];
        line_vector[i] = nullptr;
    }

    line_vector.clear();

    // Delete all origins
    _origin.clear();

    caption_list.clear();

    layer_list.clear(); // Layer war already added into scene

    line_tracked=-1;

    update();
    qDebug("Leave jpsGraphicsView::removeContents");
}
