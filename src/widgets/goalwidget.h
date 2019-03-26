//
// Created by sainho93 on 2019-03-26.
//

#ifndef JPSEDITOR_GOALWIDGET_H
#define JPSEDITOR_GOALWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/graphicscene.h"
#include "src/models/jpssourcelistmodel.h"

namespace  Ui{
class GoalWidget;
}

class GoalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GoalWidget(QWidget *parent , jpsGraphicsView *view ,
                        jpsDatamanager *dmanager);

    ~GoalWidget();

private:
    Ui::GoalWidget *ui;
};


#endif //JPSEDITOR_GOALWIDGET_H
