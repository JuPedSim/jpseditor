/**
 * \file        roomwidget.cpp
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
 * Which walls and/or doors belong to a certain room or obstacle can be defined with the help of this class.
 *
 *
 **/

#include "roomwidget.h"
#include "ui_roomwidget.h"
#include "rooms.h"
#include <iostream>



roomWidget::roomWidget(QWidget *parent, jpsDatamanager *dmanager, jpsGraphicsView *gview) :
    QTabWidget(parent),
    ui(new Ui::roomWidget)
{
    ui->setupUi(this);
    //rooms = new list_rooms(this);
    //rooms->setGeometry(QRect(10,80,291,81));
    datamanager=dmanager;
    graphview=gview;
    show_rooms();
    show_crossings();
    show_exits();
    show_obstacles();

    //temporary uncommented
    ui->auto_assign_doors->setVisible(false);
    //ui->auto_assign_exits->setVisible(false);

    // roomClasses
    ui->classBox->addItem("Not specified");
    ui->classBox->addItem("Corridor");
    ui->classBox->addItem("Office");
    ui->classBox->addItem("Lobby");
    ui->classBox->addItem("Entrance");
    ui->classBox->addItem("Stair");

    //SIGNALS AND SLOTS
    //close
    connect(ui->closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    connect(ui->closeButton_2,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    //connect(ui->closeButton_3,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    connect(ui->closeButton_4,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    //tab room
    connect(ui->new_room_button,SIGNAL(clicked(bool)),this,SLOT(new_room()));
    connect(ui->delete_room,SIGNAL(clicked(bool)),this,SLOT(delete_room()));
    connect(ui->chname_edit, SIGNAL( returnPressed() ), this, SLOT(change_roomname()));
    connect(ui->elevation_edit, SIGNAL(returnPressed()), this, SLOT(change_elevation()));
    connect(ui->add_button,SIGNAL(clicked(bool)),this,SLOT(addWall()));
    connect(ui->list_rooms,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(showWallsAndType()));
    connect(ui->list_rooms,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(showWallsAndType()));
    connect(ui->listWalls,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectWall()));
    connect(ui->remove_button,SIGNAL(clicked(bool)),this,SLOT(removeWall()));
    connect(ui->caption,SIGNAL(clicked(bool)),this,SLOT(shhi_roomCaption()));
    connect(ui->highlight,SIGNAL(clicked(bool)),this,SLOT(highlight_room()));
    connect(ui->classBox,SIGNAL(activated(int)),this,SLOT(ChangeRoomType()));
    connect(ui->classBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeRoomType()));
    //tab crossing
    connect(ui->addCrossingButton,SIGNAL(clicked(bool)),this,SLOT(new_crossing()));
    connect(ui->crossingList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(enable_roomSelectionCrossings()));
    connect(ui->roomBox1,SIGNAL(activated(int)),this,SLOT(add_rooms_to_crossing()));
    connect(ui->roomBox2,SIGNAL(activated(int)),this,SLOT(add_rooms_to_crossing()));
    connect(ui->removeCrossingButton,SIGNAL(clicked(bool)),this,SLOT(delete_crossing()));
    connect(ui->crossingList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(select_crossing()));
    //connect(ui->auto_assign_doors,SIGNAL(clicked(bool)),this,SLOT(autoAssignDoors()));
    //tab exit
//    connect(ui->addExitButton,SIGNAL(clicked(bool)),this,SLOT(new_exit()));
//    connect(ui->exitList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(enable_roomSelectionExits()));
//    connect(ui->roomBox_exits,SIGNAL(activated(int)),this,SLOT(add_rooms_to_exit()));
//    connect(ui->removeExitButton,SIGNAL(clicked(bool)),this,SLOT(delete_exit()));
//    connect(ui->exitList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(select_exit()));
//    connect(ui->auto_assign_exits,SIGNAL(clicked(bool)),this,SLOT(autoAssignExits()));
    //tab obstacles
    connect(ui->new_obs_button,SIGNAL(clicked(bool)),this,SLOT(new_obstacle()));
    connect(ui->delete_obs,SIGNAL(clicked(bool)),this,SLOT(delete_obstacle()));
    //connect(ui->apply_name_button_obs,SIGNAL(clicked(bool)),this,SLOT(change_obsName()));
    connect(ui->chname_edit_obs, SIGNAL( returnPressed() ), this, SLOT(change_obsName()));
    connect(ui->add_button_obs,SIGNAL(clicked(bool)),this,SLOT(addWallObs()));
    connect(ui->list_obstacles,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(showWallsObs()));
    connect(ui->listWallsObs,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectWallObs()));
    connect(ui->remove_button_obs,SIGNAL(clicked(bool)),this,SLOT(removeWallObs()));
    connect(ui->list_obstacles,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(enable_roomSelectionObs()));
    connect(ui->roomBox_obs,SIGNAL(activated(int)),this,SLOT(add_room_to_obs()));
    connect(ui->caption_obs,SIGNAL(clicked(bool)),this,SLOT(shhi_roomCaption()));
    connect(ui->highlight_obs,SIGNAL(clicked(bool)),this,SLOT(highlight_obs()));
    

    //lines in graphview deleted
    connect(graphview,SIGNAL(lines_deleted()),this,SLOT(show_all()));

}

