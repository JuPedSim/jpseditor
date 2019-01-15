#include "layerwidget.h"
#include "ui_layerwidget.h"

Layerwidget::Layerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Layerwidget)
{
    ui->setupUi(this);
}

Layerwidget::~Layerwidget()
{
    delete ui;
}
