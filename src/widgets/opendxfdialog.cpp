#include "opendxfdialog.h"
#include "ui_opendxfdialog.h"

#include <QFile>
#include <QFileDialog>

OpenDXFDialog::OpenDXFDialog(QWidget *parent, jpsDatamanager* dmanager):
    QDialog(parent),
    ui(new Ui::OpenDXFDialog),
    data(dmanager)
{
    ui->setupUi(this);

    connect(this, SIGNAL(DXFImported(QString)), parent, SLOT(ShowInfoOnStatusBar(QString)));

    connect(ui->pushButton_openfile, SIGNAL(clicked()), this, SLOT(openButtonClicked()));
    connect(ui->pushButton_addToImport, SIGNAL(clicked()), this, SLOT(addToImportButtonClicked()));
    connect(ui->pushButton_removeToImport, SIGNAL(clicked()), this, SLOT(removeToImportButtonCliked()));
    connect(ui->pushButton_cancelImport, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(ui->pushButton_confirmImport, SIGNAL(clicked()), this, SLOT(confirmButtonClicked()));
}

OpenDXFDialog::~OpenDXFDialog()
{
    delete ui;
}

void OpenDXFDialog::openButtonClicked()
{
    qDebug("Enter OpenDXFDialog::openButtoClicked");
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open DXF"),"",tr("DXF-Drawings (*.dxf)"));

    ui->lineEdit_file->setText(fileName);

    QFile file(fileName);
    std::string fName= fileName.toStdString();

    if(data->readLayersInDXF(fName))
    {
        for(auto i : data->get_layerInDXF())
        {
            ui->listWidget_layersInFile->addItem(i);
        }
    }

    qDebug("Leave OpenDXFDialog::openButtoClicked");
}

void OpenDXFDialog::addToImportButtonClicked()
{
    qDebug("Enter OpenDXFDialog::addToImportButtonClicked");
    if(ui->listWidget_layersInFile->currentItem() != nullptr)
    {
        QString addedText = ui->listWidget_layersInFile->currentItem()->text() + ";" + ui->comboBox_ZoneType->currentText();

        ui->listWidget_LayersToImport->addItem(addedText);
    }
    qDebug("Leave OpenDXFDialog::addToImportButtonClicked");
}

void OpenDXFDialog::removeToImportButtonCliked()
{
    qDebug("Enter OpenDXFDialog::removeToImportButtonCliked");
    if(ui->listWidget_LayersToImport->currentItem() != nullptr)
    {
        delete ui->listWidget_LayersToImport->takeItem(ui->listWidget_LayersToImport->currentRow());
    }

    qDebug("Leave OpenDXFDialog::removeToImportButtonCliked");
}

void OpenDXFDialog::cancelButtonClicked()
{
    qDebug("Enter OpenDXFDialog::cancelButtonClicked");
    this->close();
    qDebug("Leave OpenDXFDialog::cancelButtonClicked");
}

void OpenDXFDialog::confirmButtonClicked()
{
    qDebug("Enter OpenDXFDialog::confirmButtonClicked");
    for(int i = 0; i < ui->listWidget_LayersToImport->count(); ++i)
    {
        QString layer = ui->listWidget_LayersToImport->item(i)->text().split(";").first();
        QString zoneType = ui->listWidget_LayersToImport->item(i)->text().split(";").last();

        data->addItemInImportLayers(layer, zoneType);
    }

    QString fileName = ui->lineEdit_file->text();
    QFile file(fileName);
    std::string fName= fileName.toStdString();

    if (data->readDXF(fName))
    {
        emit DXFImported("DXF-File successfully loaded!");
    }
    else
    {
        emit DXFImported("DXF-File could not be parsed!");
    }

    this->close();
    qDebug("Leave OpenDXFDialog::confirmButtonClicked");
}
