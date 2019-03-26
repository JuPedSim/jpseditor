/**
 * \file       elementtype.h
 * \date        Mar-13-2018
 * \version     v0.8.8
 * \author      Tao Zhong
 * \copyright   <2009-209> Forschungszentrum Jülich GmbH. All rights reserved.
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

/*
 * This class define type for different custom QGraphicsItem classes.
 * */

#ifndef JPSEDITOR_ELEMENTTYPS_H
#define JPSEDITOR_ELEMENTTYPS_H

#include <QGraphicsItem>

const int SourceElementType = QGraphicsItem::UserType + 1;
const int GoalElementType = QGraphicsItem::UserType + 2;

#endif //JPSEDITOR_ELEMENTTYPS_H
