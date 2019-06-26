//
// Created by sainho93 on 2019-06-26.
//

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
    explicit TransitionWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~TransitionWidget();

public slots:
    void updateListWidget();

private:
    Ui::TransitionWidget *ui;
};


#endif //JPSEDITOR_TRANSITIONWIDGET_H
