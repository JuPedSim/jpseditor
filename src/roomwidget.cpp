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

    //SIGNALS AND SLOTS
    //close
    connect(ui->closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    connect(ui->closeButton_2,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    connect(ui->closeButton_3,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    connect(ui->closeButton_4,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_room()));
    //tab room
    connect(ui->new_room_button,SIGNAL(clicked(bool)),this,SLOT(new_room()));
    connect(ui->delete_room,SIGNAL(clicked(bool)),this,SLOT(delete_room()));
    connect(ui->apply_name_button,SIGNAL(clicked(bool)),this,SLOT(change_roomname()));
    connect(ui->add_button,SIGNAL(clicked(bool)),this,SLOT(addWall()));
    connect(ui->list_rooms,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(showWalls()));
    connect(ui->listWalls,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectWall()));
    connect(ui->remove_button,SIGNAL(clicked(bool)),this,SLOT(removeWall()));
    connect(ui->caption,SIGNAL(clicked(bool)),this,SLOT(shhi_roomCaption()));
    connect(ui->highlight,SIGNAL(clicked(bool)),this,SLOT(highlight_room()));
    //tab crossing
    connect(ui->addCrossingButton,SIGNAL(clicked(bool)),this,SLOT(new_crossing()));
    connect(ui->crossingList,SIGNAL(itemSelectionChanged()),this,SLOT(enable_roomSelectionCrossings()));
    connect(ui->roomBox1,SIGNAL(activated(int)),this,SLOT(add_rooms_to_crossing()));
    connect(ui->roomBox2,SIGNAL(activated(int)),this,SLOT(add_rooms_to_crossing()));
    connect(ui->removeCrossingButton,SIGNAL(clicked(bool)),this,SLOT(delete_crossing()));
    connect(ui->crossingList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(select_crossing()));
    //tab exit
    connect(ui->addExitButton,SIGNAL(clicked(bool)),this,SLOT(new_exit()));
    connect(ui->exitList,SIGNAL(itemSelectionChanged()),this,SLOT(enable_roomSelectionExits()));
    connect(ui->roomBox_exits,SIGNAL(activated(int)),this,SLOT(add_rooms_to_exit()));
    connect(ui->removeExitButton,SIGNAL(clicked(bool)),this,SLOT(delete_exit()));
    connect(ui->exitList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(select_exit()));
    //tab obstacles
    connect(ui->new_obs_button,SIGNAL(clicked(bool)),this,SLOT(new_obstacle()));
    connect(ui->delete_obs,SIGNAL(clicked(bool)),this,SLOT(delete_obstacle()));
    connect(ui->apply_name_button_obs,SIGNAL(clicked(bool)),this,SLOT(change_obsName()));
    connect(ui->add_button_obs,SIGNAL(clicked(bool)),this,SLOT(addWallObs()));
    connect(ui->list_obstacles,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(showWallsObs()));
    connect(ui->listWallsObs,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectWallObs()));
    connect(ui->remove_button_obs,SIGNAL(clicked(bool)),this,SLOT(removeWallObs()));
    connect(ui->list_obstacles,SIGNAL(itemSelectionChanged()),this,SLOT(enable_roomSelectionObs()));
    connect(ui->roomBox_obs,SIGNAL(activated(int)),this,SLOT(add_room_to_obs()));
    connect(ui->caption_obs,SIGNAL(clicked(bool)),this,SLOT(shhi_roomCaption()));
    connect(ui->highlight_obs,SIGNAL(clicked(bool)),this,SLOT(highlight_room()));

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

}

void roomWidget::show_crossings()
{

    ui->crossingList->clear();


    QList<jpsCrossing *> crossingList=datamanager->get_crossingList();

    for (int i=0; i<crossingList.size(); i++)
    {
        QString string = "Crossing ";
        string.append("x1:"+QString::number(crossingList[i]->get_cLine()->get_line()->line().x1()));
        string.append(" x2:"+QString::number(crossingList[i]->get_cLine()->get_line()->line().x2()));
        string.append(" y1:"+QString::number(crossingList[i]->get_cLine()->get_line()->line().y1()));
        string.append(" y2:"+QString::number(crossingList[i]->get_cLine()->get_line()->line().y2()));
        ui->crossingList->addItem(string);
        ui->crossingList->setCurrentRow(crossingList.size()-1);
    }



}

void roomWidget::show_exits()
{
    ui->exitList->clear();

    QList<jpsExit *> exitList=datamanager->get_exitList();

    for (int i=0; i<exitList.size(); i++)
    {
        QString string = "Exit ";
        string.append("x1:"+QString::number(exitList[i]->get_cLine()->get_line()->line().x1()));
        string.append(" x2:"+QString::number(exitList[i]->get_cLine()->get_line()->line().x2()));
        string.append(" y1:"+QString::number(exitList[i]->get_cLine()->get_line()->line().y1()));
        string.append(" y2:"+QString::number(exitList[i]->get_cLine()->get_line()->line().y2()));
        ui->exitList->addItem(string);
        ui->exitList->setCurrentRow(exitList.size()-1);
    }
}

void roomWidget::show_obstacles()
{
    ui->list_obstacles->clear();
    QList<jpsObstacle*> obslist=datamanager->get_obstaclelist();
    for (int i=0; i<obslist.size(); i++)
    {
        ui->list_obstacles->addItem(obslist[i]->get_name());
    }

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
            this->showWalls();

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
            this->showWalls();
        }
    }
}

