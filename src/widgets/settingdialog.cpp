#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QFileDialog>
#include <QDebug>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    qDebug("Enter SettingDialog::SettingDialog");
    ui->setupUi(this);

    // Set line read only
    ui->locationLine->setReadOnly(true);
    ui->lineEdit_JPScore->setReadOnly(true);

    connect(this, SIGNAL(changeTimer(int)),parent,SLOT(setTimer(int)));

    connect(ui->chooseButton, SIGNAL(clicked()),this, SLOT(chooseBackupButtonClicked()));
    connect(ui->applyButton, SIGNAL(clicked()),this, SLOT(applyButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()),this, SLOT(cancelButtonClicked()));

    connect(ui->pushButton_chooseJPScore, SIGNAL(clicked()),this, SLOT(chooseJPScoreButtonClicked()));
    connect(ui->pushButton_chooseJPSvis, SIGNAL(clicked()),this, SLOT(chooseJPSvisButtonClicked()));

    // Setting up settings
    loadSettings();
    qDebug("Leave SettingDialog::SettingDialog");
}

SettingDialog::~SettingDialog()
{
    qDebug("Enter SettingDialog::~SettingDialog");
    delete ui;
    qDebug("SettingDialog::~SettingDialog");
}

void SettingDialog::cancelButtonClicked()
{
    qDebug("SettingDialog::on_cancelButton_clicked");
    this->close();
    qDebug("SettingDialog::on_cancelButton_clicked");
}

void SettingDialog::chooseBackupButtonClicked()
{
    qDebug("SettingDialog::on_chooseButton_clicked");
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/Users",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->locationLine->setText(dir);
    qDebug("SettingDialog::on_chooseButton_clicked");
}

void SettingDialog::applyButtonClicked()
{
    qDebug("Enter SettingDialog::applyButtonClicked");

    QSettings settings("FZJ","JPSeditor");

    settings.beginGroup("backup");
    settings.setValue("backupfolder", ui->locationLine->text());
    settings.setValue("interval", ui->timeLine->text());
    settings.endGroup();

    emit changeTimer(ui->timeLine->text().toInt()); // emit to mainwindow

    settings.beginGroup("JuPedSim");
    settings.setValue("JPScore", ui->lineEdit_JPScore->text());
    settings.setValue("JPSvis", ui->lineEdit_JPSvis->text());
    settings.endGroup();

    qDebug("Leave SettingDialog::applyButtonClicked");
}

void SettingDialog::loadSettings()
{
    qDebug("Enter MWindow::loadSettings");
    QSettings settings("FZJ","JPSeditor");

    settings.beginGroup("backup");
    ui->locationLine->setText(settings.value("backupfolder", "../").toString());
    ui->timeLine->setText(settings.value("interval", "1").toString());
    settings.endGroup();

    settings.beginGroup("JuPedSim");
    ui->lineEdit_JPScore->setText(settings.value("JPScore", "../").toString());
    ui->lineEdit_JPSvis->setText(settings.value("JPSvis", "../").toString());
    settings.endGroup();

    qDebug("Leave MWindow::loadSettings");
}

void SettingDialog::chooseJPScoreButtonClicked()
{
    qDebug("Enter SettingDialog::chooseJPScoreButtonClicked");

    QString programm = QFileDialog::getOpenFileName(
            this,
            tr("Choose JPSvis"),
            "/Users",
            "jpscore");

    ui->lineEdit_JPScore->setText(programm);
    qDebug("Leave SettingDialog::chooseJPScoreButtonClicked");
}

void SettingDialog::chooseJPSvisButtonClicked()
{
    qDebug("Enter SettingDialog::chooseJPSvisButtonClicked");
    QString programm = QFileDialog::getOpenFileName(
            this,
            tr("Choose JPSvis"),
            "/Users",
            "jpsvis.app");

    ui->lineEdit_JPSvis->setText(programm);
    qDebug("Leave SettingDialog::chooseJPSvisButtonClicked");
}