roomWidget::~roomWidget()
{
    delete ui;
}

void roomWidget::show_rooms()
{
    ui->list_rooms->clear();
    QList<jpsRoom*> roomlist=datamanager->get_roomlist();
    for (int i=0; i<roomlist.size(); i++)
    {
        ui->list_rooms->addItem(roomlist[i]->get_name());
    }
    //ui->list_rooms->setCurrentRow(roomlist.size()-1); //why this line?

}

void roomWidget::show_crossings()
{

    ui->crossingList->clear();

    for (jpsCrossing* crossing:datamanager->get_crossingList())
    {
        QString string = "";
        string.sprintf("[%+04.3f, %+04.3f] - [%+04.3f, %+04.3f]", 
                       crossing->get_cLine()->get_line()->line().x1(),
                       crossing->get_cLine()->get_line()->line().x2(),
                       crossing->get_cLine()->get_line()->line().y1(), 
                       crossing->get_cLine()->get_line()->line().y2());
        ui->crossingList->addItem(string);

    }
//    for (jpsExit* crossing:datamanager->get_exitList())
//    {
//        QString string = "Door ";
//        string.append("x1:"+QString::number(crossing->get_cLine()->get_line()->line().x1()));
//        string.append(" x2:"+QString::number(crossing->get_cLine()->get_line()->line().x2()));
//        string.append(" y1:"+QString::number(crossing->get_cLine()->get_line()->line().y1()));
//        string.append(" y2:"+QString::number(crossing->get_cLine()->get_line()->line().y2()));
//        ui->crossingList->addItem(string);

//    }
    ui->crossingList->setCurrentRow(datamanager->get_crossingList().size()+datamanager->get_exitList().size()-1);
}

void roomWidget::show_exits()
{
//    ui->exitList->clear();

//    QList<jpsExit *> exitList=datamanager->get_exitList();

//    for (int i=0; i<exitList.size(); i++)
//    {
//        QString string = "Exit ";
//        string.append("x1:"+QString::number(exitList[i]->get_cLine()->get_line()->line().x1()));
//        string.append(" x2:"+QString::number(exitList[i]->get_cLine()->get_line()->line().x2()));
//        string.append(" y1:"+QString::number(exitList[i]->get_cLine()->get_line()->line().y1()));
//        string.append(" y2:"+QString::number(exitList[i]->get_cLine()->get_line()->line().y2()));
//        ui->exitList->addItem(string);

//    }
//    ui->exitList->setCurrentRow(exitList.size()-1);
}

void roomWidget::show_obstacles()
{
    ui->list_obstacles->clear();
    QList<jpsObstacle*> obslist=datamanager->get_obstaclelist();
    for (int i=0; i<obslist.size(); i++)
    {
        ui->list_obstacles->addItem(obslist[i]->get_name());
        ui->chname_edit_obs->setText(obslist[i]->get_name());
    }
    ui->list_obstacles->setCurrentRow(obslist.size()-1);

}

void roomWidget::new_room()
{

    datamanager->new_room();
    // Next three calls are necessary
    //if the user first generates a
    // crossing/exit/obs and secondly generates rooms
    enable_roomSelectionCrossings();
    enable_roomSelectionExits();
    enable_roomSelectionObs();
    this->show_rooms();
}

void roomWidget::delete_room()
{
    if (ui->list_rooms->currentItem()!=0L)
    {
        int cRow=ui->list_rooms->currentRow();
        datamanager->remove_room(datamanager->get_roomlist()[cRow]);
        ui->list_rooms->setCurrentRow(-1);
        this->show_rooms();
    }
}

