/***************************************************************
 *
 * \file roomlistwidget.cpp
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

#include "roomlistwidget.h"
#include "ui_roomlistwidget.h"

RoomListWidget::RoomListWidget(QWidget *parent, jpsDatamanager *dmanager)
    : QWidget(parent), ui(new Ui::RoomListWidget)
{
    ui->setupUi(this);
    data = dmanager;

    updateRoomsListWidget();

    // add propertyDockWidget
    connect(this, SIGNAL(zoneSelected(JPSZone *)), parent, SLOT(addPropertyDockWidget(JPSZone *)));

    connect(this, SIGNAL(roomSelected(JPSZone *)), parent, SLOT(addPropertyDockWidget(JPSZone *)));
    // Add
    connect(ui->pushButton_addRoom, SIGNAL(clicked()), this, SLOT(addRoomButtonClicked()));
    connect(ui->pushButton_addZone, SIGNAL(clicked()), this, SLOT(addZoneButtonClicked()));

    // delete
    connect(ui->pushButton_deleteRoom, SIGNAL(clicked()), this, SLOT(deleteRoomButtonClicked()));
    connect(ui->pushButton_deleteZone, SIGNAL(clicked()), this, SLOT(deleteZoneButtonClicked()));

    // Send emit to PropertyWidget
    // click room -> update zone list
    connect(ui->listWidget_rooms, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(updateZonesListWidget(QListWidgetItem *)));
    // click room -> add room property widget
    connect(ui->listWidget_rooms, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(selectRoom(QListWidgetItem *)));
    // click room -> add zone property widget
    connect(ui->listWidget_zones, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(selectZone(QListWidgetItem *)));

    // Rename items
    connect(ui->listWidget_rooms, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(renameRoom(QListWidgetItem*)));
    connect(ui->listWidget_zones, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(renameZone(QListWidgetItem*)));
}

RoomListWidget::~RoomListWidget()
{
    delete ui;
}

void RoomListWidget::setLabel(QString name)
{
    ui->label_zone->setText(name);
}

void RoomListWidget::updateRoomsListWidget()
{
    qDebug("Enter RoomListWidget::updateRoomsListWidget");
    ui->listWidget_rooms->clear();

    QList<JPSZone*> roomslist = data->getRoomlist();

    foreach(JPSZone *room, roomslist)
    {
        ui->listWidget_rooms->addItem(room->get_name());
    }

    qDebug("Leave RoomListWidget::updateRoomsListWidget");
}

void RoomListWidget::updateZonesListWidget(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::updateZonesListWidget");
    ui->listWidget_zones->clear();

    ZoneType type = data->convertToZoneType(ui->label_zone->text());
    QList<JPSZone*> zoneslist;

    // If current room is deleted, just show empty zone list
    if(item == nullptr || getCurrentRoom(item) == nullptr)
        return;

    // Get right list
    switch (type)
    {
        case Corridor:
            zoneslist = getCurrentRoom(item)->getCorridorList();
            break;
        case Platform:
            zoneslist = getCurrentRoom(item)->getPlatfromList();
            break;
        default:
            break;
    }

    // Show zones
    foreach(JPSZone *zone, zoneslist)
    {
        ui->listWidget_zones->addItem(zone->get_name());
    }
    qDebug("Leave RoomListWidget::updateZonesListWidget");
}

void RoomListWidget::addRoomButtonClicked()
{
    qDebug("Enter RoomListWidget::addRoomButtonClicked");
    data->addRoom();
    updateRoomsListWidget();
    qDebug("Leave RoomListWidget::addRoomButtonClicked");
}

void RoomListWidget::addZoneButtonClicked()
{
    qDebug("Enter RoomListWidget::addZoneButtonClicked");
    if(ui->listWidget_rooms->currentItem() != nullptr)
    {
        ZoneType type = data->convertToZoneType(ui->label_zone->text());

        switch (type)
        {
            case Corridor:
                data->addCorridor(getCurrentRoom(ui->listWidget_rooms->currentItem()));
                break;
            case Platform:
                data->addPlatform(getCurrentRoom(ui->listWidget_rooms->currentItem()));
                break;
            default:
                break;
        }
    }

    else // Show warning message
    {
        QMessageBox msgBox;
        msgBox.setText("Add or select a room at first!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    updateZonesListWidget(ui->listWidget_rooms->currentItem());
    qDebug("Leave RoomListWidget::addZoneButtonClicked");
}

void RoomListWidget::selectRoom(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::selectRoom");
    if(item == nullptr)
        return;

    auto *room = getCurrentRoom(item);
    emit roomSelected(room); // emit to mainWindow
    qDebug("Leave RoomListWidget::selectRoom");
}

void RoomListWidget::selectZone(QListWidgetItem *item)
{
    qDebug("Enter selectZone");
    if(item == nullptr)
        return;;

    auto *zone = getCurrentZone(item);
    emit zoneSelected(zone);
    qDebug("Leave selectZone");
}

JPSZone *RoomListWidget::getCurrentZone(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::getCurrentZone");
    QString name = item->text();

    ZoneType type = data->convertToZoneType(ui->label_zone->text());
    QList<JPSZone*> zoneslist;

    switch(type)
    {
        case Corridor:
            zoneslist = getCurrentRoom(ui->listWidget_rooms->currentItem())->getCorridorList();
            break;
        case Platform:
            zoneslist = getCurrentRoom(ui->listWidget_rooms->currentItem())->getPlatfromList();
            break;
        default:
            return nullptr;
    }

    if(!zoneslist.isEmpty())
    {
        foreach(JPSZone *zone, zoneslist)
        {
            if(name == zone->getName()) // find selected zone
            {
                qDebug("Found current zone. Leave RoomListWidget::getCurrentZone");
                return zone;
            }
        }
    }

    return nullptr;
}


JPSZone *RoomListWidget::getCurrentRoom(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::getCurrentRoom");
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
    qDebug("Leave RoomListWidget::getCurrentRoom");
    return nullptr;
}

void RoomListWidget::renameRoom(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::renameRoom");
    QString name = QInputDialog::getText(this, tr("Rename"),
                                         tr("New name:"), QLineEdit::Normal,
                                         "Room");

    if(!isRepeatedRoomName(name) && getCurrentRoom(item) != nullptr)
    {
        getCurrentRoom(item)->setName(name);
    } else
    {
        QMessageBox::warning(this,"Warning!", "This name is already used, change another?",
                             QMessageBox::Ok);
    }

    updateRoomsListWidget();
    qDebug("Leave RoomListWidget::renameRoom");
}

bool RoomListWidget::isRepeatedRoomName(QString name)
{
    qDebug("Enter RoomListWidget::isRepeatedRoomName");
    foreach(JPSZone *zone, data->getRoomlist())
    {
        if(name == zone->getName())
            return true;
    }
    qDebug("Leave RoomListWidget::isRepeatedRoomName");
    return false;
}

void RoomListWidget::renameZone(QListWidgetItem *item)
{
    qDebug("Enter RoomListWidget::renameZone");
    QString name = QInputDialog::getText(this, tr("Rename Zone"),
                                         tr("New name:"), QLineEdit::Normal,
                                         "Zone");

    if(!isRepeatedZoneName(name) && getCurrentZone(item) != nullptr)
    {
        getCurrentZone(item)->setName(name);
    } else
    {
        QMessageBox::warning(this,"Warning!", "This name is already used, change another?",
                                 QMessageBox::Ok);
    }

    updateZonesListWidget(ui->listWidget_rooms->currentItem());
    qDebug("Leave RoomListWidget::renameZone");
}

bool RoomListWidget::isRepeatedZoneName(QString name)
{
    qDebug("Enter RoomListWidget::isRepeatedZoneName");
    ZoneType type = data->convertToZoneType(ui->label_zone->text());
    QList<JPSZone*> zoneslist;

    switch(type)
    {
        case Corridor:
            zoneslist = getCurrentRoom(ui->listWidget_rooms->currentItem())->getCorridorList();
            break;
        case Platform:
            zoneslist = getCurrentRoom(ui->listWidget_rooms->currentItem())->getPlatfromList();
            break;
        default:
            return false;
    }

    foreach(JPSZone *zone, zoneslist)
    {
        if(name == zone->getName())
            return true;
    }
    qDebug("Leave RoomListWidget::isRepeatedZoneName");
    return false;
}

/*
    Purpose: Delete room from room list widget

    Flow: RoomListWidget::deleteRoomButtonClicked
            -> jpsDatamanager::removeRoom

*/
void RoomListWidget::deleteRoomButtonClicked()
{
    qDebug("Enter RoomListWidget::deleteRoomButtonClicked");
    if(ui->listWidget_rooms->currentItem() != nullptr)
    {
        data->removeRoom(getCurrentRoom(ui->listWidget_rooms->currentItem())); // removing opreation in datamanager
    }

    updateRoomsListWidget();
    qDebug("Leave RoomListWidget::deleteRoomButtonClicked");
}


/*
    Purpose: Delete zone from zone list widget

    Flow: RoomListWidget::deleteZoneButtonClicked
            -> jpsDatamanager::removeZone
*/
void RoomListWidget::deleteZoneButtonClicked()
{
    qDebug("Enter RoomListWidget::deleteZoneButtonClicked");
    if(ui->listWidget_zones->currentItem() != nullptr)
    {
        data->removeZone(getCurrentRoom(ui->listWidget_rooms->currentItem()),
                getCurrentZone(ui->listWidget_zones->currentItem()));
    }

    updateZonesListWidget(ui->listWidget_rooms->currentItem());
    qDebug("Leave RoomListWidget::deleteZoneButtonClicked");
}


