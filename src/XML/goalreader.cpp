/***************************************************************
 *
 * \file goalreader.cpp
 * \date 2019-04-01
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
 * Read goals xml file
****************************************************************/

#include "goalreader.h"

GoalReader::GoalReader(jpsGraphicsView *view)
{
    currentView = view;
}

bool GoalReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if(xml.readNextStartElement())
    {
        if(xml.name() == QLatin1String("JPScore")
            && xml.attributes().value("version") == QLatin1String("0.8"))
        {
            readJPScore();
        } else{
            xml.raiseError(QObject::tr("The file is not an version 0.8 file."));
        }
    }

    return !xml.error();
}

void GoalReader::readJPScore()
{
    while (xml.readNextStartElement())
    {
        if(xml.name() == QLatin1String("goals"))
            readGoals();
        else
            xml.skipCurrentElement();
    }
}

void GoalReader::readGoals()
{
    while (xml.readNextStartElement())
    {
        if(xml.name() == QLatin1String("goal"))
            readGoal();
        else
            xml.skipCurrentElement();
    }
}

void GoalReader::readGoal()
{
    QPen currentPen;
    currentPen.setColor(Qt::darkGreen);
    currentPen.setCosmetic(true);
    currentPen.setWidth(0);

    int id;
    QString caption;
    QString final;
    QList<QPointF> vertexes;

    //now token is goal StartElement
    id = xml.attributes().value("id").toString().toInt();
    caption = xml.attributes().value("caption").toString();
    final = xml.attributes().value("final").toString();

    while (xml.readNextStartElement())
    {
        if (xml.name() == QLatin1String("polygon"))
        {
            continue;
        }else if (xml.name() == QLatin1String("vertex"))
        {
            QPointF point;
            point.setX(xml.attributes().value("px").toFloat());
            point.setY(xml.attributes().value("py").toFloat());

            vertexes.append(point);

            xml.readNext();// next token is end element, skip it
        } else
            xml.skipCurrentElement();
    }

    QRectF rect(vertexes[0], vertexes[2]);
    auto *rectItem = new QGraphicsRectItem;
    rectItem->setRect(rect);
    auto *goal = new JPSGoal(rectItem);

    goal->setId(id);
    goal->setCaption(caption);
    goal->setIsFinal(final);
    goal->setBeSaved("true");

    currentView->scene()->addItem(goal);
}