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

private:
    QString name;
    bool isHide;
};


#endif //JPSEDITOR_LAYER_H