void roomWidget::change_elevation()
{
    if (ui->list_rooms->currentItem()!=0L)
    {
        int crow=ui->list_rooms->currentRow();

        datamanager->get_roomlist()[crow]->set_elevation(ui->elevation_edit->text().toFloat());
        

    }

}
void roomWidget::change_roomname()
{
    if (ui->list_rooms->currentItem()!=0L)
    {
        // if there is a roomCaption it should be hided before the change of the name is done
        if (shhi_roomCaption()==false)
        {
            int crow=ui->list_rooms->currentRow();
            datamanager->get_roomlist()[crow]->change_name(ui->chname_edit->text());
            shhi_roomCaption();
            this->show_rooms();
        }
        else
        {
            shhi_roomCaption();
            int crow=ui->list_rooms->currentRow();
            datamanager->get_roomlist()[crow]->change_name(ui->chname_edit->text());
            this->show_rooms();
        }
    }
}

void roomWidget::addWall()
{
    if (graphview->get_markedLines().size()>0)
    {

        if (ui->list_rooms->currentItem()!=0L)
        {
            int crow=ui->list_rooms->currentRow();

            datamanager->get_roomlist()[crow]->addWall(graphview->get_markedLines());
            this->showWallsAndType();

        }
    }
}

void roomWidget::removeWall()
{
    if (graphview->get_markedLines().size()>0)
    {
        if (ui->list_rooms->currentItem()!=0L)
        {
            int crow=ui->list_rooms->currentRow();

            datamanager->get_roomlist()[crow]->removeWall(graphview->get_markedLines());
            this->showWallsAndType();
        }
    }
}

void roomWidget::showWallsAndType()
{
    ui->listWalls->clear();

    if (ui->list_rooms->currentItem()!=0L)
    {
        int crow=ui->list_rooms->currentRow();
        bool show = graphview->is_hide_roomCaption(datamanager->get_roomlist()[crow]->get_name());
        if(show)
             ui->caption->setText("Show Caption");
        else
             ui->caption->setText("Hide Caption");

        if (!datamanager->get_roomlist().isEmpty())
        {
            QList<jpsLineItem *> walllist=datamanager->get_roomlist()[crow]->get_listWalls();
            for (int i=0; i<walllist.size(); i++)
            {
                QString string = "";
                string.sprintf("[%+04.3f, %+04.3f] - [%+04.3f, %+04.3f]", 
                               walllist[i]->get_line()->line().x1(),
                               walllist[i]->get_line()->line().x2(),
                               walllist[i]->get_line()->line().y1(), 
                               walllist[i]->get_line()->line().y2());

                ui->listWalls->addItem(string);
            }
            
            ShowRoomType(crow);
            QString elevation = QString::number(datamanager->get_roomlist()[crow]->get_elevation());
            ui->elevation_edit->setText(elevation);
            QString room_name = datamanager->get_roomlist()[crow]->get_name();
            ui->chname_edit->setText(room_name);
            if(!datamanager->get_roomlist()[crow]->is_highlighted())
                 highlight_room();
            
            for(auto r: datamanager->get_roomlist())
                 if(datamanager->get_roomlist()[crow]->get_name() != r->get_name())
                      if(r->is_highlighted())
                           r->highlight();
            

        }
    }
}

void roomWidget::selectWall()
{
    if (ui->listWalls->currentItem()!=0L)
    {
        if (graphview->get_markedLines().size()>0)
        {
            graphview->unmark_all_lines();
        }
        int cWallRow=ui->listWalls->currentRow();
        int cRoomRow=ui->list_rooms->currentRow();

        graphview->select_line(datamanager->get_roomlist()[cRoomRow]->get_listWalls()[cWallRow]);

    }
}

void roomWidget::new_crossing()
{
    if (graphview->get_markedLines().size()>0)
    {
        datamanager->new_crossing(graphview->get_markedLines());
    }
    show_crossings();

}

