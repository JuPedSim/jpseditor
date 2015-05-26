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

    show_landmarks();


    connect(ui->apply_name_button,SIGNAL(clicked(bool)),this,SLOT(change_name()));
    connect(ui->closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->list_landmarks,SIGNAL(activated(int)),this,SLOT(enable_room_selection()));
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
        ///mark Landmark
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
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int crow=ui->list_landmarks->currentIndex();

            _dmanager->get_landmarks()[crow]->set_name(ui->chname_edit->text());

        this->show_landmarks();
    }
}

void widgetLandmark::AddAssociation()
{
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int cLanRow=ui->list_landmarks->currentIndex();

        _dmanager->get_landmarks()[cLanRow]->AddWaypoint(new jpsWaypoint(_gview->GetCurrentSelectRect()->rect()));
        ShowAssociations();
    }
}

void widgetLandmark::ShowAssociations()
{
    ui->listWaypoints->clear();
    if (ui->list_landmarks->currentIndex()!=-1)
    {
        int cLanRow=ui->list_landmarks->currentIndex();
        QList<jpsWaypoint *> waypoints = _dmanager->get_landmarks()[cLanRow]->GetWaypoints();

        for (jpsWaypoint* waypoint:waypoints)
        {
            QPointF pos = waypoint->GetPos();
            QString name = "Waypoint: x: "+QString::number(pos.x())+"y: "+QString::number(pos.y());
            ui->listWaypoints->addItem(name);

        }
        _gview->ShowWaypoints(waypoints);
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
