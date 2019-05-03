/***************************************************************
 *
 * \file sourcereader.cpp
 * \date 2019-04-02
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
 * Read sources xml file
****************************************************************/

#include "sourcereader.h"

SourceReader::SourceReader(jpsGraphicsView *view)
{
    currentView = view;
}

bool SourceReader::read(QIODevice *device)
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

void SourceReader::readJPScore()
{
    while (xml.readNextStartElement())
    {
        if(xml.name() == QLatin1String("agents_sources"))
            readSources();
        else
            xml.skipCurrentElement();
    }
}

void SourceReader::readSources()
{
    while (xml.readNextStartElement())
    {
        if(xml.name() == QLatin1String("source"))
        {
            readSource();
        }
        else
            xml.skipCurrentElement();
    }
}

void SourceReader::readSource()
{
    QPen currentPen;
    currentPen.setColor(Qt::darkRed);
    currentPen.setCosmetic(true);
    currentPen.setWidth(0);

    int id = xml.attributes().value("id").toString().toInt();
    QString frequency = xml.attributes().value("frequency").toString();
    QString N_create = xml.attributes().value("N_create").toString();
    QString percent = xml.attributes().value("percent").toString();
    QString rate = xml.attributes().value("rate").toString();
    QString time_min = xml.attributes().value("time_min").toString();
    QString time_max = xml.attributes().value("time_max").toString();
    QString agents_max = xml.attributes().value("agents_max").toString();
    QString group_id = xml.attributes().value("group_id").toString();
    QString caption = xml.attributes().value("caption").toString();
    QString greedy = xml.attributes().value("greedy").toString();
    QString time = xml.attributes().value("time").toString();
    QString startX = xml.attributes().value("startX").toString();
    QString startY = xml.attributes().value("startY").toString();
    qreal x_min = xml.attributes().value("x_min").toFloat();
    qreal x_max = xml.attributes().value("x_max").toFloat();
    qreal y_min = xml.attributes().value("y_min").toFloat();
    qreal y_max = xml.attributes().value("y_max").toFloat();

    QRectF rect(QPointF(x_min, y_max),QPointF(x_max, y_min));
    QGraphicsRectItem *rectItem = new QGraphicsRectItem();
    rectItem->setRect(rect);
    JPSSource *source = new JPSSource(rectItem);

    source->setId(id);
    source->setFrequency(frequency);
    source->setN_create(N_create);
    source->setPercent(percent);
    source->setRate(rate);
    source->setTime_min(time_min);
    source->setTime_max(time_max);
    source->setAgents_max(agents_max);
    source->setGroup_id(group_id);
    source->setCaption(caption);
    source->setGreedy(greedy);
    source->setTime(time);
    source->setStartX(startX);
    source->setStartY(startY);
    source->setBeSaved(true);

    currentView->scene()->addItem(source);

    xml.readNext(); //now token is end element, skip it
}
