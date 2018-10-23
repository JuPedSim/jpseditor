#include "attributetabwidget.h"
#include "ui_attributetabwidget.h"

attributeTabWidget::attributeTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::attributeTabWidget)
{
    ui->setupUi(this);
}

attributeTabWidget::~attributeTabWidget()
{
    delete ui;
}
