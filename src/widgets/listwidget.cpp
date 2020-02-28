/***************************************************************
 *
 * \file listwidget.cpp
 * \date 2019-06-19
 * \version v0.8.9
 * \author Tao Zhong
 * \copyright <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section Lincense
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
 * This class define a dock widget for roomlist
 *
****************************************************************/

#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent, jpsDatamanager *dmanager, jpsGraphicsView *gview)
    : QWidget(parent), ui(new Ui::ListWidget)
{
    qDebug("Enter ListWidget::ListWidget");
    ui->setupUi(this);
    data = dmanager;
    view = gview;

    updateList();

    // Highlight room and subroom
    connect(ui->listWidget_rooms, SIGNAL(itemClicked(QListWidgetItem *)),this, SLOT(highlightRoom(QListWidgetItem *)));

    // add propertyDockWidget
    connect(this, SIGNAL(zoneSelected(JPSZone *)), parent, SLOT(addPropertyDockWidget(JPSZone *)));
    connect(this, SIGNAL(roomSelected(JPSZone *)), parent, SLOT(addPropertyDockWidget(JPSZone *)));
    connect(this, SIGNAL(zoneDeleted()), parent, SLOT(closePropertyDockWidget()));
    connect(this, SIGNAL(roomDeleted()), parent, SLOT(closePropertyDockWidget()));

    // Add
    connect(ui->pushButton_addRoom, SIGNAL(clicked()), this, SLOT(addButtonClicked()));

    // delete
    connect(ui->pushButton_deleteRoom, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));

    // Send emit to PropertyWidget
    // click room -> add room property widget
    connect(ui->listWidget_rooms, SIGNAL(itemClicked(QListWidgetItem *)),this, SLOT(selectRoom(QListWidgetItem *)));

    // Rename items
    connect(ui->listWidget_rooms, SIGNAL(itemDoubleClicked(QListWidgetItem *)),this, SLOT(renameRoom(QListWidgetItem*)));
    qDebug("Leave ListWidget::ListWidget");
}

ListWidget::~ListWidget()
{
    qDebug("Enter ListWidget::ListWidget");
    delete ui;
    qDebug("Leave ListWidget::ListWidget");
}

void ListWidget::updateList()
{
    qDebug("Enter ListWidget::updateList");
    ui->listWidget_rooms->clear();

    QList<JPSZone*> roomslist = data->getRoomlist();

    foreach(JPSZone *room, roomslist)
    {
        ui->listWidget_rooms->addItem(room->getName());
    }

    qDebug("Leave ListWidget::updateList");
}


void ListWidget::addButtonClicked()
{
    qDebug("Enter ListWidget::addButtonClicked");
    data->addRoom();
    updateList();
    qDebug("Leave ListWidget::addButtonClicked");
}

void ListWidget::selectRoom(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::selectRoom");
    if(item == nullptr)
        return;

    auto *room = getCurrentRoom(item);
    emit roomSelected(room); // emit to mainWindow
    qDebug("Leave ListWidget::selectRoom");
}


JPSZone *ListWidget::getCurrentRoom(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::getCurrentRoom");
    if(item == nullptr)
        return nullptr;

    QString name = item->text();

    QList<JPSZone*> zoneslist = data->getRoomlist();

    foreach(JPSZone *room, zoneslist)
    {
        if(name == room->getName()) // find selected room
        {
            return room;
        }
    }
    qDebug("Leave ListWidget::getCurrentRoom");
    return nullptr;
}

void ListWidget::renameRoom(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::renameRoom");
    QString name = QInputDialog::getText(this, tr("Rename"),
                                         tr("New name:"), QLineEdit::Normal,
                                         "Room");

    if(!isRepeatedRoomName(name) && getCurrentRoom(item) != nullptr)
    {
        getCurrentRoom(item)->setName(name);
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("This name is already used, change another?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Ok:
                // Ok was clicked
                break;
            default:
                // should never be reached
                break;
        }
    }

    updateList();
    qDebug("Leave ListWidget::renameRoom");
}

bool ListWidget::isRepeatedRoomName(QString name)
{
    qDebug("Enter ListWidget::isRepeatedRoomName");
    foreach(JPSZone *zone, data->getRoomlist())
    {
        if(name == zone->getName())
            return true;
    }
    qDebug("Leave ListWidget::isRepeatedRoomName");
    return false;
}

/*
    Purpose: Delete room from room list widget

    Flow: ListWidget::deleteButtonClicked
            -> jpsDatamanager::removeRoom

*/
void ListWidget::deleteButtonClicked()
{
    qDebug("Enter ListWidget::deleteButtonClicked");
    if(ui->listWidget_rooms->currentItem() != nullptr)
    {
        data->removeRoom(getCurrentRoom(ui->listWidget_rooms->currentItem())); // removing opreation in datamanager
    }
    emit roomDeleted();

    updateList();
    qDebug("Leave ListWidget::deleteButtonClicked");
}

/*
    Purpose: Delete zone from zone list widget

    Flow: ListWidget::deleteZoneButtonClicked
            -> jpsDatamanager::removeZone
*/

void ListWidget::highlightRoom(QListWidgetItem *item)
{
    qDebug("Enter oomListWidget::highlightRoom");
    if(item == nullptr)
        return;

    auto *room = getCurrentRoom(item);

    view->unmark_all_lines();
    //TODO: Fix here!
//    for(QList<JPSZone *> list : room->getZoneList())
//    {
//        for(JPSZone *zone : list)
//        {
//            if(zone != nullptr) // Mark every zones
//            {
//                if(zone->getType() == Platform)
//                {
//                    for(JPSTrack *track : zone->getTrackList())
//                    {
//                        view->markLine(track->getLine());
//                    }
//                } else
//                {
//                    for(jpsLineItem *wall : zone->get_listWalls())
//                    {
//                        view->markLine(wall);
//                    }
//                }

//                // Mark crossing
//                for(jpsCrossing *crossing : zone->getEnterAndExitList())
//                {
//                    view->markLine(crossing->get_cLine());
//                }

//                // Mark transitions in the subroom
//                for(jpsTransition *transition : data->getTransitionInSubroom(zone))
//                {
//                    view->markLine(transition->get_cLine());
//                }
//            }
//        }
//    }

    qDebug("Leave oomListWidget::highlightRoom");
}
