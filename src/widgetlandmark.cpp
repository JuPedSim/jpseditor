/**
 * \file        widgetlandmark.cpp
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
 * With the help of this class the user is able to handle landmarks and set associations (waypoints) relying on a certain landmark.
 * widgetlandmark is not enabled in v0.8.1.
 *
 *
 **/

#include "widgetlandmark.h"
#include "ui_widgetlandmark.h"

#include <iostream>


widgetLandmark::widgetLandmark(QWidget *parent, jpsDatamanager *dmanager, jpsGraphicsView *gview):
    QTabWidget(parent),
    ui(new Ui::widgetLandmark)
{
    ui->setupUi(this);
    _dmanager=dmanager;
    _gview=gview;
    _waypointIDCounter=0;

    show_landmarks();


    connect(ui->apply_name_button,SIGNAL(clicked(bool)),this,SLOT(change_name()));
    connect(ui->closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->list_landmarks,SIGNAL(activated(int)),this,SLOT(enable_room_selection()));
    connect(ui->list_landmarks,SIGNAL(currentIndexChanged(int)),_gview,SLOT(unmarkLandmark()));
    connect(ui->roomBox_landmarks,SIGNAL(activated(int)),this,SLOT(add_room_to_landmark()));
    connect(ui->add_button,SIGNAL(clicked(bool)),_gview,SLOT(StatPositionDef()));
    connect(_gview,SIGNAL(PositionDefCompleted()),this,SLOT(SetPosInCMap()));
    connect(ui->remove_button,SIGNAL(clicked(bool)),this,SLOT(RemoveAssociation()));
    connect(ui->button_showhide,SIGNAL(clicked(bool)),this,SLOT(ShowHideLandmark()));

    //connectionDef
    connect(ui->add_button_connections,SIGNAL(clicked(bool)),this,SLOT(NewConnection()));
    connect(_gview,SIGNAL(DefConnection1Completed()),this,SLOT(AskForSecondLandmark()));
    connect(_gview,SIGNAL(DefConnection2Completed()),this,SLOT(SetLandmarksToConnection()));

}

widgetLandmark::~widgetLandmark()
{
    delete ui;
}

void widgetLandmark::show_landmarks()
{
    ui->list_landmarks->clear();
    QList<jpsLandmark*> landmarks=_dmanager->get_landmarks();
    for (jpsLandmark* landmark:landmarks)
    {
        ui->list_landmarks->addItem(landmark->GetCaption());
    }
    if (!landmarks.empty())
        enable_room_selection();

}

void widgetLandmark::add_room_to_landmark()
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int cLanRow=ui->list_landmarks->currentIndex();
        int cRoomRow=ui->roomBox_landmarks->currentIndex();
        if (cRoomRow!=-1)
        {
            _dmanager->get_landmarks()[cLanRow]->SetRoom(_dmanager->get_roomlist()[cRoomRow]);
        }
    }
}

void widgetLandmark::enable_room_selection()
{
    if (_dmanager->get_landmarks().size()>0)
    {
        ui->roomBox_landmarks->setEnabled(true);
        ui->is_in->setEnabled(true);
        ui->roomBox_landmarks->clear();
        ui->add_button->setEnabled(true);
        ui->remove_button->setEnabled(true);


        QList<QString> roomNameList;
        for (int i=0; i<_dmanager->get_roomlist().size(); i++)
        {
            roomNameList.push_back(_dmanager->get_roomlist()[i]->get_name());
        }
        if (roomNameList.isEmpty()==false)
        {
             ui->roomBox_landmarks->addItems(roomNameList);
        }
        int cLanRow=ui->list_landmarks->currentIndex();
        if (cLanRow!=-1)
        {
            jpsRoom* cRoom = _dmanager->get_landmarks()[cLanRow]->GetRoom();
            if (cRoom!=nullptr)
            {
                int index = _dmanager->get_roomlist().indexOf(cRoom);
                ui->roomBox_landmarks->setCurrentIndex(index);
            }
            else
            {
                add_room_to_landmark();
            }


        }
        //mark Landmark
        _gview->select_landmark(_dmanager->get_landmarks()[cLanRow]);


    }
    else
    {
        disable_room_selection();
    }
}

void widgetLandmark::disable_room_selection()
{
    ui->roomBox_landmarks->setEnabled(false);
    ui->is_in->setEnabled(false);
    ui->label_Waypoints->setEnabled(false);
    ui->label_Waypoints2->setEnabled(false);
    ui->add_button->setEnabled(false);
    ui->remove_button->setEnabled(false);
}

void widgetLandmark::change_name()
{    
    jpsLandmark* landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        landmark->SetCaption(ui->chname_edit->text());
        int row = ui->list_landmarks->currentIndex();
        this->show_landmarks();
        ui->list_landmarks->setCurrentIndex(row);
    }

}

