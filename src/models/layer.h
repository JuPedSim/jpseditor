#ifndef JPSEDITOR_LAYER_H
#define JPSEDITOR_LAYER_H

#include <QtWidgets>
#include <QtGui>

class Layer : public QGraphicsItemGroup
{

public:
    explicit Layer();

    ~Layer();

    QString getName();
    void setName(QString layername);

    void addToLayer(QGraphicsLineItem *item);
    QList<QGraphicsLineItem *> getLineItemList();

private:
    QString name;
    bool isHide;
    QList<QGraphicsLineItem *> lineItem_list; // This is just for show info in item list widget
};


#endif //JPSEDITOR_LAYER_H
