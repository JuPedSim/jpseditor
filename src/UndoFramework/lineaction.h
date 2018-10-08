/**
 * \file       lineaction.h
 * \date        Oct-01-2018
 * \version     v0.8.4
 * \copyright   <2009-2018> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
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
 **/

     
#ifndef LINEACTION_H
#define LINEACTION_H

#include "action.h"
#include <QLine>

class LineAction : public Action
{
public:
    LineAction();
    LineAction(const QString& name, const QString& type, const int& itemID, const QLineF& oldLine);
    ~LineAction();

    QLineF GetOldLine() const;
    int GetItemID() const;
private:
    QLineF _oldLine;
    int _itemID;
};

#endif // PAINTACTION_H
