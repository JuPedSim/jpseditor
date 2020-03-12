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

    connect(ui->pushButton_openfile, SIGNAL(clicked()), this, SLOT(openButtonClicked()));
    connect(ui->pushButton_addToImport, SIGNAL(clicked()), this, SLOT(()));
}

OpenDXFDialog::~OpenDXFDialog()
{
    delete ui;
}

void OpenDXFDialog::openButtonClicked()
{
    qDebug("Enter OpenDXFDialog::openButtoClicked");
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open DXF"),"",tr("DXF-Drawings (*.dxf)"));

    ui->lineEdit_file->setText(fileName.split("/").last());

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
