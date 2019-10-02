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
    qDebug("Enter LayerWidget::LayerWidget");
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
    connect(ui->listWidget_items, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT( highLight(QListWidgetItem *)));

    // Hide/Show Layer
    connect(ui->pushButton_hideLayer, SIGNAL(clicked()), this, SLOT(hideLayerButtonClicked()));
    connect(ui->pushButton_showLayer, SIGNAL(clicked()), this, SLOT(showLayerButtonClicked()));

    qDebug("Leave LayerWidget::LayerWidget");
}

LayerWidget::~LayerWidget()
{
    qDebug("Enter LayerWidget::~LayerWidget");
    delete ui;
    qDebug("Leave LayerWidget::~LayerWidget");
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
        QMessageBox msgBox;
        msgBox.setText("This name is already used, change another?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Ok:
                // Ok was clicked
                break;
            default:
                // should never be reached
                break;
        }
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
        // For wall, crossing, transition, track, hline
        if(!view->get_markedLines().isEmpty())
        {
            foreach(jpsLineItem *line, view->get_markedLines())
            {
                view->getLayerList()[ui->listWidget_layers->currentRow()]->addLineToLayer(line);
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
        foreach(jpsLineItem *item, layer->getLineItemList())
        {
            QString string = "";
            string.sprintf("Line: [%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                    item->get_line()->line().x1(),
                    item->get_line()->line().x2(),
                    item->get_line()->line().y1(),
                    item->get_line()->line().y2());

            ui->listWidget_items->addItem(string);
        }
    }
    qDebug("Leave LayerWidget::updateItemsListWidget");
}

void LayerWidget::checkVisibility(QListWidgetItem *item)
{
    qDebug("Enter LayerWidget::checkVisibility");
    if(view->getLayerList()[ui->listWidget_layers->currentRow()]->isVisible())
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

        if(cItem->text().contains("Line")) // If cItem jpsLineItem
        {
            jpsLineItem *line = layer->getLineItemList()[ui->listWidget_items->currentRow()];
            layer->removeLineFromLayer(line);
        }
    }

    updateItemsListWidget();
    qDebug("Leave LayerWidget::removeItemsButtonClicked");
}

void LayerWidget::highLight(QListWidgetItem *item)
{
    qDebug("Enter LayerWidget::highLight");
    if(ui->listWidget_layers->currentItem() == nullptr)
        return;

    auto *layer = view->getLayerList()[ui->listWidget_layers->currentRow()];
    QString text = item->text();

    if(text.contains("Line"))
    {
        // For wall, crossing, transition, track, hline
        auto *line = layer->getLineItemList()[ui->listWidget_items->currentRow()];
        view->select_line(line);
    }

    qDebug("Leave LayerWidget::highLight");
}

void LayerWidget::hideLayerButtonClicked()
{
    qDebug("Enter LayerWidget::hideLayerButtonClicked");
    if(ui->listWidget_layers->currentItem() == nullptr)
        return;

    auto *layer = view->getLayerList()[ui->listWidget_layers->currentRow()];

    view->unmark_all_lines();
    layer->setVisible(false);
    layer->hide();
    qDebug("Leave LayerWidget::hideLayerButtonClicked");
}

void LayerWidget::showLayerButtonClicked()
{
    qDebug("Enter LayerWidget::showLayerButtonClicked");
    if(ui->listWidget_layers->currentItem() == nullptr)
        return;

    auto *layer = view->getLayerList()[ui->listWidget_layers->currentRow()];

    view->unmark_all_lines();
    layer->setVisible(true);
    layer->show();
    qDebug("Leave LayerWidget::showLayerButtonClicked");
}
