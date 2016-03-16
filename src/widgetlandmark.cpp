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
    connect(ui->add_button,SIGNAL(clicked(bool)),_gview,SLOT(StatAssoDef()));
    connect(_gview,SIGNAL(AssoDefCompleted()),this,SLOT(AddAssociation()));
    connect(ui->remove_button,SIGNAL(clicked(bool)),this,SLOT(RemoveAssociation()));
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
        ui->list_landmarks->addItem(landmark->get_name());
    }

}

void widgetLandmark::add_room_to_landmark()
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int cLanRow=ui->list_landmarks->currentIndex();
        int cRoomRow=ui->roomBox_landmarks->currentIndex();
        if (cRoomRow!=-1)
        {
            _dmanager->get_landmarks()[cLanRow]->set_room(_dmanager->get_roomlist()[cRoomRow]);
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
        ui->label_Waypoints->setEnabled(true);
        ui->label_Waypoints2->setEnabled(true);
        ui->listWaypoints->setEnabled(true);
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
            jpsRoom* cRoom = _dmanager->get_landmarks()[cLanRow]->get_room();
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

        ShowAssociations();

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
    ui->listWaypoints->setEnabled(false);
    ui->add_button->setEnabled(false);
    ui->remove_button->setEnabled(false);
}

void widgetLandmark::change_name()
{    
    ptrLandmark landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        landmark->SetCaption(ui->chname_edit->text());
        this->show_landmarks();
    }

}

void widgetLandmark::SetPosInCMap()
{
    ptrLandmark landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
        landmark->SetRect(_gview->GetCurrentSelectRect()->rect());

}

void widgetLandmark::AddAssociation()
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int cLanRow=ui->list_landmarks->currentIndex();

        _dmanager->get_landmarks()[cLanRow]->AddWaypoint(std::make_shared<jpsWaypoint>(_gview->GetCurrentSelectRect()->rect(),_waypointIDCounter));

        ShowAssociations();
        _waypointIDCounter++;
        ui->add_button->setChecked(false);
    }
}

void widgetLandmark::ShowHideLandmark()
{
    ptrLandmark landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        _gview->ShowHideLandmark(landmark);
    }
}

void widgetLandmark::RemoveAssociation()
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {

        int cLanRow=ui->list_landmarks->currentIndex();

        if (ui->listWaypoints->currentItem()!=nullptr)
        {
            int cWayRow=ui->listWaypoints->currentRow();

            _dmanager->get_landmarks()[cLanRow]->RemoveWaypoint(_dmanager->get_landmarks()[cLanRow]->GetWaypoints()[cWayRow]);
            ShowAssociations();
        }

    }
}

ptrLandmark widgetLandmark::GetCurrentLandmark() const
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int crow=ui->list_landmarks->currentIndex();

        return _dmanager->get_landmarks()[crow];
    }
    else
        return nullptr;
}
