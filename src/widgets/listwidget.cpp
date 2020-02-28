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

    zoneType = NotAssigned;

    updateList();

    // Highlight room and subroom
    connect(ui->listWidget_zones, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlight(QListWidgetItem * )));

    // add propertyDockWidget
    connect(this, SIGNAL(zoneSelected(JPSZone *)), parent, SLOT(addPropertyDockWidget(JPSZone *)));
    connect(this, SIGNAL(zoneDeleted()), parent, SLOT(closePropertyDockWidget()));

    // Add
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addButtonClicked()));

    // delete
    connect(ui->pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));

    // Send emit to PropertyWidget
    // click room -> add room property widget
    connect(ui->listWidget_zones, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(select(QListWidgetItem * )));

    // Rename items
    connect(ui->listWidget_zones, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(rename(QListWidgetItem * )));
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
    ui->listWidget_zones->clear();

    QList<JPSZone*> list = getCurrentList(zoneType);

    foreach(JPSZone *zone, list)
    {
        ui->listWidget_zones->addItem(zone->getName());
    }

    qDebug("Leave ListWidget::updateList");
}


void ListWidget::addButtonClicked()
{
    qDebug("Enter ListWidget::addButtonClicked");
    switch(zoneType){
        case Room:
            data->addRoom();
            break;
        case Stair:
            data->addStair();
            break;
        case Platform:
            data->addPlatform();
            break;
        default:
            break;
    }

    updateList();
    qDebug("Leave ListWidget::addButtonClicked");
}

void ListWidget::select(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::select");
    if(item == nullptr)
        return;

    auto *zone = getCurrent(item);

    emit zoneSelected(zone); // emit to mainWindow
    qDebug("Leave ListWidget::select");
}


JPSZone *ListWidget::getCurrent(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::getCurrent");
    if(item == nullptr)
        return nullptr;

    QString name = item->text();

    QList<JPSZone*> list = getCurrentList(zoneType);

    foreach(JPSZone *zone, list)
    {
        if(name == zone->getName()) // find selected room
        {
            return zone;
        }
    }
    qDebug("Leave ListWidget::getCurrent");
    return nullptr;
}

void ListWidget::rename(QListWidgetItem *item)
{
    qDebug("Enter ListWidget::rename");
    QString name = QInputDialog::getText(this, tr("Rename"),
                                         tr("New name:"), QLineEdit::Normal,
                                         "Room");

    if(!isRepeatedName(name) && getCurrent(item) != nullptr)
    {
        getCurrent(item)->setName(name);
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
    qDebug("Leave ListWidget::rename");
}

bool ListWidget::isRepeatedName(QString name)
{
    qDebug("Enter ListWidget::isRepeatedName");
    foreach(JPSZone *zone, data->getRoomlist())
    {
        if(name == zone->getName())
            return true;
    }
    qDebug("Leave ListWidget::isRepeatedName");
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
    if(ui->listWidget_zones->currentItem() != nullptr)
    {
        switch(zoneType){
            // removing opreation in datamanager
            case Room:
                data->removeRoom(getCurrent(ui->listWidget_zones->currentItem()));
                break;
            case Stair:
                data->removeStair(getCurrent(ui->listWidget_zones->currentItem()));
                break;
            case Platform:
                data->removePlatform(getCurrent(ui->listWidget_zones->currentItem()));
                break;
            default:
                break;
        }
    }

    emit zoneDeleted();

    updateList();
    qDebug("Leave ListWidget::deleteButtonClicked");
}

/*
    Purpose: Delete zone from zone list widget

    Flow: ListWidget::deleteZoneButtonClicked
            -> jpsDatamanager::removeZone
*/

void ListWidget::highlight(QListWidgetItem *item)
{
    qDebug("Enter oomListWidget::highlight");
    if(item == nullptr)
        return;

    auto *room = getCurrent(item);

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

    qDebug("Leave oomListWidget::highlight");
}
ZoneType ListWidget::getZoneType() const
{
    qDebug("Enter/Leave ListWidget::getZoneType()");
    return zoneType;
}
void ListWidget::setZoneType(ZoneType zoneType)
{
    qDebug("Enter ListWidget::setZoneType");
    ListWidget::zoneType = zoneType;
    updateList();
    qDebug("Leave ListWidget::setZoneType");
}

const QList<JPSZone *> &ListWidget::getCurrentList(ZoneType type) const
{
    qDebug("Enter ListWidget::getCurrentList");
    if(type == Room)
    {
        const QList<JPSZone*> &roomlist = data->getRoomlist();
        qDebug("Leave ListWidget::getCurrentList");
        return roomlist;
    }
    else if(type == Stair) {
        const QList<JPSZone*> &stairlist = data->getStair_list();
        qDebug("Leave ListWidget::getCurrentList");
        return stairlist;
    }
    else if(type == Platform) {
        const QList<JPSZone*> &platformlist = data->getPlatform_list();
        qDebug("Leave ListWidget::getCurrentList");
        return platformlist;
    }
    else{
        const QList<JPSZone*> &list = QList<JPSZone *>();
        qDebug("Leave ListWidget::getCurrentList");
        return list;
    }
}
