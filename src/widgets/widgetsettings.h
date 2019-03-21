/**
 * \file       widgetsettings.h 
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

     
#ifndef WIDGETSETTINGS_H
#define WIDGETSETTINGS_H

#include <QTabWidget>
#include "src/GraphicView.h"

namespace Ui {
class WidgetSettings;
}

class WidgetSettings : public QTabWidget
{
    Q_OBJECT

public:
    explicit WidgetSettings(QWidget *parent = nullptr, jpsGraphicsView* view = nullptr);
    ~WidgetSettings();

public slots:
    void changeGridRadio(bool status);

private:
    Ui::WidgetSettings *ui;
    jpsGraphicsView* _view;


};

#endif // WIDGETSETTINGS_H
