/**
 * \file        roomwidget.h
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
 * Which walls and/or doors belong to a certain room or obstacle can be defined with the help of this class.
 *
 *
 **/

#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QTabWidget>
#include "datamanager.h"
#include "GraphicView.h"



namespace Ui {
class roomWidget;
}

class roomWidget : public QTabWidget
{
    Q_OBJECT
    
public:
    roomWidget(QWidget *parent = 0L, jpsDatamanager *dmanager=0L, jpsGraphicsView *gview=0L);
    ~roomWidget();
    void show_rooms();
    void show_crossings();
    void show_exits();
    void show_obstacles();

protected slots:
    void new_room();
    void delete_room();
    void change_roomname();
    void change_elevation();
    void addWall();
    void removeWall();
    void showWallsAndType();
    void selectWall();
    void new_crossing();
    void enable_roomSelectionCrossings();
    void disable_roomSelectionCrossings();
    void add_rooms_to_crossing();
    void delete_crossing();
    void select_crossing();
    void new_exit();
    void enable_roomSelectionExits();
    void disable_roomSelectionExits();
    void add_rooms_to_exit();
    void delete_exit();
    void select_exit();
    void show_all();
    void new_obstacle();
    void delete_obstacle();
    void change_obsName();
    void addWallObs();
    void removeWallObs();
    void showWallsObs();
    void selectWallObs();
    void add_room_to_obs();
    void enable_roomSelectionObs();
    void disable_roomSelectionObs();
    bool shhi_roomCaption();
    bool shhi_roomCaption_obs();
    void highlight_room();
    void highlight_obs();
    void highlight_room(jpsRoom *);
    void highlight_obs(jpsRoom *);


//    void autoAssignDoors();
//    void autoAssignExits();
    void ChangeRoomType();


private:
    Ui::roomWidget *ui;
    jpsDatamanager *datamanager;
    jpsGraphicsView *graphview;

    //
    void ShowRoomType(int &cRow) const;

};

#endif // ROOMWIDGET_H
