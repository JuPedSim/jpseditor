#include "addlayerdialog.h"
#include "ui_addlayerdialog.h"

AddLayerDialog::AddLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLayerDialog)
{
    ui->setupUi(this);
}

AddLayerDialog::~AddLayerDialog()
{
    delete ui;
}

void AddLayerDialog::on_cancelButton_clicked()
{
    this->close();
}

void AddLayerDialog::on_confirmButton_clicked()
{
    QString name = ui->lineEdit->text();
}
