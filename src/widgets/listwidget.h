#ifndef JPSEDITOR_LISTWIDGET_H
#define JPSEDITOR_LISTWIDGET_H

#include <QtGui>
#include <QtWidgets>

#include "src/datamanager.h"
#include "src/global.h"
#include "src/GraphicView.h"

namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr, jpsGraphicsView *gview = nullptr);

    ~ListWidget();


public slots:
    void updateList();

    void addButtonClicked();
    void deleteButtonClicked();

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
    Ui::ListWidget *ui;
    jpsDatamanager *data;
    jpsGraphicsView *view;


};

#endif //JPSEDITOR_LISTWIDGET_H
