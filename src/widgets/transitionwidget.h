#ifndef JPSEDITOR_TRANSITIONWIDGET_H
#define JPSEDITOR_TRANSITIONWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"

namespace Ui {
    class TransitionWidget;
}

class TransitionWidget : public QWidget
{
Q_OBJECT;

public:
    explicit TransitionWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr, jpsGraphicsView *mview =
    nullptr);

    ~TransitionWidget();

protected slots:
    void updateListWidget();
    void applyRooms();
    void deleteButtonClicked();
    void showRoomsinButton(QListWidgetItem *item);
    void highlightWall(QListWidgetItem *);

private:
    Ui::TransitionWidget *ui;
    
    jpsDatamanager *data;
    jpsGraphicsView *view;
};


#endif //JPSEDITOR_TRANSITIONWIDGET_H
