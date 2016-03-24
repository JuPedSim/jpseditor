/**
 * \file        widgetlandmark.h
 * \date        Jun 26, 2015
 * \version     v0.8.1
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
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
 *
 * \section Description
 * With the help of this class the user is able to handle landmarks and set associations (waypoints) relying on a certain landmark.
 * widgetlandmark is not enabled in v0.8.1.
 *
 *
 **/

#ifndef WIDGETLANDMARK_H
#define WIDGETLANDMARK_H

#include <QTabWidget>
#include "datamanager.h"
#include "GraphicView.h"
#include <memory>

using ptrLandmark = std::shared_ptr<jpsLandmark>;


namespace Ui {
class widgetLandmark;
}

class widgetLandmark : public QTabWidget
{
    Q_OBJECT

public:
    widgetLandmark(QWidget *parent = nullptr, jpsDatamanager *dmanager=nullptr, jpsGraphicsView *gview=nullptr);
    ~widgetLandmark();

protected slots:
    void show_landmarks();
    void add_room_to_landmark();
    void enable_room_selection();
    void disable_room_selection();
    void change_name();
    void SetPosInCMap();
    void ShowLandmarkType();
    void SetLandmarkType();
    void ShowRegionBox();
    //associated landmarks
    void AddAssociation();
    void RemoveAssociation();
    //Show or hide landmark picture and ellipse in graphicsview
    void ShowHideLandmark();
    //connections
    void NewConnection();
    void AskForFirstLandmark();
    void AskForSecondLandmark();
    void SetLandmarksToConnection();
    void SetLineItemAsConnection();
    void AddConnectionsToWidget();
    void RemoveConnection();
    //regions
    void NewRegion();
    void RemoveRegion();
    void SetLandmarkToRegion();
    void ShowHideRegion();
private:
    jpsLandmark *GetCurrentLandmark() const;
    jpsRegion* GetCurrentRegion() const;
    Ui::widgetLandmark *ui;
    jpsDatamanager* _dmanager;
    jpsGraphicsView* _gview;
    jpsConnection* _currentConnection;
};

#endif // WIDGETLANDMARK_H
