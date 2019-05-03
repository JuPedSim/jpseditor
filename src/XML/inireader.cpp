/***************************************************************
 *
 * \file inireader.cpp
 * \date 2019-05-01
 * \version v0.8.8
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
 * This class is used for read inifile in inifile widget
****************************************************************/

#include "inireader.h"

IniReader::IniReader(InifileWidget *inifileWidget)
{
    widget = inifileWidget;
}

bool IniReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("JuPedSim")
            && xml.attributes().value("version") == QLatin1String("0.8"))
        {
            readJuPedSim(widget);
        } else {
            xml.raiseError(QObject::tr("The file is not an JuPedSim version 0.8 file."));
        }
    }

    return !xml.error();
}

QString IniReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

void IniReader::readJuPedSim(InifileWidget *inifileWidget)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("JuPedSim"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("seed"))
            readSeed(widget);
        else if (xml.name() == QLatin1String("geometry"))
            qDebug("read geometry!");
        else if (xml.name() == QLatin1String("max_sim_time"))
            qDebug("read max sim time!");
        else
            xml.skipCurrentElement();
    }
}

void IniReader::readSeed(InifileWidget *widget)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("seed"));

    QString title = xml.readElementText();

    widget.
}
