
/***************************************************************
 *
 * \file jpssourcelistmodel.cpp
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
 * A simple model that uses a QList<JPSSource *> as its data source.
****************************************************************/

#include "jpssourcelistmodel.h"

/*!
    Constructs a JPSSource model with the given parent.
*/

JPSSourceListModel::JPSSourceListModel(QObject *parent)
    : JPSElementListModel(parent)
{
}

/*!
    Constructs a JPSSource list model containing the specified sources
    with the given parent.
*/

JPSSourceListModel::JPSSourceListModel(QList<JPSSource *> &lst, QObject *parent)
    : JPSElementListModel(parent), lst(lst)
{
}

/*!
    Returns data for the specified role, from the item with the given index.

    If the view requests an invalid index, an invalid variant is returned.

    Ovriride from QAbstractListModel
*/

QVariant JPSSourceListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= lst.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return lst.at(index.row())->getCaption();

    return QVariant();
}

/*!
    Sets the model's internal sources list to sources. The model will
    notify any attached views that its underlying data has changed.
*/

void JPSSourceListModel::setSourceList(QList<JPSSource *> sources)
{
    beginResetModel();
    lst = sources;
    endResetModel();
}

/*!
    Returns the sources list used by the model to store data.
*/

QList<JPSSource *> JPSSourceListModel::sourcesList() {
    return lst;
}

/*!
    since 0.8.8


    Returns the number of rows in the model. This value corresponds to the
    number of items in the model's internal sources list.

    The optional  parent argument is in most models used to specify
    the parent of the rows to be counted. Because this is a list if a
    valid parent is specified, the result will always be 0.

    Hide rowCount() in JPSElementListModel
*/

int JPSSourceListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return lst.count();
}