void roomWidget::showWalls()
{
    ui->listWalls->clear();

    if (ui->list_rooms->currentItem()!=0L)
    {
        int crow=ui->list_rooms->currentRow();

        QList<jpsLineItem *> walllist=datamanager->get_roomlist()[crow]->get_listWalls();

        for (int i=0; i<walllist.size(); i++)
        {
            QString string = "Wall ";
            string.append("x1:"+QString::number(walllist[i]->get_line()->line().x1()));
            string.append(" x2:"+QString::number(walllist[i]->get_line()->line().x2()));
            string.append(" y1:"+QString::number(walllist[i]->get_line()->line().y1()));
            string.append(" y2:"+QString::number(walllist[i]->get_line()->line().y2()));
            ui->listWalls->addItem(string);
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


            int cCrossingRow=ui->crossingList->currentRow();
            if (cCrossingRow!=-1) // -1 means there is no current item
            {
                QList<jpsRoom*> cRoomlist = datamanager->get_crossingList()[cCrossingRow]->get_roomList();
                if (cRoomlist.size()>0)
                {
                    int index = datamanager->get_roomlist().indexOf(cRoomlist[0]);
                    ui->roomBox1->setCurrentIndex(index);
                    index = datamanager->get_roomlist().indexOf(cRoomlist[1]);
                    ui->roomBox2->setCurrentIndex(index);
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

            datamanager->get_crossingList()[cCrossingRow]->add_rooms(datamanager->get_roomlist()[cRoomRow1],
                                                                 datamanager->get_roomlist()[cRoomRow2]);
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
    if (datamanager->get_exitList().size()>0)
    {
        ui->roomBox_exits->setEnabled(true);
        ui->exit_from->setEnabled(true);
        ui->to_label->setEnabled(true);
        ui->outside->setEnabled(true);
        ui->roomBox_exits->clear();

        QList<QString> roomNameList;
        for (int i=0; i<datamanager->get_roomlist().size(); i++)
        {
            roomNameList.push_back(datamanager->get_roomlist()[i]->get_name());
        }
        if (roomNameList.isEmpty()==false)
        {
             ui->roomBox_exits->addItems(roomNameList);

            if (ui->exitList->currentRow()!=-1)
            {
                int cExitRow=ui->exitList->currentRow();
                QList<jpsRoom*> cRoomlist = datamanager->get_exitList()[cExitRow]->get_roomList();
                if (cRoomlist.size()>0)
                {
                    int index = datamanager->get_roomlist().indexOf(cRoomlist[0]);
                    ui->roomBox_exits->setCurrentIndex(index);
                }
                else
                {
                    add_rooms_to_exit();
                }
            }
        }
    }
    else
    {
        disable_roomSelectionExits();
    }

}

void roomWidget::disable_roomSelectionExits()
{
    ui->roomBox_exits->setEnabled(false);

    ui->exit_from->setEnabled(false);
    ui->to_label->setEnabled(false);
    ui->outside->setEnabled(false);

}

void roomWidget::add_rooms_to_exit()
{

    if (ui->exitList->currentItem()!=0L)
    {
        int cExitRow=ui->exitList->currentRow();
        if (ui->roomBox_exits->currentIndex()!=-1)
        {
            int cRoomRow=ui->roomBox_exits->currentIndex();
            datamanager->get_exitList()[cExitRow]->add_rooms(datamanager->get_roomlist()[cRoomRow]);
        }
    }

}

void roomWidget::delete_exit()
{
    if (ui->exitList->currentItem()!=0L)
    {
        int index = ui->exitList->currentRow();
        datamanager->remove_exit(datamanager->get_exitList()[index]);
        ui->exitList->setCurrentRow(-1);
        this->show_exits();

    }
}

void roomWidget::select_exit()
{
    if (ui->exitList->currentItem()!=0L)
    {
        if (graphview->get_markedLines().size()>0)
        {
            graphview->unmark_all_lines();
        }
        int cExitRow=ui->exitList->currentRow();


        graphview->select_line(datamanager->get_exitList()[cExitRow]->get_cLine());

    }
}

void roomWidget::show_all()
{
    show_crossings();
    show_exits();
    show_obstacles();
    showWalls();
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

        QList<jpsLineItem *> walllist=datamanager->get_obstaclelist()[crow]->get_listWalls();

        for (int i=0; i<walllist.size(); i++)
        {
            QString string = "Wall ";
            string.append("x1:"+QString::number(walllist[i]->get_line()->line().x1()));
            string.append(" x2:"+QString::number(walllist[i]->get_line()->line().x2()));
            string.append(" y1:"+QString::number(walllist[i]->get_line()->line().y1()));
            string.append(" y2:"+QString::number(walllist[i]->get_line()->line().y2()));
            ui->listWallsObs->addItem(string);
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
    if (ui->list_rooms->currentItem()!=0L)
    {
        int cRow=ui->list_rooms->currentRow();
        QString roomName = datamanager->get_roomlist()[cRow]->get_name();
        QPointF roomCenter = datamanager->get_roomlist()[cRow]->get_center();
        return graphview->show_hide_roomCaption(roomName,roomCenter.x(),roomCenter.y());
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

















