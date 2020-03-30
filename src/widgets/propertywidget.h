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
 * This class is for room, stair, platform property widget
****************************************************************/

#ifndef JPSEDITOR_PROPERTYWIDGET_H
#define JPSEDITOR_PROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/jpstransition.h"

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    Q_OBJECT;

public:
    explicit PropertyWidget(QWidget *parent, jpsDatamanager *dmanager,
                            JPSZone *zone, jpsGraphicsView *gview);

    ~PropertyWidget();

protected slots:
    void updateWidget();
    void updateListwidget(); // update wall/track list widget

    // WallMode tab
    void updateWallListWidget();
    void addWallButtonClicked();
    void removeWallButtonClicked();
    void highlightWall(QListWidgetItem *item);

    // TrackMode tab
    void updateTrackListWidget();
    void addTrackButtonClicked();
    void removeTrackButtonClicked();
    void updateType(QListWidgetItem* item);
    void applyTypeButtonClicked();

    // Property tab
    void updatePropertyWidget();
    int getRoomType(JPSZone *zone);
    RoomType getRoomTypeFromString(QString type);
    void applyButtonClicked();

private:
    Ui::PropertyWidget *ui;
    jpsGraphicsView *view;
    jpsDatamanager *data;

    JPSZone *current_zone;

    ZoneType zoneType;
};

#endif //JPSEDITOR_PROPERTYWIDGET_H
