#ifndef JPSEDITOR_ROOMLISTWIDGET_H
#define JPSEDITOR_ROOMLISTWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/global.h"
#include "src/GraphicView.h"

namespace Ui {
class RoomListWidget;
}

class RoomListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomListWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr, jpsGraphicsView *gview = nullptr);

    ~RoomListWidget();


public slots:
    void updateRoomsListWidget();

    void addRoomButtonClicked();
    void deleteRoomButtonClicked();

    void selectRoom(QListWidgetItem *item);
    JPSZone *getCurrentRoom(QListWidgetItem *item);

    void renameRoom(QListWidgetItem *item);
    bool isRepeatedRoomName(QString name);



    void highlightRoom(QListWidgetItem *item);

signals:
    void zoneSelected(JPSZone *);
    void roomSelected(JPSZone *);
    void zoneDeleted();
    void roomDeleted();

private:
    Ui::RoomListWidget *ui;
    jpsDatamanager *data;
    jpsGraphicsView *view;
};

#endif //JPSEDITOR_ROOMLISTWIDGET_H
