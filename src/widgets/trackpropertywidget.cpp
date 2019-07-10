/***************************************************************
 *
 * \file trackpropertywidget.cpp
 * \date 2019-07-10
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
 *
 * This class is for track property Dockwidget
****************************************************************/


#include "trackpropertywidget.h"
#include "ui_trackpropertywidget.h"

TrackPropertyWidget::TrackPropertyWidget(QWidget *parent, jpsDatamanager *dmanager)
        : QWidget(parent), ui(new Ui::TrackPropertyWidget)
{
    ui->setupUi(this);
}

TrackPropertyWidget::~TrackPropertyWidget()
{
    delete ui;
}

void TrackPropertyWidget::updateListWidget()
{

}
