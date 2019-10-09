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

    ui->InfoWidget->setReadOnly(true); // only for display
    ui->lineEdit_inifile->setReadOnly(true);
    ui->lineEdit_JPScore->setReadOnly(true);

    cmd = new QProcess(this);

    connect(cmd, SIGNAL(readyReadStandardOutput()) , this, SLOT(on_readoutput()));
    connect(cmd, SIGNAL(readyReadStandardError()) , this, SLOT(on_readerror()));
    connect(ui->lineEdit_cmd, SIGNAL(returnPressed()), this, SLOT(inputCommand()));

    connect(ui->pushButton_chooseInifle, SIGNAL(clicked()), this, SLOT(chooseInifileButtonClicked()));
    connect(ui->pushButton_chooseJPScore, SIGNAL(clicked()), this, SLOT(chooseJPScoreButtonClicked()));
    connect(ui->pushButton_run, SIGNAL(clicked()), this, SLOT(runButtonClicked()));

    cmd->start("zsh");
    cmd->waitForStarted();
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
//    QMessageBox::information(nullptr, "Error", cmd->readAllStandardError().data());
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
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose JPSCore Path"),""
            ,tr("JPScore (jpscore)"));

    ui->lineEdit_JPScore->setText(fileName);
    qDebug("Leave RunningWidget::chooseJPScoreButtonClicked");
}

void RunningWidget::runButtonClicked()
{
    qDebug("Enter RunningWidget::runButtonClicked");
    QString command = ui->lineEdit_JPScore->text() + " " + ui->lineEdit_inifile->text();
    ui->lineEdit_cmd->setText(command);

    inputCommand();
    qDebug("Leave RunningWidget::runButtonClicked");
}
