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
 * Inherited from basicpropertywidget
****************************************************************/

#include "platformpropertywidget.h"
#include "ui_platformpropertywidget.h"

PlatformPropertyWidget::PlatformPropertyWidget(QWidget *parent, jpsDatamanager *dmanager)
    : BasicPropertyWidget(parent, dmanager), ui(new Ui::PlatformPropertyWidget)
{
    ui->setupUi(this);
}

PlatformPropertyWidget::~PlatformPropertyWidget()
{
    delete ui;
}
