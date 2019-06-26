/***************************************************************
 *
 * \file platformpropertywidget.cpp
 * \date 2019-06-25
 * \version 
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
 * This class is for platform property Dockwidget
****************************************************************/

#include "platformpropertywidget.h"
#include "ui_platformpropertywidget.h"

PlatformPropertyWidget::PlatformPropertyWidget(QWidget *parent, jpsDatamanager *dmanager, jpsGraphicsView *gview)
    : QWidget(parent), ui(new Ui::PlatformPropertyWidget)
{
    ui->setupUi(this);

    data = dmanager;
    view = gview;
    current_zone = nullptr;
    current_wall = nullptr;

    updateListWidget();
    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));
}

PlatformPropertyWidget::~PlatformPropertyWidget()
{
    delete ui;
}

void PlatformPropertyWidget::addWallButtonClicked()
{
    qDebug("Enter PlatformPropertyWidget::addWallButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        current_zone->addWall(view->get_markedLines());

        updateListWidget();
    }
    qDebug("Leave PlatformPropertyWidget::addWallButtonClicked");
}

void PlatformPropertyWidget::receiveJPSZone(JPSZone *zone)
{
    qDebug("Enter PlatformPropertyWidget::receiveJPSZone");
    current_zone = zone;
    updateListWidget();
    qDebug("Leave PlatformPropertyWidget::receiveJPSZone");
}

void PlatformPropertyWidget::updateListWidget()
{
    qDebug("Enter PlatformPropertyWidget::updateListWidget");
    ui->listWidget->clear();

    if(current_zone != nullptr)
    {
        QList<jpsLineItem *> walllist = current_zone->get_listWalls();
        for (int i=0; i<walllist.size(); i++)
        {
            QString string = "";
            string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                           walllist[i]->get_line()->line().x1(),
                           walllist[i]->get_line()->line().x2(),
                           walllist[i]->get_line()->line().y1(),
                           walllist[i]->get_line()->line().y2());

            ui->listWidget->addItem(string);
        }
    }
    qDebug("Leave PlatformPropertyWidget::updateListWidget");
}

void PlatformPropertyWidget::applyNumberButtonClicked()
{
    qDebug("Enter PlatformPropertyWidget::applyNumberButtonClicked");
    int current_row=ui->listWidget->currentRow();
    current_wall = current_zone->get_listWalls()[current_row];
    qDebug("Leave PlatformPropertyWidget::applyNumberButtonClicked");
}
