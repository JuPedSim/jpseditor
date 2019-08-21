/***************************************************************
 *
 * \file layer.cpp
 * \date 2019-08-21
 * \version v0.8.9
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
 * Layer class to manage items
 * Layer can contain QGraphicsLineitem(wall, crossing...), QGraphicsRechItem(Source and Goal) and QGraphicsPixmapItem
 * (jpsLandmark)
 * Model for layer
****************************************************************/

#include "layer.h"

Layer::Layer() : QGraphicsItemGroup(nullptr)
{
    name = "Layer";
    hide = true;
}

Layer::~Layer()
{

}

QString Layer::getName()
{
    qDebug("Enter/Leave Layer::getName");
    return name;
}

void Layer::setName(QString layername)
{
    qDebug("Enter Layer::setName");
    name = layername;
    qDebug("Leave Layer::setName");
}

void Layer::addToLayer(QGraphicsLineItem *item)
{
    qDebug("Enter Layer::addToLayer");

    if(lineItem_list.contains(item) || item == nullptr)
        return;

    lineItem_list.append(item);
    addToGroup(item); // Add to group to manage items
    qDebug("Leave Layer::addToLayer");
}

QList<QGraphicsLineItem *> Layer::getLineItemList()
{
    qDebug("Enter/Leave Layer::getItemLis");
    return lineItem_list;
}

bool Layer::isHide()
{
    qDebug("Enter/Leave Layer::isHide");
    return hide;
}

void Layer::removeLineItem(QGraphicsLineItem *line)
{
    qDebug("Enter Layer::removeLineItem");
    if(line == nullptr)
        return;

    removeFromGroup(line);
    lineItem_list.removeOne(line);

    qDebug("Leave Layer::removeLineItem");
}
