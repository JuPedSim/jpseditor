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
    void addWall();
    void removeWall();
    void showWalls();
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
    void highlight_room();
    void autoAssignDoors();
    void autoAssignExits();


private:
    Ui::roomWidget *ui;
    jpsDatamanager *datamanager;
    jpsGraphicsView *graphview;

};

#endif // ROOMWIDGET_H
