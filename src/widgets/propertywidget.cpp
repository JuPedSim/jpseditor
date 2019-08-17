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

    // Add wall into zone
    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));

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
            ui->tabWidget->removeTab(0); // keep only crossing tab
            break;
        case Corridor:
            ui->tabWidget->removeTab(1); // keep only wall tab
            updateWallListWidget();
            break;
        case Platform:
            break;
        default:
            return;
    }

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