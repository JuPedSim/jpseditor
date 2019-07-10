/**
 * \file       global.h
 * \date        Mar-13-2019
 * \version     v0.8.8
 * \author      Tao Zhong
 * \copyright   <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
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

#ifndef JPSEDITOR_GLOBAL_H
#define JPSEDITOR_GLOBAL_H


#include <QGraphicsItem>

bool isDirtyChange(QGraphicsItem::GraphicsItemChange change);

enum ZoneType {Room, Corridor, Office, Lobby, Entrance, Stair, Obstacle, Platform, NotAssigned};

enum LineType {wall, door, crossing, transition, hline, track, undefined};

enum DrawingMode {Selecting, Wall, Crossing, Hline, Landmark, Source, Goal, Transition, Track, MeasureLength};
#endif //JPSEDITOR_GLOBAL_H
