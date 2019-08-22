#ifndef JPSEDITOR_LAYER_H
#define JPSEDITOR_LAYER_H

#include <QtWidgets>
#include <QtGui>
#include "src/jpsLineItem.h"

class Layer : public QGraphicsItemGroup
{

public:
    explicit Layer();

    ~Layer();

    QString getName();
    void setName(QString layername);

    // For jpsLineItem (wall, crosssing, transition, track, hline)
    void addLineToLayer(jpsLineItem *wall);
    void removeLineFromLayer(jpsLineItem *wall);
    QList<jpsLineItem *> getLineItemList();

    // For jpsLandmark

    // For JPSGoal & JPSSource

    bool isHide();

private:
    QString name;
    bool hide;

    QList<jpsLineItem *> lineItem_list; // This is just for show info in item list widget
};


#endif //JPSEDITOR_LAYER_H
