#include "snappingoptions.h"
#include "ui_snappingoptions.h"

SnappingOptions::SnappingOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnappingOptions)
{
    qDebug("Enter SnappingOptions::SnappingOptions");
    ui->setupUi(this);

    connect(ui->startendpointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->intersectionspointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->centerpointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    connect(ui->linepointBox,SIGNAL(stateChanged(int)),this,SLOT(sendState()));
    qDebug("Leave SnappingOptions::SnappingOptions");
}

SnappingOptions::~SnappingOptions()
{
    qDebug("Enter SnappingOptions::~SnappingOptions");
    delete ui;
    qDebug("Leave SnappingOptions::~SnappingOptions");
}

void SnappingOptions::sendState()
{
    qDebug("Enter SnappingOptions::sendState");
    emit snapStart_endpoint(ui->startendpointBox->isChecked());
    emit snapIntersections_point(ui->intersectionspointBox->isChecked());
    emit snapCenter_point(ui->centerpointBox->isChecked());
    emit snapSelectedLine_point(ui->linepointBox->isChecked());
    qDebug("Leave SnappingOptions::sendState");
}

void SnappingOptions::setState(QList<bool> &snapping)
{
    qDebug("Enter SnappingOptions::setState");
    if(snapping[0])
        ui->startendpointBox->setChecked(true);

    if(snapping[1])
        ui->intersectionspointBox->setChecked(true);

    if(snapping[2])
        ui->centerpointBox->setChecked(true);

    if(snapping[3])
        ui->linepointBox->setChecked(true);
    qDebug("Leave SnappingOptions::setState");
}

QList<bool> SnappingOptions::getState() {
    qDebug("Enter SnappingOptions::getState");
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
    qDebug("Leave SnappingOptions::getState");
    return objectsnapping;
}
