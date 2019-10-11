/***************************************************************
 *
 * \file basiczonewidget.cpp
 * \date 2019-06-24
 * \version 0.8.9
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
 *
 * This class is for corridor, office, lobby, entrance property widget
****************************************************************/

#include "propertywidget.h"
#include "ui_propertywidget.h"

PropertyWidget::PropertyWidget(QWidget *parent, jpsDatamanager *dmanager,
                               JPSZone *zone, jpsGraphicsView *gview)
    : QWidget(parent), ui(new Ui::PropertyWidget)
{
    qDebug("Enter PropertyWidget::PropertyWidget");
    ui->setupUi(this);
    view = gview;
    data = dmanager;
    current_zone = zone;

    // Change layout depends on type
    if(zone != nullptr)
        updateWidget(zone->getType());

    // Update list widget if line deleted
    connect(view, SIGNAL(markedLineDeleted()), this, SLOT(updateListwidget()));
    connect(view, SIGNAL(lines_deleted()),this, SLOT(updateListwidget()));
    connect(parent, SIGNAL(allContentsDeleted()),this,SLOT(clearListWidget()));

    // Set-up elevation
    ui->lineEdit_elevation->setText(QString::number(current_zone->get_elevation()));

    // For wall tab
    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));
    connect(ui->pushButton_removeWall, SIGNAL(clicked()), this, SLOT(removeWallButtonClicked()));
    connect(ui->listWidget_walls, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem *)));

    // For crossing tab
    connect(ui->pushButton_addCrossing, SIGNAL(clicked()), this, SLOT(addCrossingButtonClicked()));
    connect(ui->pushButton_removeCrossing, SIGNAL(clicked()), this, SLOT(removeCrossingButtonClicked()));
    connect(ui->listWidget_crossing, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem*)));
    connect(ui->pushButton_applyOutflow, SIGNAL(clicked()), this, SLOT(applyOutflowButtonClicked()));
    connect(ui->pushButton_applyMaxAgents, SIGNAL(clicked()), this, SLOT(applyMaxagentsButtonClicked()));
    connect(ui->listWidget_crossing, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(updateCrossingInfo(QListWidgetItem*)));

    // For inspector tab
    connect(ui->pushButton_applyElevation, SIGNAL(clicked()), this, SLOT(applyElevationButtonClicked()));

    // For track tab
    connect(ui->pushButton_addTrack, SIGNAL(clicked()), this, SLOT(addTrackButtonClicked()));
    connect(ui->pushButton_removeTrack, SIGNAL(clicked()), this, SLOT(removeTrackButtonClicked()));
    connect(ui->listWidget_track, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem*)));
    connect(ui->listWidget_track, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateType(QListWidgetItem*)));
    connect(ui->pushButton_applyType, SIGNAL(clicked()), this, SLOT(applyTypeButtonClicked()));
    qDebug("Leave PropertyWidget::PropertyWidget");
}

PropertyWidget::~PropertyWidget()
{
    qDebug("Enter PropertyWidget::~PropertyWidget");
    delete ui;
    qDebug("Leave PropertyWidget::~PropertyWidget");
}

void PropertyWidget::updateWidget(ZoneType type)
{
    qDebug("Enter PropertyWidget::updateWidget");
    switch(type)
    {
        case Room:
            ui->tabWidget->removeTab(0); // Remove wall tab
            ui->tabWidget->removeTab(0); // Remove track tab
            ui->tabWidget->removeTab(0); // Remove crossing tab
            break;
        case Corridor:
            ui->tabWidget->removeTab(1); // Remove track tab
            updateWallListWidget();
            updateCrossingListWidget();
            break;
        case Platform:
            ui->tabWidget->removeTab(0); // Remove wall tab
            updateTrackListWidget();
            updateCrossingListWidget();
            break;
        case Lobby:
            ui->tabWidget->removeTab(1); // Remove track tab
            updateWallListWidget();
            updateCrossingListWidget();
            break;
        case Office:
            ui->tabWidget->removeTab(1); // Remove track tab
            updateWallListWidget();
            updateCrossingListWidget();
        case Stair:
            ui->tabWidget->removeTab(1); // Remove track tab
            updateWallListWidget();
            updateCrossingListWidget();
        default:
            qDebug("Leave PropertyWidget::updateWidget");
            return;
    }
    qDebug("Leave PropertyWidget::updateWidget");
}

void PropertyWidget::updateListwidget()
{
    qDebug("Enter PropertyWidget::updateListwidget");

    updateWallListWidget();
    updateCrossingListWidget();
    updateTrackListWidget();

}

