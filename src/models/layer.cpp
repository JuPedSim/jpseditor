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
 * Layer can contain jpsLineitem(wall, crossing...), JPSSource, JPSGoal and jpsLandmark
 * Model for layer
****************************************************************/

#include "layer.h"

Layer::Layer() : QGraphicsItemGroup(nullptr)
{
    name = "Layer";
    visible = true;
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


QList<jpsLineItem *> Layer::getLineItemList()
{
    qDebug("Enter/Leave Layer::getItemLis");
    return lineItem_list;
}

bool Layer::isVisible()
{
    qDebug("Enter/Leave Layer::isVisible");
    return visible;
}

void Layer::setVisible(bool is_visible)
{
    qDebug("Enter Layer::setVisible");
    visible = is_visible;
    qDebug("Leave Layer::setVisible");
}

// For wall, crossing, transition, track, hline
void Layer::addLineToLayer(jpsLineItem *wall)
{
    qDebug("Enter Layer::addLineToLayer");
    if(lineItem_list.contains(wall) || wall == nullptr)
        return;

    lineItem_list.append(wall);
    addToGroup(wall->get_line());
    qDebug("Leave Layer::addLineToLayer");
}

void Layer::removeLineFromLayer(jpsLineItem *line)
{
    qDebug("Enter Layer::removeLineFromLayer");
    if(line == nullptr)
        return;

    removeFromGroup(line->get_line());
    lineItem_list.removeOne(line);
    qDebug("Leave Layer::removeLineFromLayer");
}

