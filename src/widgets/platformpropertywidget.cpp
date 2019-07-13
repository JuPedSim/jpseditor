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

    updateWallListWidget();
    updateTrackListWidget();

    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));
    // Change details of line
    connect(ui->pushButton_applyNumber, SIGNAL(clicked()), this, SLOT(applyNumberButtonClicked()));
    // Add track into Platform
    connect(ui->pushButton_addTrack,  SIGNAL(clicked()), this, SLOT(addTrackButtonClicked()));
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
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "wall")
                current_zone->addWall(line);
        }

        updateWallListWidget();
    }
    qDebug("Leave PlatformPropertyWidget::addWallButtonClicked");
}

void PlatformPropertyWidget::receiveJPSZone(JPSZone *zone)
{
    qDebug("Enter PlatformPropertyWidget::receiveJPSZone");
    current_zone = zone;
    updateWallListWidget();
    qDebug("Leave PlatformPropertyWidget::receiveJPSZone");
}

void PlatformPropertyWidget::updateWallListWidget()
{
    qDebug("Enter PlatformPropertyWidget::updateWallListWidget");
    ui->listWidget_walls->clear();

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

            ui->listWidget_walls->addItem(string);
        }
    }
    qDebug("Leave PlatformPropertyWidget::updateWallListWidget");
}

void PlatformPropertyWidget::applyNumberButtonClicked()
{
    qDebug("Enter PlatformPropertyWidget::applyNumberButtonClicked");
    qDebug("Leave PlatformPropertyWidget::applyNumberButtonClicked");
}

void PlatformPropertyWidget::addTrackButtonClicked()
{
    qDebug("Enter PlatformPropertyWidget::addTrackButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *marked_line, view->get_markedLines())
        {
            if(marked_line->getType() == "track")
            {
                QString number = ui->lineEdit->text();
                current_zone->addTrack(marked_line, number);
            }
        }

        updateTrackListWidget();
    }
    qDebug("Leave PlatformPropertyWidget::addTrackButtonClicked");
}

void PlatformPropertyWidget::updateTrackListWidget()
{
    qDebug("Enter PlatformPropertyWidget::updateTrackListWidget");
    ui->listWidget_tracks->clear();

    if(current_zone != nullptr)
    {
        QList<JPSTrack *> tracks = current_zone->getTrackList();
        for (int i=0; i<tracks.size(); i++)
        {
            QString string = "";
            string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                           tracks[i]->getLine()->get_line()->line().x1(),
                           tracks[i]->getLine()->get_line()->line().x2(),
                           tracks[i]->getLine()->get_line()->line().y1(),
                           tracks[i]->getLine()->get_line()->line().y2());

            ui->listWidget_tracks->addItem(string);
        }
    }
    qDebug("Leave PlatformPropertyWidget::updateTrackListWidget");
}
