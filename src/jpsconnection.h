/**
 * \file       jpsconnection.h 
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

     
#ifndef JPSCONNECTION_H
#define JPSCONNECTION_H

#include <memory>
#include <utility>
#include "jpslandmark.h"




class jpsConnection
{

public:
    jpsConnection(jpsLandmark* landmark1=nullptr, jpsLandmark* landmark2=nullptr, QGraphicsLineItem* lineItem=nullptr, const int& frameID=-1);
    ~jpsConnection();
    std::pair<jpsLandmark*, jpsLandmark*> GetLandmarks() const;

    void SetLineItem(QGraphicsLineItem* lineItem);
    QGraphicsLineItem* GetLineItem();

    void SetFirstLandmark(jpsLandmark* landmark);
    void SetSecondLandmark(jpsLandmark* landmark);
    //Occurence
//    bool OccursInFrame(const int &frameID) const;
//    void SetFirstFrame(const int &frameID);
//    void SetLastFrame(const int &frameID);
//    const int& GetFirstFrame() const;
//    const int& GetLastFrame() const;
    // Operator overload
    bool operator ==(const jpsConnection& con2) const;
private:
    jpsLandmark* _landmark1;
    jpsLandmark* _landmark2;
    QGraphicsLineItem* _lineItem;
    int _firstFrame;
    int _lastFrame;


};

#endif // JPSCONNECTION_H
