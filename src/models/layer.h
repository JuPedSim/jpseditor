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
    void removeLineItem(QGraphicsLineItem *line);
    QList<QGraphicsLineItem *> getLineItemList();
    bool isHide();

private:
    QString name;
    bool hide;
    QList<QGraphicsLineItem *> lineItem_list; // This is just for show info in item list widget
};


#endif //JPSEDITOR_LAYER_H
