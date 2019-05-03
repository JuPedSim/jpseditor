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
    emit snapCenter_point(ui->centerpointBox->isChecked());
    emit snapSelectedLine_point(ui->linepointBox->isChecked());
}

void SnappingOptions::setState(QList<bool> &snapping)
{
    if(snapping[0])
        ui->startendpointBox->setChecked(true);

    if(snapping[1])
        ui->intersectionspointBox->setChecked(true);

    if(snapping[2])
        ui->centerpointBox->setChecked(true);

    if(snapping[3])
        ui->linepointBox->setChecked(true);
}

QList<bool> SnappingOptions::getState() {
    QList<bool> objectsnapping;
    bool endpoint;
    bool Intersections_point;
    bool Center_point;
    bool SelectedLine_point;

    if(ui->startendpointBox->isChecked())
    {
         endpoint = true;
    }
    else{
        endpoint = false;
    }

    if(ui->intersectionspointBox->isChecked())
    {
        Intersections_point = true;
    } else{
        Intersections_point = false;
    }

    if(ui->centerpointBox->isChecked())
    {
        Center_point = true;
    } else{
        Center_point = false;
    }

    if(ui->linepointBox->isChecked())
    {
        SelectedLine_point = true;
    } else{
        SelectedLine_point = false;
    }

    objectsnapping.append(endpoint);
    objectsnapping.append(Intersections_point);
    objectsnapping.append(Center_point);
    objectsnapping.append(SelectedLine_point);;

    return objectsnapping;
}
