#ifndef JPSEDITOR_BASICZONEWIDGET_H
#define JPSEDITOR_BASICZONEWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"

namespace Ui {
class BasicZoneWidget;
}

class BasicZoneWidget : public QWidget
{
    Q_OBJECT;

public:
    explicit BasicZoneWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~BasicZoneWidget();


private:
    Ui::BasicZoneWidget *ui;
};

#endif //JPSEDITOR_BASICZONEWIDGET_H
