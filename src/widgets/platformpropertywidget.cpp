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
    // Active track number line
    connect(ui->comboBox_lineType, SIGNAL(currentIndexChanged(int)), this, SLOT(activeTrackNumber(int)));
    // Change details of line
    connect(ui->pushButton_applyNumber, SIGNAL(clicked()), this, SLOT(applyNumberButtonClicked()));
    // Show details of line
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT((updateLineDetails(int))));

    // for wall there is no number property
    ui->lineEdit->setEnabled(false);
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

    if(current_zone != nullptr)
    {
        current_wall = current_zone->get_listWalls()[current_row];
    }

    if(ui->comboBox_lineType->currentIndex() == 1)
    {
        QString track_number = ui->lineEdit->text();
        current_wall->setType(track); //see all types in global.h
        current_wall->setTrackNumber(track_number);
    } else
    {
        current_wall->setType(wall);
    }



    qDebug("Leave PlatformPropertyWidget::applyNumberButtonClicked");
}

void PlatformPropertyWidget::activeTrackNumber(int type)
{
    qDebug("Enter PlatformPropertyWidget::activeTrackNumber");
    if(type == 1)// 1 means track type
    {
        ui->lineEdit->setEnabled(true);
    }
    qDebug("Leave PlatformPropertyWidget::activeTrackNumber");
}

void PlatformPropertyWidget::updateLineDetails(int index)
{
    qDebug("Enter PlatformPropertyWidget::updateLineDetails");
    if(current_zone != nullptr)
    {
        current_wall = current_zone->get_listWalls()[index];
    }

    if(current_wall->getType() == "track")
    {
        ui->comboBox_lineType->setCurrentIndex(1);
        ui->lineEdit->setText(current_wall->getTrackNumber());
    } else
    {
        ui->comboBox_lineType->setCurrentIndex(0);
        ui->lineEdit->setEnabled(false);
    }

    qDebug("Leave PlatformPropertyWidget::updateLineDetails");
}
