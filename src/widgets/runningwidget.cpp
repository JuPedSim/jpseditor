/***************************************************************
 *
 * \file runningwidget.cpp
 * \date 2019-09-24
 * \version 
 * \author Tao Zhong
 * \copyright <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section Lincense
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 *
 * For simulation
****************************************************************/

#include "runningwidget.h"
#include "ui_runningwidget.h"

#include <QtWidgets>

RunningWidget::RunningWidget(QWidget *parent) : QWidget(parent), ui(new Ui::RunningWidget)
{
    ui->setupUi(this);

    // Set-up ui
    ui->InfoWidget->setReadOnly(true); // only for display
    ui->lineEdit_inifile->setReadOnly(true);
    ui->lineEdit_JPScore->setReadOnly(true);
    ui->lineEdit_JPSvis->setReadOnly(true);
    ui->lineEdit_Geometry->setReadOnly(true);
    ui->lineEdit_Trajetory->setReadOnly(true);

    // Load default settings
    QSettings settings("FZJ","JPSeditor");

    settings.beginGroup("JuPedSim");
    ui->lineEdit_JPScore->setText(settings.value("JPScore", "../").toString());
    ui->lineEdit_JPSvis->setText(settings.value("JPSvis", "../").toString());
    settings.endGroup();

    // Start process
    cmd = new QProcess(this);
    cmd->start("zsh");
    cmd->waitForStarted();

    // Signals & Slots
    connect(cmd, SIGNAL(readyReadStandardOutput()) , this, SLOT(on_readoutput()));
    connect(cmd, SIGNAL(readyReadStandardError()) , this, SLOT(on_readerror()));
    connect(ui->lineEdit_cmd, SIGNAL(returnPressed()), this, SLOT(inputCommand()));

    connect(ui->pushButton_chooseInifle, SIGNAL(clicked()), this, SLOT(chooseInifileButtonClicked()));
    connect(ui->pushButton_chooseJPScore, SIGNAL(clicked()), this, SLOT(chooseJPScoreButtonClicked()));
    connect(ui->pushButton_chooseJPSvis, SIGNAL(clicked()), this, SLOT(chooseJPSvisButtonClicked()));
    connect(ui->pushButton_chooseGeometry, SIGNAL(clicked()), this, SLOT(chooseGeometryButtonClicked()));
    connect(ui->pushButton_chooseTrajetory, SIGNAL(clicked()), this, SLOT(chooseTrajetoryButtonClicked()));

    connect(ui->pushButton_run, SIGNAL(clicked()), this, SLOT(runButtonClicked()));
    connect(ui->pushButton_viewGeometry, SIGNAL(clicked()), this, SLOT(viewGeometryButtonClicked()));
    connect(ui->pushButton_viewResult, SIGNAL(clicked()), this, SLOT(viewResultButtonClicked()));
}

RunningWidget::~RunningWidget()
{
    if(cmd)
    {
        cmd->terminate();
        cmd->waitForFinished();
    }
    delete ui;
}

void RunningWidget::on_readoutput()
{
    qDebug("Enter RunningWidget::on_readoutput");
    ui->InfoWidget->append(cmd->readAllStandardOutput().data());
    qDebug("Leave RunningWidget::on_readoutput");
}

void RunningWidget::on_readerror()
{
    qDebug("Enter RunningWidget::on_readerror");
    ui->InfoWidget->append(cmd->readAllStandardError().data());
    qDebug("Leave RunningWidget::on_readerror");
}

void RunningWidget::inputCommand()
{
    qDebug("Enter RunningWidget::inputCommand");
    cmd->write(ui->lineEdit_cmd->text().toLocal8Bit() + '\n');

    ui->lineEdit_cmd->clear();
    qDebug("Leave RunningWidget::inputCommand");
}
void RunningWidget::chooseInifileButtonClicked()
{
    qDebug("Enter RunningWidget::chooseGeometryButtonClicked");
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose inifile"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_inifile->setText(fileName);
    qDebug("Leave RunningWidget::chooseGeometryButtonClicked");
}

void RunningWidget::chooseJPScoreButtonClicked()
{
    qDebug("Enter RunningWidget::chooseJPScoreButtonClicked");
    QString programm = QFileDialog::getOpenFileName(
            this,
            tr("Choose JPSvis"),
            "/Users",
            "jpscore");

    ui->lineEdit_JPScore->setText(programm);
    qDebug("Leave RunningWidget::chooseJPScoreButtonClicked");
}

void RunningWidget::chooseJPSvisButtonClicked() 
{
    qDebug("Enter RunningWidget::chooseJPSvisButtonClicked");
    QString programm = QFileDialog::getOpenFileName(
            this,
            tr("Choose JPSvis"),
            "/Users",
            "jpsvis.app");

    ui->lineEdit_JPSvis->setText(programm);
    qDebug("Leave RunningWidget::chooseJPSvisButtonClicked");
}

void RunningWidget::chooseGeometryButtonClicked()
{
    qDebug("Enter RunningWidget::chooseGeometryButtonClicked");
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose geometry"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_Geometry->setText(fileName);
    qDebug("Leave RunningWidget::chooseGeometryButtonClicked");
}

void RunningWidget::chooseTrajetoryButtonClicked()
{
    qDebug("Enter RunningWidget::chooseTrajetoryButtonClicked");
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose trajetory"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_Trajetory->setText(fileName);
    qDebug("Leave RunningWidget::chooseTrajetoryButtonClicked");
}

void RunningWidget::viewGeometryButtonClicked()
{
    qDebug("Enter RunningWidget::viewGeometryButtonClicked");
    QString command = ui->lineEdit_JPSvis->text() + "/Contents/MacOS/jpsvis" + " " + ui->lineEdit_Geometry->text();
    ui->lineEdit_cmd->setText(command);

    inputCommand();
    qDebug("Leave RunningWidget::viewGeometryButtonClicked");
}

void RunningWidget::viewResultButtonClicked()
{
    qDebug("Enter RunningWidget::viewResultButtonClicked");
    QString command = ui->lineEdit_JPSvis->text() + "/Contents/MacOS/jpsvis"  +
            " " + ui->lineEdit_Trajetory->text();
    ui->lineEdit_cmd->setText(command);

    inputCommand();
    qDebug("Leave RunningWidget::viewResultButtonClicked");
}

void RunningWidget::runButtonClicked()
{
    qDebug("Enter RunningWidget::runButtonClicked");
    QString command = ui->lineEdit_JPScore->text() + " " + ui->lineEdit_inifile->text();
    ui->lineEdit_cmd->setText(command);

    inputCommand();
    qDebug("Leave RunningWidget::runButtonClicked");
}