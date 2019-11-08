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

    void setLabel(QString name);

public slots:
    void updateRoomsListWidget();
    void updateZonesListWidget(QListWidgetItem *item);

    void addRoomButtonClicked();
    void addZoneButtonClicked();
    void deleteRoomButtonClicked();
    void deleteZoneButtonClicked();

    void selectZone(QListWidgetItem *item);
    JPSZone *getCurrentZone(QListWidgetItem *item);

    void selectRoom(QListWidgetItem *item);
    JPSZone *getCurrentRoom(QListWidgetItem *item);

    void renameRoom(QListWidgetItem *item);
    bool isRepeatedRoomName(QString name);

    void renameZone(QListWidgetItem *item);
    bool isRepeatedZoneName(QString name);

    void highlightRoom(QListWidgetItem *item);
    void highlightZone(QListWidgetItem *item);

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
