//
// Created by sainho93 on 2019-06-19.
//

#ifndef JPSEDITOR_ROOMLISTWIDGET_H
#define JPSEDITOR_ROOMLISTWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"

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

private:
    Ui::RoomListWidget *ui;
};

#endif //JPSEDITOR_ROOMLISTWIDGET_H
