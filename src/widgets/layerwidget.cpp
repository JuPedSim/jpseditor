/***************************************************************
 *
 * \file layerwidget.cpp
 * \date 2019-08-21
 * \version 0.8.9
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
 * Edit layers in Geometery
 * Controller for layer widget
****************************************************************/

#include "layerwidget.h"
#include "ui_layerwidget.h"

LayerWidget::LayerWidget(QWidget *parent, jpsGraphicsView *mview)
        : QWidget(parent), ui(new Ui::LayerWidget)
{
    ui->setupUi(this);
    view = mview;

    updateLayerListWidget();

    // Layer widget
    connect(ui->pushButton_addLayer, SIGNAL(clicked()), this, SLOT(addLayerButtonClicked()));
    connect(ui->listWidget_layers, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(renameLayer(QListWidgetItem *)));
    connect(ui->pushButton_deleteLayer, SIGNAL(clicked()), this, SLOT(deleteLayerButtonClicked()));


}

LayerWidget::~LayerWidget()
{
    delete ui;
}

void LayerWidget::addLayerButtonClicked()
{
    qDebug("Enter LayerWidget::addLayerButtonClicked");
    view->addLayer();
    updateLayerListWidget();
    qDebug("Leave LayerWidget::addLayerButtonClicked");
}

void LayerWidget::updateLayerListWidget()
{
    qDebug("Enter LayerWidget::updateLayerListWidget");
    ui->listWidget_layers->clear();

    if(view->getLayerList().isEmpty())
        return;

    QList<Layer *> layer_list = view->getLayerList();
    foreach(Layer *layer, layer_list)
    {
        QString string = layer->getName();
        ui->listWidget_layers->addItem(string);
    }

    qDebug("Leave LayerWidget::updateLayerListWidget");
}

void LayerWidget::renameLayer(QListWidgetItem *item)
{
    qDebug("Enter LayerWidget::renameLayer");
    QString name = QInputDialog::getText(this, tr("Rename"),
                                         tr("New name:"), QLineEdit::Normal,
                                         "Floor");

    if(!isRepeatedName(name))
    {
        view->getLayerList()[ui->listWidget_layers->currentRow()]->setName(name);
    } else
    {
        QMessageBox::warning(this,"Warning!", "This name is already used, change another?",
                             QMessageBox::Ok);
    }

    updateLayerListWidget();

    qDebug("Leave LayerWidget::renameLayer");
}

bool LayerWidget::isRepeatedName(QString name)
{
    qDebug("Enter LayerWidget::isRepeatedName");
    foreach(Layer *layer, view->getLayerList())
    {
        if(name == layer->getName())
            return true;
    }
    qDebug("Leave LayerWidget::isRepeatedName");
    return false;
}

void LayerWidget::deleteLayerButtonClicked()
{
    qDebug("Enter void LayerWidget::deleteLayerButtonClicked");
    if(ui->listWidget_layers->currentItem() != nullptr)
    {
        view->deleteLayer(view->getLayerList()[ui->listWidget_layers->currentRow()]);
    }

    updateLayerListWidget();
    qDebug("Leave void LayerWidget::deleteLayerButtonClicked");
}
