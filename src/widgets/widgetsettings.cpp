/**
 * \file       widgetsettings.cpp 
 * \date        Oct-01-2018
 * \version     v0.8.4
 * \copyright   <2009-2018> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
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
 **/

     
#include "widgetsettings.h"
#include "ui_widgetsettings.h"

WidgetSettings::WidgetSettings(QWidget *parent, jpsGraphicsView *view) :
    QTabWidget(parent),
    ui(new Ui::WidgetSettings)
{
    qDebug("Enter WidgetSettings::WidgetSettings");
    ui->setupUi(this);
    _view=view;
    connect(ui->pushButtoncloseSettings,SIGNAL(clicked(bool)),QTabWidget::parent(),SLOT(Settings()));
    connect(ui->en_lineGrid,SIGNAL(toggled(bool)),this,SLOT(changeGridRadio(bool)));
    connect(ui->en_pointGrid,SIGNAL(toggled(bool)),this,SLOT(changeGridRadio(bool)));
    qDebug("Leave WidgetSettings::WidgetSettings");
}

WidgetSettings::~WidgetSettings()
{
    qDebug("Enter WidgetSettings::~WidgetSettings");
    delete ui;
    qDebug("Leave WidgetSettings::~WidgetSettings");
}

void WidgetSettings::changeGridRadio(bool status)
{
    qDebug("Enter WidgetSettings::changeGridRadio");
    if (status)
    {
        if (ui->en_lineGrid->isChecked())
            _view->ActivateLineGrid();

        else
            _view->ActivatePointGrid();
    }
    qDebug("Leave WidgetSettings::changeGridRadio");
}
