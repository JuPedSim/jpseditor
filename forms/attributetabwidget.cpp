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

void attributeTabWidget::on_addButton_clicked()
{
    AddLayerDialog addLayerDialog(this);
    addLayerDialog.setModal(true);
    addLayerDialog.exec();
}
