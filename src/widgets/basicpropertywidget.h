#ifndef JPSEDITOR_BASICPROPERTYWIDGET_H
#define JPSEDITOR_BASICPROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"

namespace Ui {
class BasicPropertyWidget;
}

class BasicPropertyWidget : public QWidget
{
    Q_OBJECT;

public:
    explicit BasicPropertyWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~BasicPropertyWidget();


private:
    Ui::BasicPropertyWidget *ui;
};

#endif //JPSEDITOR_BASICPROPERTYWIDGET_H