void widgetLandmark::SetPosInCMap()
{
    jpsLandmark* landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        landmark->SetRect(_gview->GetCurrentSelectRect()->rect());
        QString string = "Ellipse: x: "+QString::number(landmark->GetRect().center().x())
                + " y: "+QString::number(landmark->GetRect().center().y())+" rA: "+QString::number(landmark->GetA())
                                         + " rB: "+QString::number(landmark->GetB());
        ui->ellipse_label->setText(string);
        ui->add_button->setChecked(false);

        // show ellipse and text in graphics view
        QGraphicsEllipseItem* ellipse = _gview->GetScene()->addEllipse(landmark->GetRect(),QPen(Qt::blue,0));
        ellipse->setTransform(QTransform::fromTranslate(_gview->GetTranslationX(),_gview->GetTranslationY()), true);
        QGraphicsTextItem* text = _gview->GetScene()->addText(landmark->GetCaption());
        text->setPos(landmark->GetPos().x()+_gview->GetTranslationX(),landmark->GetPos().y()+_gview->GetTranslationY());
        //text->setScale(gl_scale_f);
        text->setData(0,_gview->GetScaleF());
        text->setTransform(QTransform::fromScale(_gview->GetScaleF(),-_gview->GetScaleF()),true);
        landmark->SetEllipseItem(ellipse);
        landmark->SetTextItem(text);
    }

}



void widgetLandmark::AddAssociation()
{
//    if (ui->list_landmarks->currentIndex()!=-1)
//    {
//        int cLanRow=ui->list_landmarks->currentIndex();

//        _dmanager->get_landmarks()[cLanRow]->AddWaypoint(std::make_shared<jpsWaypoint>(_gview->GetCurrentSelectRect()->rect(),_waypointIDCounter));

//        ShowAssociations();
//        _waypointIDCounter++;
//        ui->add_button->setChecked(false);
//    }
}

void widgetLandmark::ShowHideLandmark()
{
    jpsLandmark* landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        if (landmark->GetEllipseItem()==nullptr && landmark->GetPixmap()->isVisible())
        {
            landmark->GetPixmap()->setVisible(false);
            landmark->GetPixmapTextItem()->setVisible(false);
        }
        else if (landmark->GetEllipseItem()==nullptr && !landmark->GetPixmap()->isVisible())
        {
            landmark->GetPixmap()->setVisible(true);
            landmark->GetPixmapTextItem()->setVisible(true);
        }
        else if (!landmark->GetEllipseItem()->isVisible())
        {
            landmark->GetPixmap()->setVisible(true);
            landmark->GetPixmapTextItem()->setVisible(true);
            landmark->GetEllipseItem()->setVisible(true);
            landmark->GetTextItem()->setVisible(true);
        }
        else
        {
            landmark->GetPixmap()->setVisible(false);
            landmark->GetPixmapTextItem()->setVisible(false);
            landmark->GetEllipseItem()->setVisible(false);
            landmark->GetTextItem()->setVisible(false);
        }

    }
}

void widgetLandmark::NewConnection()
{
    jpsConnection* connection = new jpsConnection();
    _dmanager->NewConnection(connection);
    _currentConnection=connection;
    AskForFirstLandmark();
}

void widgetLandmark::AskForFirstLandmark()
{
    _gview->SetStatDefConnections(1);
}

void widgetLandmark::AskForSecondLandmark()
{
    // if mousePressed: if mousepos in landmark: first landmark set in connection
    QPointF mousePos = _gview->return_Pos();
    for (jpsLandmark* landmark:_dmanager->get_landmarks())
    {
        if (landmark->GetEllipseItem()!=nullptr)
        {
            if (landmark->GetEllipseItem()->boundingRect().contains(mousePos))
            {
                _currentConnection->SetFirstLandmark(std::shared_ptr<jpsLandmark>(landmark));
                _gview->SetStatDefConnections(2);
                return;
            }
        }
    }
}

void widgetLandmark::SetLandmarksToConnection()
{
    // if mousePressed: if mousepos in landmark: first landmark set in connection
    QPointF mousePos = _gview->return_Pos();
    for (jpsLandmark* landmark:_dmanager->get_landmarks())
    {
        if (landmark->GetEllipseItem()!=nullptr)
        {
            if (landmark->GetEllipseItem()->boundingRect().contains(mousePos))
            {
                _currentConnection->SetSecondLandmark(std::shared_ptr<jpsLandmark>(landmark));
                _gview->SetStatDefConnections(0);
                SetLineItemAsConnection();
                ui->add_button_connections->setChecked(false);
                return;
            }
        }
    }
    ui->add_button_connections->setChecked(false);
}

void widgetLandmark::SetLineItemAsConnection()
{
    QLineF line = QLineF(_currentConnection->GetLandmarks().first->GetPos(),_currentConnection->GetLandmarks().second->GetPos());
    QPen pen = QPen(Qt::blue,2);
    pen.setCosmetic(true);
    QGraphicsLineItem* lineItem = _gview->GetScene()->addLine(line,pen);
    _currentConnection->SetLineItem(lineItem);
    _currentConnection=nullptr;
}

void widgetLandmark::RemoveAssociation()
{
//    if (ui->list_landmarks->currentIndex()!=-1)
//    {

//        int cLanRow=ui->list_landmarks->currentIndex();

//        if (ui->listWaypoints->currentItem()!=nullptr)
//        {
//            int cWayRow=ui->listWaypoints->currentRow();

//            _dmanager->get_landmarks()[cLanRow]->RemoveWaypoint(_dmanager->get_landmarks()[cLanRow]->GetWaypoints()[cWayRow]);
//            ShowAssociations();
//        }

//    }
}

jpsLandmark* widgetLandmark::GetCurrentLandmark() const
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int crow=ui->list_landmarks->currentIndex();

        return _dmanager->get_landmarks()[crow];
    }
    else
        return nullptr;
}
