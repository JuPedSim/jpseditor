#include "snappingoptions.h"
#include "ui_snappingoptions.h"

SnappingOptions::SnappingOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnappingOptions)
{
    ui->setupUi(this);

    connect(ui->startendpointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->intersectionspointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->centerpointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->linepointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
}

SnappingOptions::~SnappingOptions()
{
    delete ui;
}

void SnappingOptions::sendState()
{
    if(ui->startendpointBox->isChecked())
    {
        emit snapStart_endpoint(true);
    }else
    {
        emit snapStart_endpoint(false);
    }
}