void PropertyWidget::updateWallListWidget()
{
    qDebug("Enter PropertyWidget::updateWallListWidget");
    ui->listWidget_walls->clear();

    if(current_zone == nullptr)
        return;

    QList<jpsLineItem *> walllist = current_zone->get_listWalls();

    if(walllist.isEmpty())
    {
        qDebug("Leave PropertyWidget::updateWallListWidget");
        return;
    }

    for (int i = 0; i < walllist.size(); i++)
    {
        if(walllist[i]->get_line() != nullptr)
        {
            QString string = "";
            string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                           walllist[i]->get_line()->line().x1(),
                           walllist[i]->get_line()->line().x2(),
                           walllist[i]->get_line()->line().y1(),
                           walllist[i]->get_line()->line().y2());

            ui->listWidget_walls->addItem(string);
        }
    }

    qDebug("Leave PropertyWidget::updateWallListWidget");
}

void PropertyWidget::addWallButtonClicked()
{
    qDebug("Enter PropertyWidget::addWallButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "wall")
                current_zone->addWall(line);
        }

        updateWallListWidget();
    }
    qDebug("Leave PropertyWidget::addWallButtonClicked");
}

void PropertyWidget::removeWallButtonClicked()
{
    qDebug("Enter PropertyWidget::removeWallButtonClicked");
    int row = ui->listWidget_walls->currentRow();

    if(row == -1) // There is no rows in list
        return;

    current_zone->removeWall(current_zone->get_listWalls()[row]);

    ui->listWidget_walls->setCurrentRow(-1); // Set no focus

    updateWallListWidget();
    qDebug("Leave PropertyWidget::removeWallButtonClicked");
}

void PropertyWidget::highlightWall(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::highlightWall");
    int wRow = ui->listWidget_walls->currentRow();
    int cRow = ui->listWidget_crossing->currentRow();
    int tRow = ui->listWidget_track->currentRow();
    int index = ui->tabWidget->currentIndex();

    if(ui->tabWidget->tabText(index) == "Crossing")
    {
        auto *line = current_zone->getEnterAndExitList()[cRow];
        view->unmark_all_lines();
        view->select_line(line->get_cLine());
    }
    else if(ui->tabWidget->tabText(index) == "Wall")
    {
        auto *line= current_zone->get_listWalls()[wRow];
        view->unmark_all_lines();
        view->select_line(line);
    }
    else if(ui->tabWidget->tabText(index) == "Track")
    {
        auto *line = current_zone->getTrackList()[tRow];
        view->unmark_all_lines();
        view->select_line(line->getLine());
    }
    qDebug("Leave PropertyWidget::highlightWall");
}

void PropertyWidget::updateCrossingListWidget()
{
    qDebug("Enter PropertyWidget::updateCrossingListWidget");
    ui->listWidget_crossing->clear();

    if(current_zone == nullptr)
        return;

    QList<jpsCrossing *> crossing_list = current_zone->getEnterAndExitList();

    if(crossing_list.isEmpty())
    {
        qDebug("Leave PropertyWidget::updateCrossingListWidget");
        return;
    }

    for (int i = 0; i < crossing_list.size(); i++)
    {
        QString string = "";
        string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                       crossing_list[i]->get_cLine()->get_line()->line().x1(),
                       crossing_list[i]->get_cLine()->get_line()->line().x2(),
                       crossing_list[i]->get_cLine()->get_line()->line().y1(),
                       crossing_list[i]->get_cLine()->get_line()->line().y2());

        ui->listWidget_crossing->addItem(string);
    }

    qDebug("Leave PropertyWidget::updateCrossingListWidget");
}

void PropertyWidget::addCrossingButtonClicked()
{
    qDebug("Enter PropertyWidget::addCrossingButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "crossing" && current_zone->getFatherRoom() != nullptr)
            {
                if(current_zone->getFatherRoom()->getExitedCrossing(line) == nullptr)
                {
                    auto *crossing = new jpsCrossing(line);
                    current_zone->getFatherRoom()->addCrossing(crossing); // add into crossing list of the room
                    current_zone->addInEnterAndExitList(crossing); // add as enter of exit of the subroom
                }
                else
                {
                    // this line is already for creating crossing, use existing crossing to add
                    auto *crossing = current_zone->getFatherRoom()->getExitedCrossing(line);
                    current_zone->addInEnterAndExitList(crossing); // add as enter of exit of the subroom
                }
            }
        }
    }

    updateCrossingListWidget();
    qDebug("Leave PropertyWidget::addCrossingButtonClicked");
}

void PropertyWidget::removeCrossingButtonClicked()
{
    qDebug("Enter PropertyWidget::removeCrossingButtonClicked");
    int row = ui->listWidget_crossing->currentRow();

    if(row == -1) // There is no rows in list
        return;

    if(current_zone != nullptr && current_zone->getFatherRoom() != nullptr)
    {
        auto* crossing = current_zone->getEnterAndExitList()[row];

        current_zone->removeEnterOrExit(crossing); // Remove from enter or exit list of subroom
        current_zone->getFatherRoom()->removeCrossing(crossing); // Remove from crossing list of room

        ui->listWidget_crossing->setCurrentRow(-1); // Set no focus
    }

    updateCrossingListWidget();
    qDebug("Leave PropertyWidget::removeCrossingButtonClicked");
}