void roomWidget::enable_roomSelectionCrossings()
{
    if (datamanager->get_crossingList().size()>0)
    {
        ui->roomBox1->setEnabled(true);
        ui->roomBox2->setEnabled(true);
        ui->crossing_between->setEnabled(true);
        ui->and_label->setEnabled(true);
        ui->roomBox1->clear();
        ui->roomBox2->clear();
        QList<QString> roomNameList;
        for (int i=0; i<datamanager->get_roomlist().size(); i++)
        {
            roomNameList.push_back(datamanager->get_roomlist()[i]->get_name());
        }
        if (!roomNameList.isEmpty())
        {
            ui->roomBox1->addItems(roomNameList);

            ui->roomBox2->addItems(roomNameList);
            ui->roomBox2->addItem("OUTSIDE");


            int cCrossingRow=ui->crossingList->currentRow();
            if (cCrossingRow!=-1) // -1 means there is no current item
            {
                QList<jpsRoom*> cRoomlist = datamanager->get_crossingList()[cCrossingRow]->get_roomList();
                if (cRoomlist.size()>1)
                {
                    int index = datamanager->get_roomlist().indexOf(cRoomlist[0]);
                    ui->roomBox1->setCurrentIndex(index);
                    index = datamanager->get_roomlist().indexOf(cRoomlist[1]);
                    ui->roomBox2->setCurrentIndex(index);
                }
                else if (cRoomlist.size()>0 && datamanager->get_crossingList()[cCrossingRow]->IsExit())
                {
                    int index = datamanager->get_roomlist().indexOf(cRoomlist[0]);
                    ui->roomBox1->setCurrentIndex(index);
                    ui->roomBox2->setCurrentIndex(ui->roomBox2->count()-1);
                }
                else
                {
                    add_rooms_to_crossing();
                }
            }
        }
    }
    else
    {
        disable_roomSelectionCrossings();
    }

}

void roomWidget::disable_roomSelectionCrossings()
{
    ui->roomBox1->setEnabled(false);
    ui->roomBox2->setEnabled(false);
    ui->crossing_between->setEnabled(false);
    ui->and_label->setEnabled(false);

}

void roomWidget::add_rooms_to_crossing()
{    
    if (ui->crossingList->currentItem()!=0L)
    {    
        int cCrossingRow=ui->crossingList->currentRow();
        
        if (ui->roomBox1->currentIndex()!=-1 && ui->roomBox2->currentIndex()!=-1)
        {
            int cRoomRow1=ui->roomBox1->currentIndex();
            int cRoomRow2=ui->roomBox2->currentIndex();            
            if (ui->roomBox2->currentText()=="OUTSIDE")
            {
                datamanager->get_crossingList()[cCrossingRow]->add_rooms(datamanager->get_roomlist()[cRoomRow1]);
                datamanager->get_crossingList()[cCrossingRow]->SetStatExit(true);
                datamanager->get_crossingList()[cCrossingRow]->get_cLine()->set_Exit();
            }

            else
            {
                datamanager->get_crossingList()[cCrossingRow]->add_rooms(datamanager->get_roomlist()[cRoomRow1],
                                                                 datamanager->get_roomlist()[cRoomRow2]);
                datamanager->get_crossingList()[cCrossingRow]->SetStatExit(false);
                datamanager->get_crossingList()[cCrossingRow]->get_cLine()->set_Door();
            }
        }

    }
}

void roomWidget::delete_crossing()
{
    if (ui->crossingList->currentItem()!=0L)
    {
        int index = ui->crossingList->currentRow();
        datamanager->remove_crossing(datamanager->get_crossingList()[index]);
        ui->crossingList->setCurrentRow(-1);
        this->show_crossings();

    }
}

void roomWidget::select_crossing()
{
    if (ui->crossingList->currentItem()!=0L)
    {
        if (graphview->get_markedLines().size()>0)
        {
            graphview->unmark_all_lines();
        }
        int cCrossRow=ui->crossingList->currentRow();

        graphview->select_line(datamanager->get_crossingList()[cCrossRow]->get_cLine());

    }
}

void roomWidget::new_exit()
{
    if (graphview->get_markedLines().size()>0)
    {
        datamanager->new_exit(graphview->get_markedLines());
    }
    show_exits();

}

