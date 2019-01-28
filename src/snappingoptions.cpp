#include "snappingoptions.h"
#include "ui_snappingoptions.h"

SnappingOptions::SnappingOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnappingOptions)
{
    ui->setupUi(this);
}

SnappingOptions::~SnappingOptions()
{
    delete ui;
}
