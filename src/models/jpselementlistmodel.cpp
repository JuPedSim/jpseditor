
/***************************************************************
 *
 * \file jpselementlistmodel.cpp
 * \date 2019-03-21
 * \version 0.8.8
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
 * this model is for all elements in jpseditor, which inherit from QGraphicsItem
 * A simple model that uses a QList<QGraphicsItem* > as its data source.
 * data() must be implement in subclasses
****************************************************************/

#include "jpselementlistmodel.h"

/*!
    Constructs a QGraohicsItem model with the given parent.
*/

JPSElementListModel::JPSElementListModel(QObject *parent)
        : QAbstractListModel(parent)
{
}

/*!
    Constructs a QGraohicsItem list model containing the specified sources
    with the given parent.
*/

JPSElementListModel::JPSElementListModel(QList<QGraphicsItem *> &items, QObject *parent)
        : QAbstractListModel(parent)
{

}

JPSElementListModel::~JPSElementListModel()
{
}

/*!
    since 0.8.8

    Returns the number of rows in the model. This value corresponds to the
    number of items in the model's internal sources list.

    The optional  parent argument is in most models used to specify
    the parent of the rows to be counted. Because this is a list if a
    valid parent is specified, the result will always be 0.
*/

int JPSElementListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return lst.count();
}


