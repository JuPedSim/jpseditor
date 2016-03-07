/**
 * \file       jpsyahpointer.h 
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

     
#ifndef JPSYAHPOINTER_H
#define JPSYAHPOINTER_H
#include <QPointF>
#include <vector>
class jpsYAHPointer
{
public:
    jpsYAHPointer(const QPointF& point, const qreal& angle=0.0);
    jpsYAHPointer(const qreal& x, const qreal& y, const qreal& angle=0.0);
    ~jpsYAHPointer();

    const QPointF& GetPos() const;
    const qreal& GetDirection() const;

    void SetPos(const QPointF& point);
    void SetDirection(const qreal& angle);

    //Occurence
    const int& GetFirstFrame() const;
    const int& GetLastFrame() const;
    void SetFirstFrame(const int& frame);
    void SetLastFrame(const int& frame);

    //
    const QPointF& GetPosWhenFrame(const int& frame);
    const qreal& GetDirWhenFrame(const int& frame);

private:
    QPointF _cPos;
    qreal _cDirection;
    std::vector<QPointF> _allPos;
    std::vector<qreal> _allDir;

    //Occurence
    int _firstFrame;
    int _lastFrame;
};

#endif // JPSYAHPOINTER_H
