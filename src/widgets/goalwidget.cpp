/***************************************************************
 *
 * \file goalwidget.cpp
 * \date 2019-03-26
 * \version v0.8.8
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
 * This class define a dock widget for goals
 *
****************************************************************/

#include "goalwidget.h"
#include "ui_goalwidget.h"
#include "src/models/jpsgoallistmodel.h"

#include <QDebug>
#include <QModelIndex>

GoalWidget::GoalWidget(QWidget *parent, jpsGraphicsView *view, jpsDatamanager *dmanager) :
    QWidget(parent),
    ui(new Ui::GoalWidget)
{
    ui->setupUi(this);

    currentView = view;

    //Set-up model and listview for sources widget
    model = new JPSGoalListModel();
    model->setGoalsList(currentView->getGoals());
    ui->goalsListView->setModel(model);
    ui->goalsListView->setEditTriggers(QAbstractItemView::DoubleClicked);
    showGoals();

    connect(currentView, SIGNAL(goalsChanged()), this, SLOT(showGoals()));
    connect(ui->goalsListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showGoalInformation()));
    connect(ui->applyButton, SIGNAL(clicked(bool)),this, SLOT(applyGoalInformation()));
    connect(this, SIGNAL(goalChanged(int)), currentView, SLOT(changeGoal(int)));
    connect(this, SIGNAL(goalDeleted(int)), currentView, SLOT(deleteGoal(int)));
    connect(ui->goalsListView, SIGNAL(clicked(const QModelIndex &)), currentView, SLOT(seleteGoal(const
    QModelIndex &)));
    connect(ui->deleteSourceButton, SIGNAL(clicked(bool)),this, SLOT(deleteButtonClicked()));
}

GoalWidget::~GoalWidget()
{
    delete ui;
}

/*
    show the new added goal informations
*/
void GoalWidget::showGoals()
{
    model->setGoalsList(currentView->getGoals());
    ui->goalsListView->setModel(model);
    ui->IDlineEdit->clear();
    ui->captionLineEdit->clear();
    ui->finalLineEdit->clear();
    ui->lineEdit_XMin->clear();
    ui->lineEdit_XMax->clear();
    ui->lineEdit_YMin->clear();
    ui->lineEdit_YMax->clear();
}


void GoalWidget::showGoalInformation()
{
    qDebug(">> Enter GoalWidget::showGoalInformation");
    QModelIndex index = ui->goalsListView->currentIndex();
    auto goal = currentView->getGoals().at(index.row());

    ui->IDlineEdit->setText(QString::number(goal->getId()));
    ui->captionLineEdit->setText(goal->getCaption());
    ui->finalLineEdit->setText(goal->getIsFinal());
    ui->lineEdit_XMin->setText(goal->getXMin());
    ui->lineEdit_XMax->setText(goal->getXMax());
    ui->lineEdit_YMin->setText(goal->getYMin());
    ui->lineEdit_YMax->setText(goal->getYMax());

    if(goal->getBeSaved()=="true")
    {
        ui->isSaveButton->setChecked(true);
    } else
    {
        ui->isSaveButton->setChecked(false);
    }

    qDebug(">> Leave SourceWidget::showGoalInformation");
}

void GoalWidget::applyGoalInformation()
{
    qDebug(">> Enter GoalWidget::applyGoalInformation");

    QModelIndex index = ui->goalsListView->currentIndex();
    if(index.isValid() && index.row() != -1)
    {
        auto goal = currentView->getGoals().at(index.row());

        goal->setId(ui->IDlineEdit->text().toInt());
        goal->setCaption(ui->captionLineEdit->text());
        goal->setIsFinal(ui->finalLineEdit->text());
        goal->setXMin(ui->lineEdit_XMin->text());
        goal->setXMax(ui->lineEdit_XMax->text());
        goal->setYMin(ui->lineEdit_YMin->text());
        goal->setYMax(ui->lineEdit_YMax->text());

        if(ui->isSaveButton->isChecked())
        {
            goal->setBeSaved("true");
        } else
        {
            goal->setBeSaved("false");
        }

        emit goalChanged(index.row());

    }

    qDebug(">> Leave GoalWidget::applyGoalInformation");
}

void GoalWidget::deleteButtonClicked()
{
    if (ui->goalsListView->currentIndex().isValid())
    {
        int index = ui->goalsListView->currentIndex().row();
        emit goalDeleted(index);
        showGoals();
    }
}

