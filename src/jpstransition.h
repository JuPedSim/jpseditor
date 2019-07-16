/**
 * \file        jpsexit.h
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
 *
 * \section Description
 * This class is representing an exit to the outside.
 **/

#ifndef JPSEXIT_H
#define JPSEXIT_H

#include "jpszone.h"

class jpsTransition
{
public:
    jpsTransition(jpsLineItem *line);
    ~jpsTransition(){}
    QList<JPSZone *> get_roomList();
    QString get_name();
    jpsLineItem *get_cLine();
    void change_name(QString name);
    QString get_type();
    int get_id();
    void set_id(int id);
    void set_type(QString type);
    void set_rooms(JPSZone* room1, JPSZone* room2=nullptr);

private:
    QList<JPSZone *> roomList;
    QString cName;
    jpsLineItem *cLine;
    QString _type;
    int _id;

};

#endif // JPSEXIT_H