void roomWidget::enable_roomSelectionExits()
{
//    if (datamanager->get_exitList().size()>0)
//    {
//        //ui->roomBox_exits->setEnabled(true);
//        ui->exit_from->setEnabled(true);
//        ui->to_label->setEnabled(true);
//        ui->outside->setEnabled(true);
//        ui->roomBox_exits->clear();

//        QList<QString> roomNameList;
//        for (int i=0; i<datamanager->get_roomlist().size(); i++)
//        {
//            roomNameList.push_back(datamanager->get_roomlist()[i]->get_name());
//        }
//        if (roomNameList.isEmpty()==false)
//        {
//             ui->roomBox_exits->addItems(roomNameList);

//            if (ui->exitList->currentRow()!=-1)
//            {
//                int cExitRow=ui->exitList->currentRow();
//                QList<jpsRoom*> cRoomlist = datamanager->get_exitList()[cExitRow]->get_roomList();
//                if (cRoomlist.size()>0)
//                {
//                    int index = datamanager->get_roomlist().indexOf(cRoomlist[0]);
//                    ui->roomBox_exits->setCurrentIndex(index);
//                }
//                else
//                {
//                    add_rooms_to_exit();
//                }
//            }
//        }
//    }
//    else
//    {
//        disable_roomSelectionExits();
//    }

}

void roomWidget::disable_roomSelectionExits()
{
//    ui->roomBox_exits->setEnabled(false);

//    ui->exit_from->setEnabled(false);
//    ui->to_label->setEnabled(false);
//    ui->outside->setEnabled(false);

}

void roomWidget::add_rooms_to_exit()
{

//    if (ui->exitList->currentItem()!=0L)
//    {
//        int cExitRow=ui->exitList->currentRow();
//        if (ui->roomBox_exits->currentIndex()!=-1)
//        {
//            int cRoomRow=ui->roomBox_exits->currentIndex();
//            datamanager->get_exitList()[cExitRow]->add_rooms(datamanager->get_roomlist()[cRoomRow]);
//        }
//    }

}

void roomWidget::delete_exit()
{
//    if (ui->exitList->currentItem()!=0L)
//    {
//        int index = ui->exitList->currentRow();
//        datamanager->remove_exit(datamanager->get_exitList()[index]);
//        ui->exitList->setCurrentRow(-1);
//        this->show_exits();

//    }
}

void roomWidget::select_exit()
{
//    if (ui->exitList->currentItem()!=0L)
//    {
//        if (graphview->get_markedLines().size()>0)
//        {
//            graphview->unmark_all_lines();
//        }
//        int cExitRow=ui->exitList->currentRow();


//        graphview->select_line(datamanager->get_exitList()[cExitRow]->get_cLine());

//    }
}

void roomWidget::show_all()
{
    show_rooms();
    show_crossings();
    show_exits();
    show_obstacles();
    showWallsAndType();
    showWallsObs();
}


void roomWidget::new_obstacle()
{
    datamanager->new_obstacle();
    this->show_obstacles();
}

void roomWidget::delete_obstacle()
{
    if (ui->list_obstacles->currentItem()!=0L)
    {
        int cRow=ui->list_obstacles->currentRow();

        datamanager->remove_obstacle(datamanager->get_obstaclelist()[cRow]);
        ui->list_obstacles->setCurrentRow(-1);
        this->show_obstacles();
    }
}

void roomWidget::change_obsName()
{
    if (ui->list_obstacles->currentItem()!=0L)
    {
        int crow=ui->list_obstacles->currentRow();

            datamanager->get_obstaclelist()[crow]->change_name(ui->chname_edit_obs->text());

        this->show_obstacles();
    }
}

void roomWidget::addWallObs()
{
    if (graphview->get_markedLines().size()>0)
    {

        if (ui->list_obstacles->currentItem()!=0L)
        {
            int crow=ui->list_obstacles->currentRow();

            datamanager->get_obstaclelist()[crow]->addWall(graphview->get_markedLines());
            this->showWallsObs();

        }
    }
}

void roomWidget::removeWallObs()
{
    if (graphview->get_markedLines().size()>0)
    {
        if (ui->list_obstacles->currentItem()!=0L)
        {
            int crow=ui->list_obstacles->currentRow();

            datamanager->get_obstaclelist()[crow]->removeWall(graphview->get_markedLines());
            this->showWallsObs();
        }
    }
}

void roomWidget::showWallsObs()
{
    ui->listWallsObs->clear();

    if (ui->list_obstacles->currentItem()!=0L)
    {
        int crow=ui->list_obstacles->currentRow();

        if (!datamanager->get_obstaclelist().isEmpty())
        {
            QList<jpsLineItem *> walllist=datamanager->get_obstaclelist()[crow]->get_listWalls();

            for (int i=0; i<walllist.size(); i++)
            {
                QString string = "";
                string.sprintf("[%+04.3f, %+04.3f] - [%+04.3f, %+04.3f]", 
                               walllist[i]->get_line()->line().x1(),
                               walllist[i]->get_line()->line().x2(),
                               walllist[i]->get_line()->line().y1(),
                               walllist[i]->get_line()->line().y2());
                
                ui->listWallsObs->addItem(string);
            }
        }
    }
}

