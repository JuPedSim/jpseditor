#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QFileDialog>
#include <QDebug>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    ui->locationLine->setReadOnly(true);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_cancelButton_clicked()
{
    this->close();
}

void SettingDialog::on_chooseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/Users",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->locationLine->setText(dir);
}


void SettingDialog::on_confirmButton_clicked()
{
    int interval =  ui->timeLine->text().toInt()*60000;
    QString interval_String = QString::number(interval);

    QString dir = ui->locationLine->text();

//    QString jpscore = ui->jpscoreLine->text();
//    QString jpsvis = ui->jpsvisLine->text();

    QMap<QString, QString> settingsmap;
    settingsmap["backupfolder"] = dir;
    settingsmap["interval"] = interval_String;

//    setting["defaultjpscore"] = jpscore;
//    setting["defaultjpsvis"] = jpsvis;

    emit sendSetting(settingsmap);

    this->close();
}

void SettingDialog::setCurrentSetting(QMap<QString, QString> settingsmap)
{
    ui->locationLine->setText(settingsmap["backupfolder"]);

    QString minutes = QString::number(settingsmap["interval"].toInt()/60000, 10);
    ui->timeLine->setText(minutes);

//    ui->jpscoreLine->setText(settingsmap["defaultjpscore"]);
//    ui->jpsvisLine->setText(settingsmap["defaultjpsvis"]);
}

void SettingDialog::on_choosejpsButton_clicked()
{
//    QString programm = QFileDialog::getOpenFileName(
//            this,
//            tr("Choose JPScore"),
//            "/Users",
//            "Programm (jpscore *.exe *.*)");

//    ui->jpscoreLine->setText(programm);
}

void SettingDialog::on_jpsvisButton_clicked()
{
//    QString programm = QFileDialog::getOpenFileName(
//            this,
//            tr("Choose JPSvis"),
//            "/Users",
//            "Programm (*.app *.exe *.*)");

//    ui->jpsvisLine->setText(programm);
}

void SettingDialog::on_cancelButton_page2_clicked()
{
    this->close();
}

void SettingDialog::on_confirmButton_page2_clicked()
{
    on_confirmButton_clicked();
}

void SettingDialog::on_applyButton_clicked()
{
    int interval =  ui->timeLine->text().toInt()*60000;
    QString interval_String = QString::number(interval);

    QString dir = ui->locationLine->text();

//    QString jpscore = ui->jpscoreLine->text();
//    QString jpsvis = ui->jpsvisLine->text();

    QMap<QString, QString> settingsmap;
    settingsmap["backupfolder"] = dir;
    settingsmap["interval"] = interval_String;

    emit sendSetting(settingsmap);

}
