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
    ui->setupUi(this);
    view = gview;
    data = dmanager;
    current_zone = zone;

    // Change layout depends on type
    updateWidget(zone->getType());

    // Update list widget if line deleted
    connect(view, SIGNAL(lines_deleted()), this, SLOT(updateListwidget()));

    // Add wall into zone
    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));
    connect(ui->pushButton_removeWall, SIGNAL(clicked()), this, SLOT(removeWallButtonClicked()));
    connect(ui->listWidget_walls, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem *)));

    // Add crossing into room
    connect(ui->pushButton_addCrossing, SIGNAL(clicked()), this, SLOT(addCrossingButtonClicked()));
    connect(ui->pushButton_removeCrossing, SIGNAL(clicked()), this, SLOT(removeCrossingButtonClicked()));
    connect(ui->listWidget_crossing, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem*)));

    // For inspector tab
    connect(ui->pushButton_applyElevation, SIGNAL(clicked()), this, SLOT(applyElevationButtonClicked()));

    // Set-up elevation
    ui->lineEdit_elevation->setText(QString::number(current_zone->get_elevation()));
}

PropertyWidget::~PropertyWidget()
{
    delete ui;
}

void PropertyWidget::updateWidget(ZoneType type)
{
    switch(type)
    {
        case Room:
            ui->tabWidget->removeTab(1); // Remove wall tab
            updateCrossingListWidget();
            break;
        case Corridor:
            ui->tabWidget->removeTab(0);// Remove crossing tab
            updateWallListWidget();
            break;
        case Platform:
            ui->tabWidget->removeTab(0);// keep crossing tab
            break;
        default:
            return;
    }
}

void PropertyWidget::updateListwidget()
{
    qDebug("Enter PropertyWidget::updateListwidget");
    updateWallListWidget();
    updateCrossingListWidget();
    qDebug("Leave PropertyWidget::updateListwidget");
}

void PropertyWidget::updateWallListWidget()
{
    qDebug("Enter PropertyWidget::updateWallListWidget");
    ui->listWidget_walls->clear();

    if(current_zone == nullptr)
        return;

    QList<jpsLineItem *> walllist = current_zone->get_listWalls();

    for (int i = 0; i < walllist.size(); i++)
    {
        QString string = "";
        string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                       walllist[i]->get_line()->line().x1(),
                       walllist[i]->get_line()->line().x2(),
                       walllist[i]->get_line()->line().y1(),
                       walllist[i]->get_line()->line().y2());

        ui->listWidget_walls->addItem(string);
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

    jpsLineItem *wall = current_zone->get_listWalls()[row];

    current_zone->removeWall(wall);

    ui->listWidget_walls->setCurrentRow(-1); // Set no focus

    updateWallListWidget();
    qDebug("Leave PropertyWidget::removeWallButtonClicked");
}

void PropertyWidget::highlightWall(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::highlightWall");
    int wRow = ui->listWidget_walls->currentRow();
    int cRow = ui->listWidget_crossing->currentRow();
    int index = ui->tabWidget->currentIndex();

    if(ui->tabWidget->tabText(index) == "Crossing")
    {
        auto *line = current_zone->getCrossingList()[cRow];
        view->select_line(line->get_cLine());
    }
    else if(ui->tabWidget->tabText(index) == "Wall")
    {
        auto *line= current_zone->get_listWalls()[wRow];
        view->select_line(line);
    }
    qDebug("Leave PropertyWidget::highlightWall");
}

void PropertyWidget::updateCrossingListWidget()
{
    qDebug("Enter PropertyWidget::updateCrossingListWidget");
    ui->listWidget_crossing->clear();

    if(current_zone == nullptr)
        return;

    QList<jpsCrossing *> crossing_list = current_zone->getCrossingList();

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
            if(line->getType() == "crossing")
                current_zone->addCrossing(line);
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

    auto* crossing = current_zone->getCrossingList()[row];

    current_zone->removeCrossing(crossing);

    ui->listWidget_crossing->setCurrentRow(-1); // Set no focus

    updateCrossingListWidget();
    qDebug("Leave PropertyWidget::removeCrossingButtonClicked");
}

void PropertyWidget::applyElevationButtonClicked()
{
    qDebug("Enter PropertyWidget::applyElevationButtonClicked");
    float elevation = ui->lineEdit_elevation->text().toFloat();

    current_zone->set_elevation(elevation);
    qDebug("Leave PropertyWidget::applyElevationButtonClicked");
}
