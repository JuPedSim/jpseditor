/**
 * \file        widgetlandmark.cpp
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
 * With the help of this class the user is able to handle landmarks and set associations (waypoints) relying on a certain landmark.
 * widgetlandmark is not enabled in v0.8.1.
 *
 *
 **/

#include "widgetlandmark.h"
#include "ui_widgetlandmark.h"


#include <iostream>
#include <cmath>

widgetLandmark::widgetLandmark(QWidget *parent, jpsDatamanager *dmanager, jpsGraphicsView *gview):
    QTabWidget(parent),
    ui(new Ui::widgetLandmark)
{
    ui->setupUi(this);
    _dmanager=dmanager;
    _gview=gview;

    //Landmark type
    ui->Box_landmarkType->addItem("Landmark");
    ui->Box_landmarkType->addItem("Main Target");


    show_landmarks();

    connect(ui->apply_name_button,SIGNAL(clicked(bool)),this,SLOT(change_name()));
    connect(ui->closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->closeButton_2,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->closeButton_regions,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->cogmap_closeButton,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(define_landmark()));
    connect(ui->list_landmarks,SIGNAL(activated(int)),this,SLOT(enable_room_selection()));
    connect(ui->list_landmarks,SIGNAL(activated(int)),this,SLOT(ShowLandmarkType()));
    connect(ui->list_landmarks,SIGNAL(currentIndexChanged(int)),_gview,SLOT(unmarkLandmark()));
    connect(ui->roomBox_landmarks,SIGNAL(activated(int)),this,SLOT(add_room_to_landmark()));
    connect(ui->add_button,SIGNAL(clicked(bool)),_gview,SLOT(StatPositionDef()));
    connect(_gview,SIGNAL(PositionDefCompleted()),this,SLOT(SetPosInCMap()));
    connect(ui->remove_button,SIGNAL(clicked(bool)),this,SLOT(RemoveAssociation()));
    connect(ui->button_showhide,SIGNAL(clicked(bool)),this,SLOT(ShowHideLandmark()));
    connect(ui->Box_landmarkType,SIGNAL(activated(int)),this,SLOT(SetLandmarkType()));

    //connectionDef
    connect(ui->add_button_connections,SIGNAL(clicked(bool)),this,SLOT(NewConnection()));
    connect(_gview,SIGNAL(DefConnection1Completed()),this,SLOT(AskForSecondLandmark()));
    connect(_gview,SIGNAL(DefConnection2Completed()),this,SLOT(SetLandmarksToConnection()));
    connect(ui->remove_button_connections,SIGNAL(clicked(bool)),this,SLOT(RemoveConnection()));

    //Region Def
    connect(ui->add_button_regions,SIGNAL(clicked(bool)),_gview,SLOT(ChangeRegionStatDef()));
    connect(_gview,SIGNAL(RegionDefCompleted()),this,SLOT(NewRegion()));
    connect(ui->remove_button_regions,SIGNAL(clicked(bool)),this,SLOT(RemoveRegion()));
    connect(ui->box_regions,SIGNAL(activated(int)),this,SLOT(SetLandmarkToRegion()));
    connect(ui->button_showhide_region,SIGNAL(clicked(bool)),this,SLOT(ShowHideRegion()));

    //saveCogMap
    connect(ui->save_button_cogmap,SIGNAL(clicked(bool)),this->parentWidget(),SLOT(SaveCogMapXML()));
    connect(ui->save_button_multiple_maps,SIGNAL(clicked(bool)),this,SLOT(CreateSimilarMaps()));

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
    ShowLandmarkType();
    ShowRegionBox();
    ShowRegions();

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
        QPen pen = QPen(Qt::blue,2);
        pen.setCosmetic(true);
        QGraphicsEllipseItem* ellipse = _gview->GetScene()->addEllipse(landmark->GetRect(),pen);
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

void widgetLandmark::ShowLandmarkType()
{
   jpsLandmark* landmark = GetCurrentLandmark();

   if (landmark!=nullptr)
   {
       if (landmark->GetType()=="Landmark")
        ui->Box_landmarkType->setCurrentIndex(0);
       else
           ui->Box_landmarkType->setCurrentIndex(1);
   }
}

void widgetLandmark::SetLandmarkType()
{
    jpsLandmark* landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        landmark->SetType(ui->Box_landmarkType->currentText());
    }
}

