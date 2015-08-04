#ifndef WIDGETSETTINGS_H
#define WIDGETSETTINGS_H

#include <QTabWidget>
#include "GraphicView.h"

namespace Ui {
class WidgetSettings;
}

class WidgetSettings : public QTabWidget
{
    Q_OBJECT

public:
    explicit WidgetSettings(QWidget *parent = nullptr, jpsGraphicsView* view = nullptr);
    ~WidgetSettings();

public slots:
    void changeGridRadio(bool status);

private:
    Ui::WidgetSettings *ui;
    jpsGraphicsView* _view;


};

#endif // WIDGETSETTINGS_H
