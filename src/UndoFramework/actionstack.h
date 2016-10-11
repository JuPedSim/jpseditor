/**
 * \file       actionstack.h 
 * \date        Mar 07, 2016
 * \version     v0.8.1
 * \copyright   <2009-2016> Forschungszentrum Jülich GmbH. All rights reserved.
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

     
#ifndef ACTIONSTACK_H
#define ACTIONSTACK_H

#include <QStack>
#include "action.h"
#include "lineaction.h"

class ActionStack
{
public:
    ActionStack();
    ~ActionStack();

    LineAction GetRecentAction();
    void PushNewAction(const LineAction& action);
    bool IsEmpty();
private:
    QStack<LineAction> _actionstack;
};

#endif // ACTIONSTACK_H
