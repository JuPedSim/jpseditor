#ifndef JPSEDITOR_PLATFORMPROPERTYWIDGET_H
#define JPSEDITOR_PLATFORMPROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/widgets/basicpropertywidget.h"

namespace Ui {
    class PlatformPropertyWidget;
}

class PlatformPropertyWidget : public BasicPropertyWidget
{
    Q_OBJECT;

public:
    explicit PlatformPropertyWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr);

    ~PlatformPropertyWidget();

private:
    Ui::PlatformPropertyWidget *ui;
};


#endif //JPSEDITOR_PLATFORMPROPERTYWIDGET_H
