/***************************************************************
 *
 * \file template.cpp
 * \date 2019-09-23
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
 * Show template list
****************************************************************/

#include "templatewidget.h"
#include "ui_templatewidget.h"

TemplateWidget::TemplateWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::TemplateWidget)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Dialog);

    connect(this, SIGNAL(sendTemplateName(QString)), parent, SLOT(readFromTemplate(QString)));

    connect(ui->pushButton_choose, SIGNAL(clicked(bool)), this, SLOT(pushButton_chooseClicked()));
    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(pushButton_cancelClicked()));
}

TemplateWidget::~TemplateWidget()
{
    delete ui;
}

void TemplateWidget::pushButton_chooseClicked()
{
    qDebug("Enter TemplateWidget::pushButton_chooseClicked");
    int current_int = ui->treeWidget->currentColumn();
    QString current_name = ui->treeWidget->currentItem()->text(current_int);
    sendTemplateName(current_name);

    close();
    qDebug("Leave TemplateWidget::pushButton_chooseClicked");
}

void TemplateWidget::pushButton_cancelClicked()
{
    qDebug("Enter TemplateWidget::pushButton_cancelClicked");
    close();
    qDebug("Leave TemplateWidget::pushButton_cancelClicked");
}
