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

    connect(ui->listWidget_layers, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(checkVisibility(QListWidgetItem*)));

    // Layer widget
    connect(ui->pushButton_addLayer, SIGNAL(clicked()), this, SLOT(addLayerButtonClicked()));
    connect(ui->listWidget_layers, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(renameLayer(QListWidgetItem *)));
    connect(ui->pushButton_deleteLayer, SIGNAL(clicked()), this, SLOT(deleteLayerButtonClicked()));

    // Item widget
    connect(ui->pushButton_addItems, SIGNAL(clicked()), this, SLOT(addItemsButtonClicked()));
    connect(ui->pushButton_removeItems, SIGNAL(clicked()), this, SLOT(removeItemsButtonClicked()));
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
    updateItemsListWidget();
    qDebug("Leave void LayerWidget::deleteLayerButtonClicked");
}

void LayerWidget::addItemsButtonClicked()
{
    qDebug("Enter LayerWidget::addItemsButtonClicked");
    if(ui->listWidget_layers->currentItem() != nullptr)
    {
        if(view->getLayerList()[ui->listWidget_layers->currentRow()] == nullptr)
            return;

        if(!view->get_markedLines().isEmpty())
        {
            // Add Wall into layer
            foreach(jpsLineItem *line, view->get_markedLines())
            {
                view->getLayerList()[ui->listWidget_layers->currentRow()]->addToLayer(line->get_line());
            }
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Add or select a Layer at first!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    updateItemsListWidget();

    qDebug("Leave LayerWidget::addItemsButtonClicked");
}

void LayerWidget::updateItemsListWidget()
{
    qDebug("Enter LayerWidget::updateItemsListWidget");
    ui->listWidget_items->clear();

    if(ui->listWidget_layers->currentItem() == nullptr)
        return;

    foreach(Layer *layer, view->getLayerList())
    {
        // Show QGraphicsLineItem
        foreach(QGraphicsLineItem *item, layer->getLineItemList())
        {
            QString string = "";
            string.sprintf("Line: [%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                    item->line().x1(),
                    item->line().x2(),
                    item->line().y1(),
                    item->line().y2());

            ui->listWidget_items->addItem(string);
        }
    }
    qDebug("Leave LayerWidget::updateItemsListWidget");
}

void LayerWidget::checkVisibility(QListWidgetItem *item)
{
    qDebug("Enter LayerWidget::checkVisibility");
    if(view->getLayerList()[ui->listWidget_layers->currentRow()]->isHide())
    {
        ui->pushButton_showLayer->setDisabled(true);
        ui->pushButton_hideLayer->setDisabled(false);
        item->setToolTip("This Layer is already hidden");
    }
    else
    {
        ui->pushButton_hideLayer->setDisabled(true);
        ui->pushButton_showLayer->setDisabled(false);
    }
    qDebug("Leave LayerWidget::checkVisibility");
}

void LayerWidget::removeItemsButtonClicked()
{
    qDebug("Enter LayerWidget::removeItemsButtonClicked");
    if(ui->listWidget_items->currentItem() != nullptr)
    {
        Layer *layer = view->getLayerList()[ui->listWidget_layers->currentRow()];
        QListWidgetItem *cItem = ui->listWidget_items->currentItem();
        if(cItem->text().contains("Line")) // If cItem is Line Item
        {
            QGraphicsLineItem *line = layer->getLineItemList()[ui->listWidget_items->currentRow()];
            layer->removeLineItem(line);
        }
    }

    updateItemsListWidget();
    qDebug("Leave LayerWidget::removeItemsButtonClicked");
}
