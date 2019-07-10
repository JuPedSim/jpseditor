#ifndef JPSEDITOR_TRACKPROPERTYWIDGET_H
#define JPSEDITOR_TRACKPROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/GraphicView.h"

namespace Ui {
    class TrackPropertyWidget;
}

class TrackPropertyWidget : public QWidget
{
    Q_OBJECT;

public:
    explicit TrackPropertyWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~TrackPropertyWidget();

    void updateListWidget();

private:
    Ui::TrackPropertyWidget *ui;
    jpsDatamanager *data;
    jpsGraphicsView *view;
};


#endif //JPSEDITOR_TRACKPROPERTYWIDGET_H
