#ifndef JPSEDITOR_LAYERWIDGET_H
#define JPSEDITOR_LAYERWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/GraphicView.h"

namespace Ui {
    class LayerWidget;
}

class LayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LayerWidget(QWidget *parent = nullptr, jpsGraphicsView *mview = nullptr);

    ~LayerWidget();

private:
    Ui::LayerWidget *ui;
    jpsGraphicsView *view;
};


#endif //JPSEDITOR_LAYERWIDGET_H