void PropertyWidget::applyElevationButtonClicked()
{
    qDebug("Enter PropertyWidget::applyElevationButtonClicked");
    float elevation = ui->lineEdit_elevation->text().toFloat();

    if(current_zone == nullptr)
        return;

    current_zone->set_elevation(elevation);

    // Setting elevation which in subrooms
    for(auto transition : data->getTransitionInSubroom(current_zone))
    {
        transition->setElevation(elevation);
    }
    qDebug("Leave PropertyWidget::applyElevationButtonClicked");
}

void PropertyWidget::updateTrackListWidget()
{
    qDebug("Enter PropertyWidget::updateTrackListWidget");
    ui->listWidget_track->clear();

    if(current_zone == nullptr)
        return;

    QList<JPSTrack *> track_list = current_zone->getTrackList();

    if(track_list.isEmpty())
    {
        qDebug("Leave PropertyWidget::updateTrackListWidget");
        return;
    }



    for (int i = 0; i < track_list.size(); i++)
    {
        QString string = "";
        string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                       track_list[i]->getLine()->get_line()->line().x1(),
                       track_list[i]->getLine()->get_line()->line().x2(),
                       track_list[i]->getLine()->get_line()->line().y1(),
                       track_list[i]->getLine()->get_line()->line().y2());

        ui->listWidget_track->addItem(string);
    }
    qDebug("Leave PropertyWidget::updateTrackListWidget");
}

void PropertyWidget::addTrackButtonClicked()
{
    qDebug("Enter PropertyWidget::addTrackButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "track")
                current_zone->addTrack(line, ui->lineEdit_Type->text());
        }
    }

    updateTrackListWidget();
    qDebug("Leave PropertyWidget::addTrackButtonClicked");
}

void PropertyWidget::removeTrackButtonClicked()
{
    qDebug("Enter PropertyWidget::removeTrackButtonClicked");
    if(ui->listWidget_track->currentItem() == nullptr)
    {
        return;
    }
    else
    {
        int row = ui->listWidget_track->currentRow();
        auto *track = current_zone->getTrackList()[row];
        current_zone->removeTrack(track);

        ui->listWidget_track->setCurrentRow(-1);

        updateTrackListWidget();
    }
    qDebug("Leave PropertyWidget::removeTrackButtonClicked");
}

void PropertyWidget::updateType(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::updateType");
    ui->lineEdit_Type->clear();

    if(current_zone == nullptr || item == nullptr)
        return;

    auto *track = current_zone->getTrackList()[ui->listWidget_track->currentRow()];
    ui->lineEdit_Type->setText(QString::number(track->getType()));
    qDebug("Leave PropertyWidget::updateType");
}

void PropertyWidget::applyTypeButtonClicked()
{
    qDebug("Enter PropertyWidget::applyTypeButtonClicked");
    int type = ui->lineEdit_Type->text().toInt();

    if(current_zone != nullptr)
        current_zone->getTrackList()[ui->listWidget_track->currentRow()]->setType(type);
    qDebug("Leave PropertyWidget::applyTypeButtonClicked");
}

void PropertyWidget::applyOutflowButtonClicked()
{
    qDebug("Enter PropertyWidget::applyOutflowButtonClicked");
    QString outflow = ui->lineEdit_outflow->text();

    if(current_zone != nullptr)
        current_zone->getEnterAndExitList()[ui->listWidget_crossing->currentRow()]->setOutflow(outflow);

    qDebug("Leave PropertyWidget::applyOutflowButtonClicked");
}

void PropertyWidget::applyMaxagentsButtonClicked()
{
    qDebug("Enter PropertyWidget::applyMaxagentsButtonClicked");
    QString maxagents = ui->lineEdit_maxAgents->text();

    if(current_zone != nullptr)
        current_zone->getEnterAndExitList()[ui->listWidget_crossing->currentRow()]->setMaxAgents(maxagents);
    qDebug("Leave PropertyWidget::applyMaxagentsButtonClicked");
}

void PropertyWidget::updateCrossingInfo(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::updateCrossingInfo");
    ui->lineEdit_outflow->clear();
    ui->lineEdit_maxAgents->clear();

    if(current_zone == nullptr || item == nullptr)
        return;

    auto *crossing = current_zone->getEnterAndExitList()[ui->listWidget_crossing->currentRow()];
    ui->lineEdit_outflow->setText(crossing->getOutflow());
    ui->lineEdit_maxAgents->setText(crossing->getMaxAgents());
    qDebug("Leave PropertyWidget::updateCrossingInfo");
}

void PropertyWidget::clearListWidget()
{
    qDebug("Enter PropertyWidget::clearListWidget");
    current_zone = nullptr;

    ui->listWidget_crossing->clear();
    ui->listWidget_track->clear();
    ui->listWidget_walls->clear();
    qDebug("Leave PropertyWidget::clearListWidget");
}

