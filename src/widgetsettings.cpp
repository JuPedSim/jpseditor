#include "widgetsettings.h"
#include "ui_widgetsettings.h"

WidgetSettings::WidgetSettings(QWidget *parent, jpsGraphicsView *view) :
    QTabWidget(parent),
    ui(new Ui::WidgetSettings)
{
    ui->setupUi(this);
    _view=view;
    connect(ui->pushButtoncloseSettings,SIGNAL(clicked(bool)),QTabWidget::parent(),SLOT(Settings()));
    connect(ui->en_lineGrid,SIGNAL(toggled(bool)),this,SLOT(changeGridRadio(bool)));
    connect(ui->en_pointGrid,SIGNAL(toggled(bool)),this,SLOT(changeGridRadio(bool)));

}

WidgetSettings::~WidgetSettings()
{
    delete ui;
}

void WidgetSettings::changeGridRadio(bool status)
{
    if (status)
    {
        if (ui->en_lineGrid->isChecked())
            _view->ActivateLineGrid();

        else
            _view->ActivatePointGrid();
    }
}
