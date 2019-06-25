//
// Created by sainho93 on 2019-06-19.
//

#ifndef JPSEDITOR_ROOMLISTWIDGET_H
#define JPSEDITOR_ROOMLISTWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/global.h"

namespace Ui {
class RoomListWidget;
}

class RoomListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomListWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~RoomListWidget();

    void setLabel(QString name);

public slots:
    void updateRoomsListWidget();
    void updateZonesListWidget();
    void addRoomButtonClicked();
    void addZoneButtonClicked();


private:
    Ui::RoomListWidget *ui;
    jpsDatamanager *data;
};

#endif //JPSEDITOR_ROOMLISTWIDGET_H
