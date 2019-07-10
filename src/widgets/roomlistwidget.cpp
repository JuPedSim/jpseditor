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

    connect(ui->listWidget_rooms, SIGNAL(itemSelectionChanged()), this, SLOT(updateZonesListWidget()));
    connect(ui->pushButton_addRoom, SIGNAL(clicked()), this, SLOT(addRoomButtonClicked()));
    connect(ui->pushButton_addZone, SIGNAL(clicked()), this, SLOT(addZoneButtonClicked()));

    // send emit to PropertyWidget
    connect(ui->listWidget_zones, SIGNAL(itemSelectionChanged()), this, SLOT(currentZoneChanged()));
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

    QList<JPSZone*> roomslist = data->getRoomslist();

    foreach(JPSZone *room, roomslist)
    {
        ui->listWidget_rooms->addItem(room->get_name());
    }

    qDebug("Leave RoomListWidget::updateRoomsListWidget");
}

void RoomListWidget::updateZonesListWidget()
{
    qDebug("Enter RoomListWidget::updateZonesListWidget");
    ui->listWidget_zones->clear();

    ZoneType type = data->convertToZoneType(ui->label_zone->text());
    QList<JPSZone*> zoneslist;

    if(type == ZoneType::Platform)
    {
        zoneslist = data->getPlatformslist();
    }

    // show zones
    foreach(JPSZone *zone, zoneslist)
    {
        if(zone->getFatherZone()->getName()==ui->listWidget_rooms->currentItem()->text())
        {
            ui->listWidget_zones->addItem(zone->get_name());
        }
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
        QString father_zone_name = ui->listWidget_rooms->currentItem()->text();
        ZoneType type = data->convertToZoneType(ui->label_zone->text());

        foreach(JPSZone *room, data->getRoomslist())
        {
            if(room->getName() == father_zone_name) // find selected room
            {
                if(type == ZoneType::Platform)
                {
                    data->addPlatform(room);
                }
            }
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Add or select a room at first!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    updateZonesListWidget();
    qDebug("Leave RoomListWidget::addZoneButtonClicked");
}

void RoomListWidget::currentZoneChanged()
{
    qDebug("Enter currentZoneChanged");
    auto *zone = getCurrentZone(ui->listWidget_zones->currentItem());
    emit zoneSelected(zone);
    qDebug("Leave currentZoneChanged");
}

JPSZone *RoomListWidget::getCurrentZone(QListWidgetItem *item)
{
    QString name = item->text();

    ZoneType type = data->convertToZoneType(ui->label_zone->text());
    QList<JPSZone*> zoneslist;

    if(type == ZoneType::Platform)
    {
        zoneslist = data->getPlatformslist();
    }

    foreach(JPSZone *zone, zoneslist)
    {
        if(name == zone->getName()) // find selected room
        {
            return zone;
        }
    }

}
