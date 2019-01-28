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
    emit snapStart_endpoint(ui->startendpointBox->isChecked());
    emit snapIntersections_point(ui->intersectionspointBox->isChecked());

//    if(ui->startendpointBox->isChecked())
//    {
//        emit snapStart_endpoint(true);
//    }else
//    {
//        emit snapStart_endpoint(false);
//    }

//    if(ui->intersectionspointBox->isChecked())
//    {
//        emit snapIntersections_point(true);
//    }else
//    {
//        emit snapIntersections_point(false);
//    }

//    if(ui->intersectionspointBox->isChecked())
//    {
//        emit snapIntersections_point(true);
//    }else
//    {
//        emit snapIntersections_point(false);
//    }
}