void widgetLandmark::ShowRegionBox()
{
    if (_dmanager->GetRegions().empty())
        return;

    for (jpsRegion* region:_dmanager->GetRegions())
    {
        ui->box_regions->addItem(region->GetCaption()); 
    }

    jpsLandmark* landmark = GetCurrentLandmark();
    QString cRegion;
    if (landmark!=nullptr)
    {
        if (landmark->GetRegion()!=nullptr)

            cRegion = landmark->GetRegion()->GetCaption();
            for (int i=0; i<ui->box_regions->count(); ++i)
            {
                if (ui->box_regions->itemText(i)==cRegion)
                {
                    ui->box_regions->setCurrentIndex(i);
                    break;
                }
            }
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
                _currentConnection->SetFirstLandmark(landmark);
                landmark->NewConnection(_currentConnection);
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
                _currentConnection->SetSecondLandmark(landmark);
                landmark->NewConnection(_currentConnection);
                _gview->SetStatDefConnections(0);
                AddConnectionsToWidget();
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
    lineItem->setTransform(QTransform::fromTranslate(_gview->GetTranslationX(),_gview->GetTranslationY()), true);
    _currentConnection->SetLineItem(lineItem);
    _currentConnection=nullptr;
}

void widgetLandmark::AddConnectionsToWidget()
{
    QString string = _currentConnection->GetLandmarks().first->GetCaption()+" <-> "+_currentConnection->GetLandmarks().second->GetCaption();
    ui->listWidgetConnections->addItem(string);

}

void widgetLandmark::RemoveConnection()
{
    if (ui->listWidgetConnections->currentRow()!=-1)
    {
        int currentRow = ui->listWidgetConnections->currentRow();
        QListWidgetItem* item = ui->listWidgetConnections->takeItem(currentRow);
        delete item;
        _dmanager->RemoveConnection(_dmanager->GetAllConnections()[currentRow]);
    }
}

void widgetLandmark::NewRegion()
{
    ui->add_button->setChecked(false);
    // show ellipse and text in graphics view
    QPen pen = QPen(Qt::darkGreen,2);
    pen.setCosmetic(true);
    QRectF rect = _gview->GetCurrentSelectRect()->rect();
    QGraphicsEllipseItem* ellipse = _gview->GetScene()->addEllipse(rect,pen);
    ellipse->setTransform(QTransform::fromTranslate(_gview->GetTranslationX(),_gview->GetTranslationY()), true);


    // create region
    jpsRegion* region = new jpsRegion(_dmanager->GetRegionCounter(),"Region"+QString::number(_dmanager->GetRegionCounter()),
                                      rect.center(),std::fabs(rect.width()/2.0),std::fabs(rect.height()/2.0));


    QGraphicsTextItem* text = _gview->GetScene()->addText(region->GetCaption());
    text->setPos(rect.center().x()+_gview->GetTranslationX(),rect.center().y()+_gview->GetTranslationY());
    //text->setScale(gl_scale_f);
    text->setData(0,_gview->GetScaleF());
    text->setTransform(QTransform::fromScale(_gview->GetScaleF(),-_gview->GetScaleF()),true);

    region->SetTextItem(text);
    region->SetEllipse(ellipse);
    _dmanager->NewRegion(region);

    //show Region in listwidget
    ShowRegions();
}

void widgetLandmark::RemoveRegion()
{
    int row = ui->listWidgetRegions->currentRow();
    if (row!=-1)
    {
        _dmanager->RemoveRegion(_dmanager->GetRegions()[row]);
        QListWidgetItem* item = ui->listWidgetRegions->takeItem(row);
        delete item;
    }
}

void widgetLandmark::SetLandmarkToRegion()
{
    jpsLandmark* landmark = GetCurrentLandmark();

    if (landmark!=nullptr)
    {
        int row = ui->box_regions->currentIndex();

        if (row!=-1)
        {
            for (jpsRegion* region:_dmanager->GetRegions())
            {
                region->RemoveLandmark(landmark);
            }
            jpsRegion* cRegion = _dmanager->GetRegions()[row];
            landmark->SetRegion(cRegion);
            cRegion->AddLandmark(landmark);
        }
    }

}

void widgetLandmark::ShowHideRegion()
{
    jpsRegion* region = GetCurrentRegion();

    if (region!=nullptr)
    {
        if (region->GetEllipseItem()->isVisible())
        {
            region->GetEllipseItem()->setVisible(false);
            region->GetTextItem()->setVisible(false);
        }
        else
        {
            region->GetEllipseItem()->setVisible(true);
            region->GetTextItem()->setVisible(true);
        }
    }
}

void widgetLandmark::ShowRegions()
{
    ui->listWidgetRegions->clear();
    for (jpsRegion* region:_dmanager->GetRegions())
    {

        QString string = "x: "+QString::number(region->GetPos().x())
                + " y: "+QString::number(region->GetPos().y())+" rA: "+QString::number(region->GetA())
                                         + " rB: "+QString::number(region->GetB());
        ui->listWidgetRegions->addItem(string);
    }
}

void widgetLandmark::CreateSimilarMaps()
{
    int numberMaps = ui->spinBox_numberMaps->value();

    int k=1;

    for (int i=1; i<=numberMaps; ++i)
    {
        double m = 0.3;
        if (i==int(5.01/47.0*numberMaps) || i==int(8.01/47.0*numberMaps) || i==int(11.01/47.0*numberMaps) || i==int(15.01/47.0*numberMaps) || i==int(17.01/47.0*numberMaps) || i==int(18.01/47.0*numberMaps) || i==int(25.01/47.0*numberMaps) || i==int(38.01/47.0*numberMaps) || i==int(43.01/47.0*numberMaps))
            ++k;
        for (size_t a=1; a<=1; ++a)
        {
            double p0=-7.0*m;
            //int id=0;
            for (size_t b=1; b<=10; ++b)
            {
                _dmanager->CreateAndSaveASimilarCogMap(i,k,m,p0);

                p0+=1.0/9.0;
                //++id;
            }

            m+=0.025;
        }
    }
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

jpsRegion *widgetLandmark::GetCurrentRegion() const
{
    if (ui->listWidgetRegions->currentRow()!=-1)
    {
        int crow=ui->listWidgetRegions->currentRow();

        return _dmanager->GetRegions()[crow];
    }
    else
        return nullptr;
}