void roomWidget::selectWallObs()
{
    if (ui->listWallsObs->currentItem()!=0L)
    {
        if (graphview->get_markedLines().size()>0)
        {
            graphview->unmark_all_lines();
        }
        int cWallRow=ui->listWallsObs->currentRow();
        int cObsRow=ui->list_obstacles->currentRow();

        graphview->select_line(datamanager->get_obstaclelist()[cObsRow]->get_listWalls()[cWallRow]);

    }
}

void roomWidget::add_room_to_obs()
{
    if (ui->list_obstacles->currentItem()!=0L)
    {
        int cObsRow=ui->list_obstacles->currentRow();
        int cRoomRow=ui->roomBox_obs->currentIndex();
        if (cRoomRow!=-1)
        {
            datamanager->get_obstaclelist()[cObsRow]->set_room(datamanager->get_roomlist()[cRoomRow]);
        }
    }

}




void roomWidget::enable_roomSelectionObs()
{
    if (datamanager->get_obstaclelist().size()>0)
    {
        ui->roomBox_obs->setEnabled(true);
        ui->is_in->setEnabled(true);      
        ui->roomBox_obs->clear();

        QList<QString> roomNameList;
        for (int i=0; i<datamanager->get_roomlist().size(); i++)
        {
            roomNameList.push_back(datamanager->get_roomlist()[i]->get_name());
        }
        if (roomNameList.isEmpty()==false)
        {
             ui->roomBox_obs->addItems(roomNameList);
        }
        int cObsRow=ui->list_obstacles->currentRow();
        if (cObsRow!=-1)
        {
            jpsRoom* cRoom = datamanager->get_obstaclelist()[cObsRow]->get_room();
            if (cRoom!=0L)
            {
                int index = datamanager->get_roomlist().indexOf(cRoom);
                ui->roomBox_obs->setCurrentIndex(index);
            }
            else
            {
                add_room_to_obs();
            }
        }
    }
    else
    {
        disable_roomSelectionObs();
    }

}

void roomWidget::disable_roomSelectionObs()
{
    ui->roomBox_obs->setEnabled(false);
    ui->is_in->setEnabled(false);
}

bool roomWidget::shhi_roomCaption() // obstacles!!!!!!!
{
     bool show;
    if (ui->list_rooms->currentItem()!=0L)
    {
        int cRow=ui->list_rooms->currentRow();
        QString roomName = datamanager->get_roomlist()[cRow]->get_name();
        QPointF roomCenter = datamanager->get_roomlist()[cRow]->get_center();
        show = graphview->show_hide_roomCaption(roomName,roomCenter.x(),roomCenter.y());
        if(show)
             ui->caption->setText("Hide Caption");
        else
             ui->caption->setText("Show Caption");
        return show;
    }
    return false;
}

void roomWidget::highlight_room()
{
    if (ui->list_rooms->currentItem()!=0L)
    {
        int cRow=ui->list_rooms->currentRow();
        datamanager->get_roomlist()[cRow]->highlight();
        
    }

}

void roomWidget::highlight_obs()
{
    if (ui->list_obstacles->currentItem()!=0L)
    {
        int cRow=ui->list_obstacles->currentRow();
        datamanager->get_obstaclelist()[cRow]->highlight();
    }

}

//void roomWidget::autoAssignDoors()
//{
//    datamanager->AutoAssignCrossings();
//    show_all();
//    enable_roomSelectionCrossings();
//}

//void roomWidget::autoAssignExits()
//{
//    datamanager->AutoAssignExits();
//    show_all();
//    enable_roomSelectionExits();

//}

void roomWidget::ChangeRoomType()
{
    if (ui->list_rooms->currentItem()!=0L)
    {
        int cRoomRow=ui->list_rooms->currentRow();
        int cClassRow=ui->classBox->currentIndex();
        if (cClassRow!=-1)
        {
            datamanager->get_roomlist()[cRoomRow]->set_type(ui->classBox->currentText());
        }
    }

}

void roomWidget::ShowRoomType(int& cRow) const
{
    QString type = datamanager->get_roomlist()[cRow]->get_type();
    int itemindex = ui->classBox->findText(type);
    ui->classBox->setCurrentIndex(itemindex);

}

















