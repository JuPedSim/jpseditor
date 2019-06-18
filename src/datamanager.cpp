/**
 * \file        datamanager.cpp
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
 * The class is responsible for handling and organisation of rooms, walls and doors. Further more
 * it handles the reading and writing of dxf and xml files.
 *
 **/


#include "datamanager.h"
#include <iostream>
#include <utility>
#include <chrono>
#include <QtWidgets>

using myClock = std::chrono::high_resolution_clock;


jpsDatamanager::jpsDatamanager(QWidget *parent, jpsGraphicsView *view)
{
    parent_widget=parent;
    _mView=view;
    room_id_counter=1;
    obs_id_counter=0;
    _crossingIdCounter=1;
    //_frameRate=0;
    _landmarkCounter=0;
    _regionCounter=0;

    myClock::duration d = myClock::now().time_since_epoch();

    auto seed = d.count();

    _generator=std::default_random_engine(seed);

    roomlist= QList<jpsRoom *> ();
    sourcelist = _mView->getSources();
    goallist = _mView->getGoals();
}

jpsDatamanager::~jpsDatamanager()
{
    qDebug("Enter jpsDatamanager::~jpsDatamanager");
    //remove_all(); vmiew->delete_all() does this work
    remove_all();
    qDebug("Leave jpsDatamanager::~jpsDatamanager");
}

void jpsDatamanager::new_room()
{
    qDebug("Enter jpsDatamanager::new_room. room_id_counter = %d", room_id_counter);
    jpsRoom* new_room = new jpsRoom(this->room_id_counter);
    roomlist.push_back(new_room);
    room_id_counter+=1;
    qDebug("Leave jpsDatamanager::new_room. room_id_counter = %d", room_id_counter);
}

void jpsDatamanager::remove_room(jpsRoom *room)
{
    qDebug("Enter jpsDatamanager::remove_room. room_id_counter = %d", room_id_counter);
    //set room to nullptr in doors
    for (jpsCrossing* crossing: get_crossingList())
    {
        // will only be removed if room is in crossings roomlist
        crossing->RemoveRoom(room);
    }

    //if (roomlist.size()>0)
    //{
    int roomID=room->get_id();
    roomlist.removeOne(room);
    delete room;
    room = nullptr;
    //change IDs of other room with ID greater than roomID
    for (jpsRoom* otherroom:roomlist)
        if (otherroom->get_id()>roomID)
            otherroom->set_id(otherroom->get_id()-1);

    room_id_counter-=1;

    //}
    qDebug("Leave jpsDatamanager::remove_room. room_id_counter = %d", room_id_counter);
}

void jpsDatamanager::change_roomName(jpsRoom* room, QString name)
{
     qDebug("Enter jpsDatamanager::change_roomName with name=<%s>", name.toStdString().c_str());
    room->change_name(name);
    qDebug("Leave jpsDatamanager::change_roomName");
}

void jpsDatamanager::remove_all_rooms()
{
     qDebug("Enter jpsDatamanager::remove_all_rooms");
    for (int i=0; i<roomlist.size(); i++)
    {
        delete roomlist[i];
        roomlist[i] = nullptr;
    }
    roomlist.clear();
    qDebug("Leave jpsDatamanager::remove_all_rooms");
}

QList<jpsRoom *> jpsDatamanager::get_roomlist()
{
    qDebug("Enter/return jpsDatamanager::get_roomlist");
    return this->roomlist;
}

QList<QString> jpsDatamanager::getElevationList()
{
    QList<QString> elevationlist;
    QList<jpsRoom *> roomlist = get_roomlist();

    QListIterator<jpsRoom *> i(roomlist);
    while (i.hasNext())
    {
        float elevation = i.next()->get_elevation();
        if (!elevationlist.contains(QString::number(elevation)) &&
                i.peekPrevious()->get_type() != "Stair")
        {
            elevationlist.append(QString::number(elevation));
        }

    }

    return elevationlist;
}

void jpsDatamanager::new_obstacle()
{
    qDebug("Enter: jpsDatamanager::new_obstacle. obs_id_counter = %d", obs_id_counter);
    jpsObstacle* new_obs = new jpsObstacle(this->obs_id_counter);
    obstaclelist.push_back(new_obs);
    obs_id_counter+=1;
    qDebug("Leave jpsDatamanager::new_obstacle obs_id_counte = %d", obs_id_counter);
}

void jpsDatamanager::remove_obstacle(jpsObstacle *obs)
{
     qDebug("Enter: jpsDatamanager::remove_obstacle. obs_id_counter = %d", obs_id_counter);
    if (obstaclelist.size()>0)
    {
        obstaclelist.removeOne(obs);
        delete obs;
        obs_id_counter-=1;
    }
    qDebug("Leave jpsDatamanager::remove_obstacle. obs_id_counter = %d", obs_id_counter);
}

void jpsDatamanager::change_obstacleName(jpsObstacle* obs, QString name)
{
     qDebug("Enter jpsDatamanager::change_obstacleName with name=<%s>", name.toStdString().c_str());
    obs->change_name(name);
    qDebug("Leave jpsDatamanager::change_obstacleName");
}

void jpsDatamanager::remove_all_obstacles()
{
    qDebug("Enter jpsDatamanager::remove_all_obstacles");
    for (int i=0; i<obstaclelist.size(); i++)
    {
        delete obstaclelist[i];
    }
    obstaclelist.clear();
    qDebug("Leave jpsDatamanager::remove_all_obstacles");
}

QList<jpsObstacle *> jpsDatamanager::get_obstaclelist()
{
    qDebug("Enter/Return jpsDatamanager::get_obstaclelist");
    return obstaclelist;
}

QList<jpsCrossing *> jpsDatamanager::get_crossingList()
{
    qDebug("Enter/Return jpsDatamanager::get_crossinglist");
    return crossingList;
}

void jpsDatamanager::new_crossing(QList <jpsLineItem *> newCrossings)
{
    qDebug("Enter jpsDatamanager::new_crossing QList");
    for (int i=0; i<newCrossings.size(); i++)
    {
        if ((newCrossings[i]->is_Door() || newCrossings[i]->is_Exit())
        && !isInCrossingList(newCrossings[i]))// only door can be crossing
        {
            auto newCrossing = new jpsCrossing(newCrossings[i]);

            newCrossing->set_id(_crossingIdCounter);
            _crossingIdCounter += 1;

            crossingList.push_back(newCrossing);
        }
    }
    qDebug("Leave jpsDatamanager::new_crossing QList");
}

void jpsDatamanager::new_crossing(jpsLineItem *newCrossing)
{
    qDebug("Enter jpsDatamanager::new_crossing");
    if (newCrossing->is_Door() && !isInCrossingList(newCrossing))
    {
        jpsCrossing* newCros = new jpsCrossing(newCrossing);
        newCros->set_id(_crossingIdCounter);
        _crossingIdCounter += 1;
        crossingList.push_back(newCros);
    }
    qDebug("Leave jpsDatamanager::new_crossing");
}

bool jpsDatamanager::isInCrossingList(jpsLineItem *markedLine)
{
    qDebug("Enter jpsDatamanager::isInCrossingList");
    for (jpsCrossing* crossing:crossingList)
    {
        if (crossing->get_cLine()==markedLine)
        {
            return true;
        }
    }
    return false;
    qDebug("Leave jpsDatamanager::isInCrossingList");
}

void jpsDatamanager::remove_crossing(jpsCrossing *crossing)
{
     qDebug("Enter jpsDatamanager::remove_crossing. crossingList.size() = %d", crossingList.size());
    if (crossingList.size()>0)
    {
        for (jpsRoom * room:roomlist)
        {
            // door will only be removed if belongs to room (see method removeDoor of jpsRoom)
            room->removeDoor(crossing);
        }
        crossingList.removeOne(crossing);
        delete crossing;
    }
    qDebug("Leave jpsDatamanager::remove_crossing. crossingList.size() = %d", crossingList.size());
}

void jpsDatamanager::change_crossingName(jpsCrossing *crossing, QString name)
{
    qDebug("Enter jpsDatamanager::change_crossingName with name = <%s>",
           name.toStdString().c_str());
    crossingList[crossingList.indexOf(crossing)]->change_name(name);
    qDebug("Leave jpsDatamanager::change_crossingName");
}

void jpsDatamanager::remove_all_crossings()
{
     qDebug("Enter jpsDatamanager::remove_all_crossings");
    for (int i=0; i<crossingList.size(); i++)
    {
        delete crossingList[i];
    }
    crossingList.clear();
    qDebug("Leave jpsDatamanager::remove_all_crossings");
}


QList<jpsExit *> jpsDatamanager::get_exitList()
{
     qDebug("Enter/Return jpsDatamanager::get_exitList");
    return exitList;
}

void jpsDatamanager::new_exit(QList <jpsLineItem *> newExits)
{
    qDebug("Enter jpsDatamanager::new_exit QList");
    for (int i=0; i<newExits.size(); i++)
    {
        //if (newExits[i]->is_Exit())
        //{
        jpsExit* newExit = new jpsExit(newExits[i]);
        exitList.push_back(newExit);
        //}
    }
    qDebug("Leave jpsDatamanager::new_exit QList");
}

void jpsDatamanager::new_exit(jpsLineItem *newExit)
{
    qDebug("Enter jpsDatamanager::new_exit");
    jpsExit* newEx = new jpsExit(newExit);
    exitList.push_back(newEx);
    qDebug("Leave jpsDatamanager::new_exit");
}

void jpsDatamanager::remove_exit(jpsExit *exit)
{
     qDebug("Enter jpsDatamanager::remove_exit");
    if (exitList.size()>0)
    {
        exitList.removeOne(exit);
        delete exit;
    }
    qDebug("Leave jpsDatamanager::remove_exit");
}

void jpsDatamanager::remove_all_exits()
{
     qDebug("Enter jpsDatamanager::remove_all_exits");
    for (int i=0; i<exitList.size(); i++)
    {
        delete exitList[i];
    }
    exitList.clear();
    qDebug("Leave jpsDatamanager::remove_all_exits");
}

QList<jpsLandmark *> jpsDatamanager::get_landmarks()
{
    return _landmarks;
}

void jpsDatamanager::new_landmark(jpsLandmark *newlandmark)
{
    newlandmark->SetId(_landmarkCounter);
    _landmarkCounter++;
    _landmarks.push_back(newlandmark);
}

void jpsDatamanager::remove_landmark(jpsLandmark *landmark)
{
    _landmarks.removeOne(landmark);
    for (jpsConnection* connection:landmark->GetConnections())
    {
        RemoveConnection(connection);
    }
    delete landmark;
    _landmarkCounter++;
}

void jpsDatamanager::change_LandmarkName(jpsLandmark *landmark, QString name)
{
    landmark->SetCaption(name);
}

void jpsDatamanager::remove_all_landmarks()
{
    for (jpsLandmark* landmark:_landmarks)
    {
        delete landmark;
    }
    _landmarks.clear();
}

const int &jpsDatamanager::GetLandmarkCounter() const
{
    return _landmarkCounter;
}

const QList<jpsConnection *> &jpsDatamanager::GetAllConnections() const
{
    return _landmarkConnections;
}

void jpsDatamanager::NewConnection(jpsConnection *newConnection)
{
    _landmarkConnections.push_back(newConnection);
}

void jpsDatamanager::RemoveConnection(jpsConnection *connection)
{
    _landmarkConnections.removeOne(connection);
    connection->GetLandmarks().first->RemoveConnection(connection);
    connection->GetLandmarks().second->RemoveConnection(connection);
    delete connection->GetLineItem();
    delete connection;
}

void jpsDatamanager::RemoveAllConnections()
{
    for (jpsConnection* connection:_landmarkConnections)
    {
        delete connection->GetLineItem();
        delete connection;

        //remove also from _ConnectionsAfterLandmarkLoose
        if (_ConnectionsAfterLandmarkLoose.contains(connection))
        {
            _ConnectionsAfterLandmarkLoose.removeOne(connection);
        }
    }
    _landmarkConnections.clear();

    for (jpsConnection* connection:_ConnectionsAfterLandmarkLoose)
    {
        delete connection;
    }
    _ConnectionsAfterLandmarkLoose.clear();
}

const QList<jpsRegion *> &jpsDatamanager::GetRegions() const
{
    return _regions;
}

void jpsDatamanager::NewRegion(jpsRegion *region)
{
    _regions.push_back(region);
    _regionCounter++;
}

void jpsDatamanager::RemoveRegion(jpsRegion *region)
{
    _regions.removeOne(region);
    delete region;

}

void jpsDatamanager::RemoveAllRegions()
{
    for (jpsRegion* region:_regions)
    {
        delete region;
    }
    _regions.clear();
}

const int &jpsDatamanager::GetRegionCounter() const
{
    return _regionCounter;
}

void jpsDatamanager::writeXML(QFile &file)
{
    qDebug(">> Enter jpsDatamanager::writeXML");
    auto *stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = _mView->get_line_vector();

    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    // Header for geometry
    writeHeader(stream);

    if(check_printAbility().isEmpty()) // crossing, transitions, obstacles will be checked.
    {
        //write rooms and crossings
        stream->writeStartElement("rooms");
        writeRooms(stream,lines);
        stream->writeEndElement(); // End rooms

        stream->writeStartElement("transitions");
        writeTransitions(stream,lines);
        stream->writeEndElement();// End transitions
    }
    stream->writeStartElement("Undefine");
    writeNotAssignedDoors(stream,lines);
    writeNotAssignedWalls(stream,lines);
    stream->writeEndElement();

    stream->writeEndElement();//geometry

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
    qDebug("<< Leave jpsDatamanager::writeXML");
}



void jpsDatamanager::writeRoutingXML(QFile &file) // Construction side
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > hLines;

    for (jpsLineItem* line:_mView->get_line_vector())
    {
        if (line->IsHLine())
        {
            hLines.push_back(line);
        }
    }


    writeRoutingHeader(stream);

    stream->writeStartElement("Hlines");
    writeHLines(stream,hLines);
    stream->writeEndElement();//Hlines

    stream->writeEndDocument();

    delete stream;

}

void jpsDatamanager::writeLineItems(QFile &file)
{

    for (jpsLineItem* line:_mView->get_line_vector())
    {
        QString string = QString::number(line->get_line()->line().p1().x())+ "," + QString::number(line->get_line()->line().p1().y())+ ";\n"+
                QString::number(line->get_line()->line().p2().x())+ "," + QString::number(line->get_line()->line().p2().y())+ "; \n";
        file.write(string.toUtf8());
    }
}

void jpsDatamanager::WriteCognitiveMapXML(QFile &file, bool fuzzy)
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    WriteCognitiveMapHeader(stream);

    //write regions incl. their landmarks and connections
    stream->writeStartElement("regions");
    WriteRegions(stream,fuzzy);
    stream->writeEndElement();//regions

    stream->writeEndElement();//cognitiveMap

    stream->writeEndDocument();

    delete stream;
}

void jpsDatamanager::WriteCognitiveMapXML(QFile &file, int k, double m, double p0)
{
    QXmlStreamWriter stream(&file);
    WriteCognitiveMapHeader(&stream);

    //write regions incl. their landmarks and connections
    stream.writeStartElement("regions");
    WriteRegions(&stream,k,m,p0);
    stream.writeEndElement();//regions

    stream.writeEndElement();//cognitiveMap

    stream.writeEndDocument();
}

void jpsDatamanager::WriteCognitiveMapHeader(QXmlStreamWriter *stream)
{
    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("cognitiveMap");
    stream->writeAttribute("version", "0.81");
    stream->writeAttribute("caption","cogMap");
    stream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/jps_geometry.xsd");
    stream->writeAttribute("unit","m");
}

void jpsDatamanager::WriteRegions(QXmlStreamWriter *stream, bool fuzzy)
{
    for (jpsRegion* region:_regions)
    {
        stream->writeStartElement("region");

        int id = region->GetId();
        QString caption = region->GetCaption();
        qreal px = region->GetPos().x();
        qreal py = region->GetPos().y();
        qreal a = region->GetA();
        qreal b = region->GetB();

//        if (fuzzy)
//        {
//            px = MakeItFuzzy(px,a/2.0);
//            py = MakeItFuzzy(py,b/2.0);
//            a = MakeItFuzzy(a,a/2.0);
//            if (a<0.5)
//                a=0.5;
//            b= MakeItFuzzy(b,b/2.0);
//            if (b<0.5)
//                b=0.5;
//        }


        stream->writeAttribute("id",QString::number(id));
        stream->writeAttribute("caption",caption);
        stream->writeAttribute("px",QString::number(px));
        stream->writeAttribute("py",QString::number(py));
        stream->writeAttribute("a",QString::number(a));
        stream->writeAttribute("b",QString::number(b));


        stream->writeStartElement("landmarks");
        WriteLandmarks(region,stream,fuzzy);
        stream->writeEndElement();//landmarks

        stream->writeStartElement("connections");
        WriteConnections(region,stream);
        stream->writeEndElement();//connections

        stream->writeEndElement();//region
    }
}

void jpsDatamanager::WriteRegions(QXmlStreamWriter *stream, int k, double m, double p0)
{
    for (jpsRegion* region:_regions)
    {
        stream->writeStartElement("region");

        int id = region->GetId();
        QString caption = region->GetCaption();
        qreal px = region->GetPos().x();
        qreal py = region->GetPos().y();
        qreal a = region->GetA();
        qreal b = region->GetB();


        stream->writeAttribute("id",QString::number(id));
        stream->writeAttribute("caption",caption);
        stream->writeAttribute("px",QString::number(px));
        stream->writeAttribute("py",QString::number(py));
        stream->writeAttribute("a",QString::number(a));
        stream->writeAttribute("b",QString::number(b));


        stream->writeStartElement("landmarks");
        WriteLandmarks(region,stream,k,m,p0);
        stream->writeEndElement();//landmarks

        stream->writeStartElement("connections");
        WriteConnections(region,stream);
        stream->writeEndElement();//connections

        stream->writeEndElement();//region
    }
}

void jpsDatamanager::AutoSaveXML(QFile &file)
{
    qDebug("Enter jpsDatamanager::AutoSaveXML");
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = _mView->get_line_vector();

    writeHeader(stream);
    stream->writeStartElement("rooms");
    AutoSaveRooms(stream,lines);
    stream->writeEndElement();//rooms

    stream->writeStartElement("transitions");
    writeTransitions(stream,lines);
    stream->writeEndElement();//transitions

//    stream->writeStartElement("landmarks");
//    writeLandmarks(stream,landmarks);
//    stream->writeEndElement();//landmarks

    stream->writeEndElement();//geometry

    stream->writeEndDocument();
    qDebug("Leave jpsDatamanager::AutoSaveXML");
    delete stream;
}

void jpsDatamanager::writeHeader(QXmlStreamWriter *stream)
{
    qDebug("Enter jpsDatamanager::writeHeader");

    stream->writeStartElement("geometry");
    stream->writeAttribute("version", "0.8");
    stream->writeAttribute("caption","corner");
    stream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/jps_geometry.xsd");
    stream->writeAttribute("unit","m");
    qDebug("Leave jpsDatamanager::writeHeader");
}

void jpsDatamanager::writeRoutingHeader(QXmlStreamWriter *stream)
{
    qDebug("Enter jpsDatamanager::writeRoutingHeader");
    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("routing");
    stream->writeAttribute("version", "0.8");
    //stream->writeAttribute("caption","corner");
    stream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/jps_routing.xsd");
    stream->writeAttribute("unit","m");
    qDebug("Leave jpsDatamanager::writeRoutingHeader");
}

void jpsDatamanager::writeHLines(QXmlStreamWriter *stream, QList<jpsLineItem *> &hLines)
{
    // to be sure that id is unique (considering crossings, transitions)
    int id=1000;
    QString rid;

    for (jpsLineItem *lineItem:hLines)
    {
        stream->writeStartElement("Hline");
        stream->writeAttribute("id",QString::number(id));
        stream->writeAttribute("room_id","0");
        rid = RoomIDHLine(lineItem);
//        if (rid.contains("Warning"))
//        {
//            QMessageBox::critical(_mView,
//                                  "WriteHLines",
//                                  rid,
//                                  QMessageBox::Ok);
//        }
        stream->writeAttribute("subroom_id",rid);

        //Vertices
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(lineItem->get_line()->line().p1().x()));
        stream->writeAttribute("py",QString::number(lineItem->get_line()->line().p1().y()));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(lineItem->get_line()->line().p2().x()));
        stream->writeAttribute("py",QString::number(lineItem->get_line()->line().p2().y()));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //Hline
        id++;
    }

    if (rid.contains("Warning"))
    {
        QMessageBox::critical(_mView,
                              "WriteHLines",
                              rid,
                              QMessageBox::Ok);
    }
}

QString jpsDatamanager::RoomIDHLine(jpsLineItem *lineItem)
{

    for (jpsRoom* room:roomlist)
    {
        room->IdentifyInnerOuter();
        QPolygonF rPolygon = room->RoomAsSortedPolygon(room->GetOuterPolygon());
        if (rPolygon.containsPoint(lineItem->get_line()->line().p1(),Qt::OddEvenFill) ||
                rPolygon.contains(lineItem->get_line()->line().p1()))
        {
            if (rPolygon.containsPoint(lineItem->get_line()->line().p2(),Qt::OddEvenFill)||
                    rPolygon.contains(lineItem->get_line()->line().p2()))
                return QString::number(room->get_id());
            else
                return "Warning! HLine intersects walls";
        }
//        else if (rPolygon.containsPoint(lineItem->get_line()->line().p2(),Qt::OddEvenFill)||
//                 rPolygon.contains(lineItem->get_line()->line().p2()))
//        {
//            if (rPolygon.containsPoint(lineItem->get_line()->line().p1(),Qt::OddEvenFill)||
//                    rPolygon.contains(lineItem->get_line()->line().p1()))
//                return QString::number(room->get_id());
//            else
//                return "Warning! HLine intersects walls";
//        }

    }
    return "Warning! HLine outside geometry";

}

void jpsDatamanager::writeRooms(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    qDebug(" Enter jpsDatamanager::writeRooms");

    // write stair at first
    for (jpsRoom* room:roomlist)
    {
        if (room->get_type()=="Stair") // rooms as stairs
        {
            stream->writeStartElement("room");
            stream->writeAttribute("id", QString::number(room->get_id()));
            stream->writeAttribute("caption","stair");

            writeSubRoom(stream,room,lines);

            // A stair hasn't crossings
            stream->writeStartElement("crossings");
            stream->writeEndElement(); //crossings

            stream->writeEndElement(); //room
        }
    }

    // write all subrooms in a room which ID is 0
    stream->writeStartElement("room");
    stream->writeAttribute("id", "0");
    stream->writeAttribute("caption","floor");

    for (int i=0; i<roomlist.size(); i++)
    {
        if (roomlist[i]->get_type()!="Stair")
        {
            writeSubRoom(stream,roomlist[i],lines);
        }
    }
    //Crossings
    writeCrossings(stream,lines);

    stream->writeEndElement();//room

    qDebug(" Leave jpsDatamanager::writeRooms");
}

void jpsDatamanager::writeSubRoom(QXmlStreamWriter *stream, jpsRoom *room, QList<jpsLineItem *> &lines)
{
    stream->writeStartElement("subroom");
    stream->writeAttribute("id",QString::number(room->get_id()));
    stream->writeAttribute("caption",room->get_name());
    stream->writeAttribute("class",room->get_type());
    room->correctPlaneCoefficients();
    stream->writeAttribute("A_x",QString::number(room->get_ax()));
    stream->writeAttribute("B_y",QString::number(room->get_by()));
    stream->writeAttribute("C_z",QString::number(room->get_cz()));

    //walls
    QList<jpsLineItem* > wallList=room->get_listWalls();
    for (int j=0; j<wallList.size(); ++j)
    {
        stream->writeStartElement("polygon");
        stream->writeAttribute("caption","wall");

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y1()));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y2()));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //polygon

        //remove wall from lines
        lines.removeOne(wallList[j]);

    }

    for (int k=0; k<obstaclelist.size(); k++)
    {
        if (room==obstaclelist[k]->get_room())
        {
            writeObstacles(stream ,obstaclelist[k],lines);
        }
    }

    // if stair write up and down
    if (room->get_type() == "Stair"){
         // <up>
         stream->writeStartElement("up");
         stream->writeAttribute("px",QString::number( room->get_up().x()  ));
         stream->writeAttribute("py",QString::number( room->get_up().y()  ));
         stream->writeEndElement();
         // </up>
         // <down>
         stream->writeStartElement("down");
         stream->writeAttribute("px",QString::number( room->get_down().x()  ));
         stream->writeAttribute("py",QString::number( room->get_down().y()  ));
         stream->writeEndElement();
         // </down>
    }
    stream->writeEndElement();//subroom
}

void jpsDatamanager::AutoSaveRooms(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
     qDebug("Enter jpsDatamanager::AutoSaveRooms");
    ///rooms
    stream->writeStartElement("room");
    stream->writeAttribute("id","0");
    stream->writeAttribute("caption","hall");

    for (int i=0; i<roomlist.size(); i++)
    {
        stream->writeStartElement("subroom");
        stream->writeAttribute("id",QString::number(roomlist[i]->get_id()));
        stream->writeAttribute("caption",roomlist[i]->get_name());
        stream->writeAttribute("class","subroom");

        //walls
        QList<jpsLineItem* > wallList=roomlist[i]->get_listWalls();
        for (int j=0; j<wallList.size(); j++)
        {
            stream->writeStartElement("polygon");
            stream->writeAttribute("caption","wall");

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x1()));
            stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y1()));
            stream->writeEndElement(); //vertex

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x2()));
            stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y2()));
            stream->writeEndElement(); //vertex

            stream->writeEndElement(); //polygon

            ///remove wall from lines
            lines.removeOne(wallList[j]);
        }

        //polygonzug
        /*
        QList<QPointF> vertices = roomlist[i]->get_vertices();

        for (int j=0; j<vertices.size(); j++)
        {

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(vertices[j].x()));
            stream->writeAttribute("py",QString::number(vertices[j].y()));
            stream->writeEndElement(); //vertex
        }*/

        for (int k=0; k<obstaclelist.size(); k++)
        {
            if (roomlist[i]==obstaclelist[k]->get_room())
            {
                writeObstacles(stream ,obstaclelist[k],lines);
            }
        }

        stream->writeEndElement();//subroom
    }
    // Not assigned lines
    writeNotAssignedWalls(stream,lines);

    //Crossings
    writeCrossings(stream,lines);

    // Not assigned doors
    writeNotAssignedDoors(stream,lines);
    stream->writeEndElement();//crossings

    stream->writeEndElement();//room
    qDebug("Leave jpsDatamanager::AutoSaveRooms");
}

void jpsDatamanager::writeCrossings(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    qDebug("Enter jpsDatamanager::writeCrossings");
    stream->writeStartElement("crossings");

    for (int i=0; i<crossingList.size(); i++)
    {
        if (!crossingList[i]->IsExit()
        && crossingList[i]->get_roomList().size() == 2 // A crossing must between two subrooms or romm and stair
        && !(crossingList[i]->get_roomList()[0]->get_type()=="Stair"
        && crossingList[i]->get_roomList()[1]->get_type()=="Stair")) // both sides can't be stair at the same time
        {
            stream->writeStartElement("crossing");
            stream->writeAttribute("id",QString::number(i));

            stream->writeAttribute("subroom1_id",QString::number(crossingList[i]->get_roomList()[0]->get_id()));
            stream->writeAttribute("subroom2_id",QString::number(crossingList[i]->get_roomList()[1]->get_id()));

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossingList[i]->get_cLine()->get_line()->line().x1()));
            stream->writeAttribute("py",QString::number(crossingList[i]->get_cLine()->get_line()->line().y1()));
            stream->writeEndElement(); //vertex

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossingList[i]->get_cLine()->get_line()->line().x2()));
            stream->writeAttribute("py",QString::number(crossingList[i]->get_cLine()->get_line()->line().y2()));
            stream->writeEndElement(); //vertex

            stream->writeEndElement(); //crossing

            lines.removeOne(crossingList[i]->get_cLine());
        }
        else
        {
            //TODO: Exception handling, error return to event log
//            this->new_exit(crossingList[i]->get_cLine());
//            if (crossingList[i]->get_roomList().size()>1)
//            {
//                // mention stair id first
//                if (crossingList[i]->get_roomList()[0]->get_type()=="Stair")
//                    exitList.back()->set_rooms(crossingList[i]->get_roomList()[0], crossingList[i]->get_roomList()[1]);
//                else
//                    exitList.back()->set_rooms(crossingList[i]->get_roomList()[1], crossingList[i]->get_roomList()[0]);
//            }
//            else
//                exitList.back()->set_rooms(crossingList[i]->get_roomList()[0]);
        }

    }

    stream->writeEndElement();//crossings
    qDebug("Leave jpsDatamanager::writeCrossings");
}

void jpsDatamanager::writeTransitions(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    qDebug("Enter jpsDatamanager::writeTransitions");

    stream->writeStartElement("transition");

    for (jpsCrossing* crossing:crossingList)
    {
        if(crossing->IsExit()) // Only transitions will be wrote here!
        {
            stream->writeAttribute("id",QString::number(crossing->get_id()));
            stream->writeAttribute("caption","exit");
            stream->writeAttribute("type","emergency");

            if (crossing->get_roomList().size()==1
            && crossing->get_roomList()[0]->get_type()=="Stair") // stair - outside
            {
                //stair id
                stream->writeAttribute("room1_id",QString::number(crossing->get_roomList()[0]->get_id()));
                stream->writeAttribute("subroom1_id",QString::number(crossing->get_roomList()[0]->get_id()));
                //floor id
                stream->writeAttribute("room2_id","-1");
                stream->writeAttribute("subroom2_id","-1");
            }
            else if(crossing->get_roomList().size()==1
            && crossing->get_roomList()[0]->get_type()!="Stair")// room - outside
            {
                //room id
                stream->writeAttribute("room1_id","0");
                stream->writeAttribute("subroom1_id",QString::number(crossing->get_roomList()[0]->get_id()));
                //outside
                stream->writeAttribute("room2_id","-1");
                stream->writeAttribute("subroom2_id","-1");
            }
            else if (crossing->get_roomList().size()==2)  // stair - room or room - stair
            {
                //stair id
                stream->writeAttribute("room1_id",QString::number(crossing->get_roomList()[0]->get_id()));
                stream->writeAttribute("subroom1_id",QString::number(crossing->get_roomList()[0]->get_id()));
                //room2 id
                stream->writeAttribute("room2_id","0");
                stream->writeAttribute("subroom2_id",QString::number(crossing->get_roomList()[1]->get_id()));
            } else
            {
                //TODO: Exception handling, error report to event log
            }

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossing->get_cLine()->get_line()->line().x1()));
            stream->writeAttribute("py",QString::number(crossing->get_cLine()->get_line()->line().y1()));
            stream->writeEndElement(); //vertex

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossing->get_cLine()->get_line()->line().x2()));
            stream->writeAttribute("py",QString::number(crossing->get_cLine()->get_line()->line().y2()));
            stream->writeEndElement(); //vertex

            stream->writeEndElement(); //transition
            lines.removeOne(crossing->get_cLine());
        }
    }
    qDebug("Leave jpsDatamanager::writeTransitions");
}

void jpsDatamanager::writeObstacles(QXmlStreamWriter *stream, jpsObstacle* obs, QList<jpsLineItem* >& lines)
{
    stream->writeStartElement("obstacle");
    stream->writeAttribute("id",QString::number(obs->get_id()));
    stream->writeAttribute("caption",obs->get_name());
    stream->writeAttribute("closed","1");
    //stream->writeAttribute("height","1.0");  // height not implemented yet!

    //walls

    QList<jpsLineItem* > wallList = obs->get_listWalls();

    for (int j=0; j<wallList.size(); j++)
    {
        stream->writeStartElement("polygon");

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y1()));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y2()));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //polygon

        lines.removeOne(wallList[j]);
    }

    stream->writeEndElement(); //obstacle

}

void jpsDatamanager::writeNotAssignedWalls(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
     qDebug("Enter jpsDatamanager::writeNotAssignedWalls");

    /// save lines which are not assigned to a room yet
    QList<jpsLineItem *> walls;

    for (jpsLineItem* line:lines)
    {
        if (line->is_Wall())
        {
            walls.push_back(line);
        }
    }
    if (walls.isEmpty())
        return;

    stream->writeStartElement("subroom");
    stream->writeAttribute("id",QString::number(-1));
    stream->writeAttribute("caption","not assigned lines");
    stream->writeAttribute("class","container");

    for (jpsLineItem* line:walls)
    {
        stream->writeStartElement("polygon");
        stream->writeAttribute("caption","wall");

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(line->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(line->get_line()->line().y1()));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(line->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(line->get_line()->line().y2()));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //polygon

        lines.removeOne(line);
    }
    stream->writeEndElement();//subroom
    qDebug("Leave jpsDatamanager::writeNotAssignedWalls");
}

void jpsDatamanager::writeNotAssignedDoors(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
     qDebug("Enter jpsDatamanager::writeNotAssignedDoors");
    for (jpsLineItem* line:lines)
    {
        if (line->is_Door() && !isInCrossingList(line))
        {
            stream->writeStartElement("crossing");

            stream->writeAttribute("id",QString::number(-2));
            stream->writeAttribute("subroom1_id","-2");
            stream->writeAttribute("subroom2_id","-2");
            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(line->get_line()->line().x1()));
            stream->writeAttribute("py",QString::number(line->get_line()->line().y1()));
            stream->writeEndElement(); //vertex
            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(line->get_line()->line().x2()));
            stream->writeAttribute("py",QString::number(line->get_line()->line().y2()));
            stream->writeEndElement();//vertex

            stream->writeEndElement();//crossing

            lines.removeOne(line);
        }
    }
    qDebug("Leave jpsDatamanager::writeNotAssignedDoors");
}

void jpsDatamanager::writeNotAssignedExits(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{

    for (jpsLineItem* line:lines)
    {
        stream->writeStartElement("transition");

        stream->writeAttribute("id",QString::number(-2));
        stream->writeAttribute("caption","main exit");
        stream->writeAttribute("type","emergency");
        stream->writeAttribute("room1_id","0");
        stream->writeAttribute("subroom1_id","-2");
        stream->writeAttribute("room2_id","-1");
        stream->writeAttribute("subroom2_id","-2");
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(line->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(line->get_line()->line().y1()));
        stream->writeEndElement(); //vertex
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(line->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(line->get_line()->line().y2()));
        stream->writeEndElement();//vertex

        stream->writeEndElement();//transition
    }

}


void jpsDatamanager::WriteLandmarks(jpsRegion* cRegion, QXmlStreamWriter *stream, bool fuzzy)
{

    //cut some landmarks and/or their connections
    _ConnectionsAfterLandmarkLoose=_landmarkConnections;
    _landmarksAfterLoose=_landmarks;
    if (fuzzy)
        CutOutLandmarks();
    
    for (jpsLandmark* landmark:_landmarksAfterLoose)
    {
        if (landmark->GetRegion()==cRegion)
        {
            int id = landmark->GetId();
            QString caption = landmark->GetCaption();
            QString type = landmark->GetType();
            if (type=="Main Target")
                type="main";
            int id_subroom;
            if (landmark->GetRoom()!=nullptr)
                id_subroom = landmark->GetRoom()->get_id();
            else
                id_subroom = -1;
            qreal pxreal = landmark->GetRealPos().x();
            qreal pyreal = landmark->GetRealPos().y();
            qreal px = landmark->GetPos().x();
            qreal py = landmark->GetPos().y();
            qreal a = landmark->GetA();
            qreal b = landmark->GetB();

//            if (fuzzy)
//            {
//                px = MakeItFuzzy(px,a/5.0);
//                py = MakeItFuzzy(py,b/5.0);
//                a = MakeItFuzzy(a,a/3.0);
//                if (a<0.5)
//                    a=0.5;
//                b = MakeItFuzzy(b,b/3.0);
//                if (b<0.5)
//                    b=0.5;
//            }


            stream->writeStartElement("landmark");

            stream->writeAttribute("id",QString::number(id));
            stream->writeAttribute("caption",caption);
            stream->writeAttribute("type",type);
            stream->writeAttribute("room1_id","0");
            if (landmark->GetRoom()!=nullptr)
                stream->writeAttribute("subroom1_id",QString::number(id_subroom));
            else
                stream->writeAttribute("subroom1_id","NaN");
            stream->writeAttribute("pxreal",QString::number(pxreal));
            stream->writeAttribute("pyreal",QString::number(pyreal));
            stream->writeAttribute("px",QString::number(px));
            stream->writeAttribute("py",QString::number(py));
            stream->writeAttribute("a",QString::number(a));
            stream->writeAttribute("b",QString::number(b));

            stream->writeStartElement("associations");
            //
            stream->writeEndElement();//associations
            stream->writeEndElement();//landmark
        }

    }


}

void jpsDatamanager::WriteLandmarks(jpsRegion *cRegion, QXmlStreamWriter *stream, int k, double m, double p0)
{
    //cut some landmarks and/or their connections
    _ConnectionsAfterLandmarkLoose=_landmarkConnections;
    _landmarksAfterLoose=_landmarks;

    CutOutLandmarks(k,m,p0);

    for (jpsLandmark* landmark:_landmarksAfterLoose)
    {
        if (landmark->GetRegion()==cRegion)
        {
            int id = landmark->GetId();
            QString caption = landmark->GetCaption();
            QString type = landmark->GetType();
            if (type=="Main Target")
                type="main";
            int id_subroom;
            if (landmark->GetRoom()!=nullptr)
                id_subroom = landmark->GetRoom()->get_id();
            else
                id_subroom = -1;
            qreal pxreal = landmark->GetRealPos().x();
            qreal pyreal = landmark->GetRealPos().y();
            qreal px = landmark->GetPos().x();
            qreal py = landmark->GetPos().y();
            qreal a = landmark->GetA();
            qreal b = landmark->GetB();

            stream->writeStartElement("landmark");

            stream->writeAttribute("id",QString::number(id));
            stream->writeAttribute("caption",caption);
            stream->writeAttribute("type",type);
            stream->writeAttribute("room1_id","0");
            if (landmark->GetRoom()!=nullptr)
                stream->writeAttribute("subroom1_id",QString::number(id_subroom));
            else
                stream->writeAttribute("subroom1_id","NaN");
            stream->writeAttribute("pxreal",QString::number(pxreal));
            stream->writeAttribute("pyreal",QString::number(pyreal));
            stream->writeAttribute("px",QString::number(px));
            stream->writeAttribute("py",QString::number(py));
            stream->writeAttribute("a",QString::number(a));
            stream->writeAttribute("b",QString::number(b));

            stream->writeStartElement("associations");
            //
            stream->writeEndElement();//associations
            stream->writeEndElement();//landmark
        }

    }
}

void jpsDatamanager::CutOutLandmarks()
{
    using myClock = std::chrono::high_resolution_clock;

    int numberMainTargets = GetNumberOfMainTargets();
    int statcutMainTarget= numberMainTargets;

    int number;

    myClock::duration d = myClock::now().time_since_epoch();

    auto seed = d.count();

    std::default_random_engine generator(seed);

    std::discrete_distribution<int> distribution({ 100,0 });

    for (jpsLandmark* landmark:_landmarksAfterLoose)
    {
        // at least one main target will be kept
        if (landmark->GetType()=="main" || landmark->GetType()=="Main Target")
        {
            if (numberMainTargets==1)
                continue;
            else if (statcutMainTarget==1)
                continue;
        }



        number = distribution(generator);


        if (!number)
        {
            if (landmark->GetType()=="main" || landmark->GetType()=="Main Target")
                statcutMainTarget--;
            _landmarksAfterLoose.removeOne(landmark);
            BridgeLostLandmark(landmark);
        }


    }

}

void jpsDatamanager::CutOutLandmarks(int k, double m, double p0)
{


    //int numberMainTargets = GetNumberOfMainTargets();
    //int statcutMainTarget= numberMainTargets;

    int number=-1;


    QList<jpsLandmark* > cLandmarks = _landmarksAfterLoose;
    double prob=GetProbability(k,m,p0);



    for (jpsLandmark* landmark:cLandmarks)
    {
//        // at least one main target will be kept
//        if (landmark->GetType()=="main" || landmark->GetType()=="Main Target")
//        {
//            if (numberMainTargets==1)
//                continue;
//            else if (statcutMainTarget==1)
//                continue;
//        }

        double newProb=prob;

//        if (landmark->GetId()==0 || landmark->GetId()==8 || landmark->GetId()==1 || landmark->GetId()==5 || landmark->GetId()==12
//                || landmark->GetId()==11)

//        {
//            newProb=2.0*prob;
//            if (newProb>1.0)
//                newProb=1.0;
//        }
        //else if (landmark->GetId()==11 || landmark->GetId()==7 || landmark->GetId()==12 || landmark->GetId()==4
        //          || landmark->GetId()==3 || landmark->GetId()==9 || landmark->GetId()==13)
        //         newProb=prob;


//        if (landmark->GetId()==12)
//            std::cout << "12 " << newProb << std::endl;

//        else if (landmark->GetId()==10)
//            std::cout << "10 " << newProb << std::endl;


        std::discrete_distribution<int> distribution({ newProb*100.0,100.0-newProb*100.0 });

        number = distribution(_generator);

        if (number)
        {
            _landmarksAfterLoose.removeOne(landmark);
            BridgeLostLandmark(landmark);
        }


    }
}

double jpsDatamanager::GetProbability(int k, double m, double p0)
{
    double val = p0+m*k;
    if (val>1)
        val=1;
    else if (val<0)
        val=0;
    return val;
}

void jpsDatamanager::BridgeLostLandmark(jpsLandmark *landmark)
{
    // if landmark was removed from the landmarknetwork adjacent connected
    // landmarks will be connected with each other (if random engine returns 1)

    //using myClock = std::chrono::high_resolution_clock;

    QList<jpsLandmark* > connectedLandmarks;
    QList<jpsConnection* > connections = landmark->GetConnections();

    //int n=0;

    //myClock::duration d = myClock::now().time_since_epoch();

    //auto seed = d.count();

    //std::default_random_engine generator(seed);

    //std::discrete_distribution<int> distribution({ 0, 100 });

    for (jpsConnection* connection:connections)
    {
        _ConnectionsAfterLandmarkLoose.removeOne(connection);


//        int number = distribution(generator);
//        //std::cout << number << std::endl;

//        if (number)
//        {
//            std::pair<jpsLandmark*, jpsLandmark*> lPair = connection->GetLandmarks();
//            if (lPair.first!=landmark)
//                connectedLandmarks.push_back(lPair.first);
//            else
//                connectedLandmarks.push_back(lPair.second);
//        }
        //n++;
    }

//    for (jpsLandmark* connectedLandmark:connectedLandmarks)
//    {
//        for (jpsLandmark* connectedLandmark2:connectedLandmarks)
//        {
//            if (connectedLandmark!=connectedLandmark2)
//            {
//                bool status=false;
//                jpsConnection* newConnection = new jpsConnection(connectedLandmark,connectedLandmark2);
//                for (jpsConnection* connection:_ConnectionsAfterLandmarkLoose)
//                {
//                    if (*connection==*newConnection)
//                    {
//                        status=true;
//                    }
//                }
//                if (!status)
//                    _ConnectionsAfterLandmarkLoose.push_back(newConnection);
//            }
//        }
//    }

}

void jpsDatamanager::WriteConnections(jpsRegion* cRegion, QXmlStreamWriter *stream)
{
    int n=0;
    for (jpsConnection* connection:_ConnectionsAfterLandmarkLoose)
    {
        // only write connection if adjacent landmark is still there
        if (_landmarksAfterLoose.contains(connection->GetLandmarks().first) && _landmarksAfterLoose.contains(connection->GetLandmarks().second))
        {
            if (connection->GetLandmarks().first->GetRegion()==cRegion)
            {
                stream->writeStartElement("connection");

                stream->writeAttribute("id",QString::number(n));
                stream->writeAttribute("caption","Connection "+QString::number(n));
                stream->writeAttribute("type","Not specified");
                stream->writeAttribute("landmark1_id",QString::number(connection->GetLandmarks().first->GetId()));
                stream->writeAttribute("landmark2_id",QString::number(connection->GetLandmarks().second->GetId()));

                stream->writeEndElement();//connection
                n++;
            }
        }

    }
}

void jpsDatamanager::CreateAndSaveASimilarCogMap(const int& id)
{
    //Filename of first cognitivemap
    QString filename;
    if (id==0)
    {
        _currentCogMapFileName = QFileDialog::getSaveFileName(this->parent_widget,QFileDialog::tr("Save CognitiveMap XML"),"",QFileDialog::tr("XML-Files (*.xml)"));
        if (_currentCogMapFileName.isEmpty()) return;
        _currentCogMapFileName.remove(".xml");
        filename=_currentCogMapFileName+QString::number(id)+".xml";
    }
    else
    {
        filename=_currentCogMapFileName+QString::number(id)+".xml";
    }
    QFile file(filename);


    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        if (id==0)
            WriteCognitiveMapXML(file,false);
        else
            WriteCognitiveMapXML(file,true);

    }

}

void jpsDatamanager::CreateAndSaveASimilarCogMap(int id, int k, double m, double p0)
{

    QString filename;
    //FreeConsole();
//    if (id==0)
//    {
//        _currentCogMapFileName = QFileDialog::getSaveFileName(this->parent_widget,QFileDialog::tr("Save CognitiveMap XML"),"",QFileDialog::tr("XML-Files (*.xml)"));
//        if (_currentCogMapFileName.isEmpty()) return;
//        _currentCogMapFileName.remove(".xml");
//        filename=_currentCogMapFileName+QString::number(id)+"_"+QString::number(k)+"_"+QString::number(m)+"_"+QString::number(p0)+".xml";
        QString qstring = QString::number(m)+"_"+QString::number(p0);
        std::string str="mkdir \"D:/Dokumente/paper/WayfindingAndCognitiveMapsForPedestrianModels/simulations/cogmap/cogmaps/zweiteIteration/"+qstring.toStdString();
        //system(str.c_str());
        system(str.c_str());
//   }
//    else
//    {
    filename="D:/Dokumente/paper/WayfindingAndCognitiveMapsForPedestrianModels/simulations/cogmap/cogmaps/zweiteIteration/"+QString::number(m)+"_"+QString::number(p0)+"/"+"cogmap"+QString::number(id)+"0.xml";
    //}
    QFile file(filename);


    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {

        WriteCognitiveMapXML(file,k,m,p0);

    }


}

qreal jpsDatamanager::MakeItFuzzy(const qreal& mean, const qreal &std)
{
    using myClock = std::chrono::high_resolution_clock;
    myClock::duration d = myClock::now().time_since_epoch();

    auto seed = d.count();

    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(mean,std);

    double number = distribution(generator);

    return number;
}

int jpsDatamanager::GetNumberOfMainTargets() const
{
    qDebug("Enter jpsDatamanager::GetNumberOfMainTargets");
    int counter=0;
    for (jpsLandmark* landmark:_landmarks)
    {
        if (landmark->GetType()=="main" || landmark->GetType()=="Main Target")
        {
            counter++;
        }
    }
    qDebug("Leave jpsDatamanager::GetNumberOfMainTargets");
    return counter;
}

void jpsDatamanager::remove_all()
{
    qDebug("Enter jpsDatamanager::remove_all");
    remove_all_crossings();
    remove_all_exits();
    remove_all_obstacles();
    remove_all_rooms();
    remove_all_landmarks();
    RemoveAllConnections();
    RemoveAllRegions();
    room_id_counter=0;
    obs_id_counter=0;
    qDebug("Leave jpsDatamanager::remove_all");
}

void jpsDatamanager::remove_marked_lines()
{
    QList<jpsLineItem* > marked_lines = _mView->get_markedLines();
    QList<jpsObstacle*> obstacle_list = this->get_obstaclelist();
    QList<jpsRoom* > cList= this->get_roomlist();

    for (int i=0; i<marked_lines.size(); i++)
    {
        if (marked_lines[i]->is_Wall()==true && obstacle_list.size()>0)
        {
            for (int m=0; m<obstacle_list.size(); m++)
            {
                QList<jpsLineItem* > deleted_obstacle_lines;
                for (int n=0; n<obstacle_list[m]->get_listWalls().size(); n++)
                {
                    if(marked_lines[i]==obstacle_list[m]->get_listWalls()[n])
                    {
                        deleted_obstacle_lines.push_back(obstacle_list[m]->get_listWalls()[n]);
                    }
                }
                obstacle_list[m]->removeWall(deleted_obstacle_lines);
                qDebug()<< "jpsDatamanager::remove_marked_lines(): Obstacle line is deleted!";
            }
        }
        else
        {
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Marked line isn't in obstacle!";
        }

        if (marked_lines[i]->is_Wall()==true && cList.size()>0)
        {
            for (int j=0; j<cList.size(); j++)
            {
                QList<jpsLineItem* > delete_candidates;

                for (int k=0; k<cList[j]->get_listWalls().size(); k++)
                {
                    if (marked_lines[i]==cList[j]->get_listWalls()[k])
                    {
                        delete_candidates.push_back(cList[j]->get_listWalls()[k]);
                    }
                }
                cList[j]->removeWall(delete_candidates);
                qDebug()<< "jpsDatamanager::remove_marked_lines(): Wall line is deleted!";
            }
            qDebug()<< "jpsDatamanager::remove_marked_lines: marked line is removed" ;
        }

        else if (marked_lines[i]->is_Door() || marked_lines[i]->is_Exit())
        {
            QList<jpsCrossing* > cList= this->get_crossingList();
            for (int j=0; j<cList.size(); j++)
            {
                if (marked_lines[i]==cList[j]->get_cLine())
                {
                    this->remove_crossing(cList[j]);
                    break;
                }
            }
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Door line is deleted!";
        }
        else
        {
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Marked line isn't defined!";
        }
        
    }
}

void jpsDatamanager::set_view(jpsGraphicsView *view)
{
    _mView=view;
}

jpsGraphicsView * jpsDatamanager::get_view()
{
    return _mView;
}

//void jpsDatamanager::AutoAssignCrossings()
//{
//    QList<jpsCrossing* > crossings = crossingList;

//    for (jpsCrossing *crossing: crossings)
//    {
//        int roomCounter=0;
//        for (jpsRoom *room : roomlist)
//        {
//            if (room->ContainsDoor(crossing->get_cLine()) && roomCounter==0)
//            {
//                crossing->add_rooms(room);
//                roomCounter++;

//            }
//            else if (room->ContainsDoor(crossing->get_cLine()) && roomCounter==1)
//            {
//                crossing->add_rooms(crossing->get_roomList()[0],room);
//                crossings.removeOne(crossing);
//                break;
//            }
//        }
//    }



//    for (jpsCrossing *crossing: crossings)
//    {
//        int roomCounter =0;

//        for (jpsRoom *room: roomlist)
//        {
//            QList<jpsLineItem* > walls = room->get_listWalls();

//            int pointCounter = 0;

//            for (jpsLineItem* wall: walls)
//            {
//                if (wall->get_line()->line().p1()==crossing->get_cLine()->get_line()->line().p1() ||
//                       wall->get_line()->line().p1()==crossing->get_cLine()->get_line()->line().p2() ||
//                        wall->get_line()->line().p2()==crossing->get_cLine()->get_line()->line().p1() ||
//                        wall->get_line()->line().p2()==crossing->get_cLine()->get_line()->line().p2())
//                {
//                    pointCounter++;

//                }
//            }

//            if (pointCounter==2 && roomCounter==0)
//            {
//                crossing->add_rooms(room);
//                roomCounter++;

//            }
//            else if (pointCounter==2 && roomCounter==1)
//            {
//                crossing->add_rooms(crossing->get_roomList()[0],room);
//                crossings.removeOne(crossing);
//                break;
//            }

//        }
//    }



//}

//void jpsDatamanager::AutoAssignExits()
//{
//    for (jpsExit *exit: exitList)
//    {
//        for (jpsRoom *room: roomlist)
//        {
//            QList<jpsLineItem* > walls = room->get_listWalls();

//            int pointCounter = 0;

//            for (jpsLineItem* wall: walls)
//            {
//                if (wall->get_line()->line().p1()==exit->get_cLine()->get_line()->line().p1() ||
//                       wall->get_line()->line().p1()==exit->get_cLine()->get_line()->line().p2() ||
//                        wall->get_line()->line().p2()==exit->get_cLine()->get_line()->line().p1() ||
//                        wall->get_line()->line().p2()==exit->get_cLine()->get_line()->line().p2())
//                {
//                    pointCounter++;
//                }
//            }

//            if (pointCounter==2)
//            {
//                exit->add_rooms(room);
//                break;
//            }
//        }
//    }

//}


bool jpsDatamanager::readXML(QFile &file)
{

    QXmlStreamReader xmlReader(&file);

    // skip header
    xmlReader.readNext();
    xmlReader.readNext();

    // see if file starts with geometry
    if (xmlReader.name() != "geometry")
        return false;

    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement)
        {
            /* If it's named rooms, we'll go to the next.*/
            if(xmlReader.name() == "rooms")
            {
                continue;
            }
            /* If it's named room, we'll dig the information from there.*/
            if(xmlReader.name() == "room")
            {
                continue;
            }
            if(xmlReader.name() == "crossings")
            {
                continue;
            }
            if(xmlReader.name() == "transitions")
            {
                continue;
            }
            if(xmlReader.name() == "subroom")
            {
                this->parseSubRoom(xmlReader);
            }
            if(xmlReader.name() == "crossing")
            {
                this->parseCrossings(xmlReader);
            }
            if(xmlReader.name() == "transition")
            {
                this->parseTransitions(xmlReader);
            }
        }

    }

    /* Error handling. */
    if(xmlReader.hasError())
    {
        QMessageBox::critical(_mView,
                              "QXSRExample::parseXML",
                              xmlReader.errorString(),
                              QMessageBox::Ok);
        return false;
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xmlReader.clear();

    return true;
}

bool jpsDatamanager::readRoutingXML(QFile &file)
{
    QXmlStreamReader xmlReader(&file);

    // skip header
    xmlReader.readNext();
    xmlReader.readNext();

    // see if file starts with geometry
    if (xmlReader.name() != "routing")
        return false;

    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement)
        {
            /* If it's named Hlines, we'll go to the next.*/
            if(xmlReader.name() == "Hlines")
            {
                continue;
            }

            if(xmlReader.name() == "Hline")
            {
                this->parseHline(xmlReader);
            }
        }
    }
    /* Error handling. */
    if(xmlReader.hasError())
    {
        QMessageBox::critical(_mView,
                              "QXSRExample::parseXML",
                              xmlReader.errorString(),
                              QMessageBox::Ok);
        return false;
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xmlReader.clear();

    return true;
}

void jpsDatamanager::parseHline(QXmlStreamReader &xmlReader)
{

    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "Hline"))
    {
        xmlReader.readNext();

        if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                         xmlReader.name() == "vertex")
        {

            // get coords from vertices
            qreal x1=xmlReader.attributes().value("px").toString().toFloat();
            qreal y1=xmlReader.attributes().value("py").toString().toFloat();

            // go to next vertex
            xmlReader.readNext();
            xmlReader.readNext();
            xmlReader.readNext();

            qreal x2=xmlReader.attributes().value("px").toString().toFloat();
            qreal y2=xmlReader.attributes().value("py").toString().toFloat();
            // add Line to graphview
            _mView->addLineItem(x1,y1,x2,y2,"HLine");
        }

    }
}

void jpsDatamanager::parseSubRoom(QXmlStreamReader &xmlReader)
{
    // new subroom
    this->new_room();
    /* Let's get the attributes for subroom */
    QXmlStreamAttributes attributes = xmlReader.attributes();
    /* Let's check that subroom has id attribute. */

    /* We'll add it to the room. */
    roomlist.last()->set_id(attributes.value("id").toString().toInt());
    if(attributes.hasAttribute("A_x"))
        roomlist.last()->set_ax(attributes.value("A_x").toString().toFloat());
    else
        roomlist.last()->set_ax(0.0);

    roomlist.last()->set_by(attributes.value("B_y").toString().toFloat());
    auto elevation = attributes.value("C_z").toString().toFloat();
    
    roomlist.last()->set_cz(elevation);
    roomlist.last()->set_elevation(elevation);

    /* We'll add it to the room. */
    roomlist.last()->change_name(attributes.value("caption").toString());
    if(attributes.hasAttribute("class"))
    {
        if (attributes.value("class").toString()=="subroom")
            roomlist.last()->set_type("Not specified");
        else
            roomlist.last()->set_type(attributes.value("class").toString());
    }
    this->parseWalls(xmlReader,roomlist.last());
    this->parseObstacles(xmlReader,roomlist.last());

    /* if room is container for not assigned lines */

    if (roomlist.last()->get_id()==-1)
    {
        remove_room(roomlist.last());
    }
}

void jpsDatamanager::parseWalls(QXmlStreamReader &xmlReader, jpsRoom *room)
{

    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "subroom"))
    {

        if (xmlReader.name()=="obstacle")
        {
            break;
        }

        xmlReader.readNext();


        if(xmlReader.name() == "polygon")
        {
            continue;
        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                 xmlReader.name() == "vertex")
        {

            // get coords from vertices
            qreal x1=xmlReader.attributes().value("px").toString().toFloat();
            qreal y1=xmlReader.attributes().value("py").toString().toFloat();

            // go to next vertex
            xmlReader.readNext();
            xmlReader.readNext();
            xmlReader.readNext();

            qreal x2=xmlReader.attributes().value("px").toString().toFloat();
            qreal y2=xmlReader.attributes().value("py").toString().toFloat();
            // add Line to graphview
            jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"Wall");

            room->addWall(lineItem);

        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                 xmlReader.name() == "up")
        { // subroom is a stair?
        //      /*@todo: calculate Ax, B_y and C_z*/
             QPointF up = this->parseUp(xmlReader);
             QPointF down = this->parseDown(xmlReader);
             roomlist.last()->set_up(up);
             roomlist.last()->set_down(down);
        }
    } // while  subroom

}


void jpsDatamanager::parseWalls(QXmlStreamReader &xmlReader, jpsObstacle *room)
{

    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "obstacle"))
    {

        xmlReader.readNext();

        if(xmlReader.name() == "polygon")
        {
            continue;
        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                 xmlReader.name() == "vertex")
        {

            // get coords from vertices
            qreal x1=xmlReader.attributes().value("px").toString().toFloat();
            qreal y1=xmlReader.attributes().value("py").toString().toFloat();

            // go to next vertex
            xmlReader.readNext();
            xmlReader.readNext();
            xmlReader.readNext();

            qreal x2=xmlReader.attributes().value("px").toString().toFloat();
            qreal y2=xmlReader.attributes().value("py").toString().toFloat();
            // add Line to graphview
            jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"Wall");

            room->addWall(lineItem);

        }
    }
}


void jpsDatamanager::parseCrossings(QXmlStreamReader &xmlReader)
{
     qDebug("Enter jpsDatamanager::parseCrossings");
    int id = xmlReader.attributes().value("id").toString().toInt();
    int room_id1 = xmlReader.attributes().value("subroom1_id").toString().toInt();
    int room_id2 = xmlReader.attributes().value("subroom2_id").toString().toInt();
    qDebug("\t id = %d, room_id1 %d, room_id2  %d,", id, room_id1, room_id2);

    // go to first vertex
    while(xmlReader.name() != "vertex")
    {
        xmlReader.readNext();
    }

    qreal x1=xmlReader.attributes().value("px").toString().toFloat();
    qreal y1=xmlReader.attributes().value("py").toString().toFloat();

    // go to next vertex
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();

    qreal x2=xmlReader.attributes().value("px").toString().toFloat();
    qreal y2=xmlReader.attributes().value("py").toString().toFloat();
    qDebug("\t x1 = %.2f, y1 = %.2f, x2 = %.2f y2 = %.2f", x1, y1, x2, y2);
    jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"Door");

    if (id!=-2)
    {
        jpsCrossing* crossing = new jpsCrossing(lineItem);
        crossing->set_id(id);

        jpsRoom* room1 = nullptr;
        jpsRoom* room2 = nullptr;

        ///find rooms which belong to crossing
        for (int i=0; i<roomlist.size(); i++)
        {
            if (roomlist[i]->get_id()==room_id1)
            {
                room1 = roomlist[i];

            }
            else if (roomlist[i]->get_id()==room_id2)
            {
                room2 = roomlist[i];
            }
        }
        if (room1!=nullptr && room2!=nullptr){
            crossing->add_rooms(room1,room2);
            qDebug("\t added two rooms");
        }
        crossingList.push_back(crossing);
    }
    qDebug("Leave jpsDatamanager::parseCrossings");
}
QPointF jpsDatamanager::parseUp(QXmlStreamReader &xmlReader)
{
     // find <up>
    while(xmlReader.name() != "up")
    {
        xmlReader.readNext();
    }
    qreal x1=xmlReader.attributes().value("px").toString().toFloat();
    qreal y1=xmlReader.attributes().value("py").toString().toFloat();
    return QPointF(x1, y1); 
}
QPointF jpsDatamanager::parseDown(QXmlStreamReader &xmlReader)
{
     // find <down>
    while(xmlReader.name() != "down")
    {
        xmlReader.readNext();
    }
    qreal x1=xmlReader.attributes().value("px").toString().toFloat();
    qreal y1=xmlReader.attributes().value("py").toString().toFloat();
    return QPointF(x1, y1); 
}

void jpsDatamanager::parseTransitions(QXmlStreamReader &xmlReader)
{
     qDebug("Enter jpsDatamanager::parseTransitions");
    int id = xmlReader.attributes().value("id").toString().toInt();
    QString caption = xmlReader.attributes().value("caption").toString();
    QString type = xmlReader.attributes().value("type").toString();
    int room_id = xmlReader.attributes().value("subroom1_id").toString().toInt();
    int room_id2 = xmlReader.attributes().value("subroom2_id").toString().toInt();
    qDebug("\t id= %d, caption=%s, type = %s, room_id = %d, room_id2 = %d", id, caption.toStdString().c_str(), type.toStdString().c_str(), room_id, room_id2);
    // go to first vertex
    while(xmlReader.name() != "vertex")
    {
        xmlReader.readNext();
    }

    qreal x1=xmlReader.attributes().value("px").toString().toFloat();
    qreal y1=xmlReader.attributes().value("py").toString().toFloat();

    // go to next vertex
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();

    qreal x2=xmlReader.attributes().value("px").toString().toFloat();
    qreal y2=xmlReader.attributes().value("py").toString().toFloat();
    qDebug("\t x1 = %.2f, y1 = %.2f, x2 = %.2f y2 = %.2f", x1, y1, x2, y2);
    jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"Exit");

    if (id!=-2)
    {
        jpsCrossing *exit = new jpsCrossing(lineItem);
        exit->set_id(id);
        exit->change_name(caption);
        //exit->set_type(type);
        jpsRoom* room1 = nullptr;
        jpsRoom* room2 = nullptr;
        ///find rooms which belong to transition
        for (int i=0; i<roomlist.size(); i++)
        {
             if (roomlist[i]->get_id()==room_id)
             {
                  room1 = roomlist[i];
             }
             else if (roomlist[i]->get_id()==room_id2)
             {
                  room2 = roomlist[i];
             }
        }
        if (room1!=nullptr && room2!=nullptr){
            exit->add_rooms(room1,room2);
            qDebug("\t added two rooms");
        }
        else if (room1!=nullptr){
             exit->add_rooms(room1);
             qDebug("\t add room1");
        }
             
        else if (room2!=nullptr){
             exit->add_rooms(room2);             
             qDebug("\t add room2");
        }
        else
             std::cout << "ERROR: Transition has no rooms!\n";

        exit->SetStatExit(true);
        crossingList.push_back(exit);
    }
    qDebug("Leave jpsDatamanager::parseTransitions");
}

void jpsDatamanager::parseObstacles(QXmlStreamReader &xmlReader, jpsRoom *room)
{

    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "subroom"))
    {


        if (xmlReader.tokenType() == QXmlStreamReader::StartElement && xmlReader.name()=="obstacle")
        {
          int id = xmlReader.attributes().value("id").toString().toInt();
            QString caption = xmlReader.attributes().value("caption").toString();

            jpsObstacle* obs = new jpsObstacle(id);
            obs->change_name(caption);
            obs->set_room(room);

           // while (xmlReader.name()=="obstacle")
           // {
            //    xmlReader.readNext();
            //}
            this->parseWalls(xmlReader,obs);

            obstaclelist.push_back(obs);
        }

        xmlReader.readNext();
    }
}

bool jpsDatamanager::readDXF(std::string filename)
{

    DL_Dxf dxf;
    if (!dxf.in(filename, this))
    {
        return false;
    }
    else
    {
        ///AutoZoom to contents (items of Scene)
        _mView->AutoZoom();
        return true;
    }

}

void jpsDatamanager::addLine(const DL_LineData &d)
{
    DL_Attributes attributes = DL_CreationInterface::getAttributes();
    std::string layername = attributes.getLayer();
    std::transform(layername.begin(), layername.end(), layername.begin(), ::tolower);
    if (layername=="wall")
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"Wall");
    else if (layername=="door")
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"Door");
    else
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2);
}


void jpsDatamanager::writeDXF(std::string filename)
{
    DL_Dxf* dxf = new DL_Dxf;
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out(filename.c_str(), exportVersion);
    if (dw==0L)
    {
        printf("Cannot open file 'myfile.dxf' \
        for writing.");
        // abort function e.g. with return

    }
    //Header
    writeDXFHeader(dxf,dw);
    // ....

    writeDXFTables(dxf,dw);
    writeDXFBlocks(dxf,dw);
    writeDXFEntities(dxf,dw);
    writeDXFObjects(dxf,dw);

    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;
}

void jpsDatamanager::writeDXFHeader(DL_Dxf *dxf, DL_WriterA *dw)
{
    dxf->writeHeader(*dw);
    dw->sectionEnd();

}

void jpsDatamanager::writeDXFTables(DL_Dxf *dxf, DL_WriterA *dw)
{
    dxf->writeVPort(*dw);

    // Linetypes
    dw->tableLinetypes(25);

    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK","", 0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER","", 0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("CONTINUOUS","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("ACAD_ISO02W100","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("ACAD_ISO03W100","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("ACAD_ISO04W100","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("ACAD_ISO05W100","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDER","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDER2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BORDERX2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTER","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTER2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CENTERX2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT2","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("DASHDOTX2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHED","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHED2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DASHEDX2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE2","",0,0,0));
    dxf->writeLinetype(*dw,
    DL_LinetypeData("DIVIDEX2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOT","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOT2","",0,0,0));
    dxf->writeLinetype(*dw, DL_LinetypeData("DOTX2","",0,0,0));
    dw->tableEnd();

    //Layers
    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);
    dxf->writeLayer(*dw,
    DL_LayerData("0", 0),
    DL_Attributes(
    std::string(""),   //leave empty
    0,   //default color
    0,               //default color24
    100,             //default width
    "CONTINUOUS"));    //default line style

    dxf->writeLayer(*dw,
    DL_LayerData("mainlayer", 0),
    DL_Attributes(
    std::string(""),
    2,
    2,
    100,
    "CONTINUOUS"));

    dxf->writeLayer(*dw,
    DL_LayerData("anotherlayer", 0),
    DL_Attributes(
    std::string(""),
    1,
    1,
    100,
    "CONTINUOUS"));
    dw->tableEnd();

    //dxf->writeStyle(*dw,);
    dxf->writeView(*dw);
    dxf->writeUcs(*dw);
    dw->tableAppid(1);
    dw->tableAppidEntry(0x12);
    dw->dxfString(2, "ACAD");
    dw->dxfInt(70, 0);
    dw->tableEnd();

    /*
    dxf->writeDimStyle(*dw,
    arrowSize,
    extensionLineExtension,
    extensionLineOffset,
    dimensionGap,
    dimensionTextSize);
    */
    //blockrecords
    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();

    //end tables
    dw->sectionEnd();




}

void jpsDatamanager::writeDXFBlocks(DL_Dxf *dxf, DL_WriterA *dw)
{
    dw->sectionBlocks();
    dxf->writeBlock(*dw,
    DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw,
    DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw,
    DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");
    dxf->writeBlock(*dw,
    DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf.writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");
    dxf->writeBlock(*dw,
    DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf.writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");
    dw->sectionEnd();
}

void jpsDatamanager::writeDXFEntities(DL_Dxf *dxf, DL_WriterA *dw)
{
    dw->sectionEntities();
    // write all your entities..

    //dxf->writePoint(
    //*dw,
    //DL_PointData(10.0,
    //45.0,
    //0.0),
    //DL_Attributes("mainlayer", 256, 256, -1, "BYLAYER"));


    QList<jpsLineItem* > lines = _mView->get_line_vector();


    DL_Attributes attributeW("wall", 256, 256, -1, "BYLAYER");
    DL_Attributes attributeD("door", 16, 78, 139, "BYLAYER");

    for (jpsLineItem* lineItem:lines)
    {
        DL_LineData linedata(lineItem->get_line()->line().x1(),
                    lineItem->get_line()->line().y1(),
                    0.0,
                    lineItem->get_line()->line().x2(),
                    lineItem->get_line()->line().y2(),
                    0.0);

        if (lineItem->is_Wall())
            dxf->writeLine(*dw,linedata,attributeW);
        else
            dxf->writeLine(*dw,linedata,attributeD);

    }


    /*
    dxf->writeLine(
    *dw,
    DL_LineData(25.0,
    // start point
    30.0,
    0.0,
    100.0,
    // end point
    120.0,
    0.0),
    DL_Attributes("mainlayer", 256,256, -1, "BYLAYER"));
    */
    dw->sectionEnd();
}

void jpsDatamanager::writeDXFObjects(DL_Dxf *dxf, DL_WriterA *dw)
{
    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);
}

QString jpsDatamanager::check_printAbility()
{
    qDebug("Enter jpsDatamanager::check_printAbility");
    if (roomlist.size()<1)
    {
        QString string = "No rooms defined! Save XML-file not possible!";
        return string;
    }

    // check crossing
    for (int i=0; i<crossingList.size(); i++)
    {
        if (!crossingList[i]->IsExit() && crossingList[i]->get_roomList().size() < 2) //crossing
        {
            QString string = "There are crossings which are not assigned to a room! Save XML-file not possible!";
            return string;
        }
    }

    // check transitions
    for (int i=0; i<crossingList.size(); i++)
    {
        if (crossingList[i]->IsExit() && crossingList[i]->get_roomList().size() < 1) //transitions
        {
            QString string = "There are exits which are not assigned to a room! Save XML-file not possible!";
            return string;
        }
    }

    // check obstacles
    for (int i=0; i<obstaclelist.size(); i++)
    {
        if (obstaclelist[i]->get_room()==nullptr)
        {
            QString string = "There are obstacles which are not assigned to a room! Save XML-file not possible!";
            return string;
        }
    }

    qDebug("Leave jpsDatamanager::check_printAbility");
    return "";
}

bool jpsDatamanager::ParseCogMap(QFile &file)
{
    QXmlStreamReader xmlReader(&file);
    jpsRegion* actRegion=nullptr;

    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement)
        {
            /* If it's named rooms, we'll go to the next.*/
            if(xmlReader.name() == "cognitiveMap")
            {
                continue;
            }
//            if(xmlReader.name() == "header")
//            {
//                continue;
//            }
            if(xmlReader.name() == "regions")
            {
                continue;
            }
            if (xmlReader.name()=="region")
            {
                actRegion=ParseRegion(xmlReader);
            }
            if(xmlReader.name() == "landmarks")
            {
                continue;
            }
            if(xmlReader.name() == "landmark")
            {
                if (actRegion!=nullptr)
                    ParseLandmark(actRegion,xmlReader);
            }
            if(xmlReader.name() == "connections")
            {
                continue;
            }
            if(xmlReader.name() == "connection")
            {
                if (actRegion!=nullptr)
                    ParseConnection(actRegion,xmlReader);
            }
        }
    }
    /* Error handling. */
    if(xmlReader.hasError())
    {
        QMessageBox::critical(_mView,
                              "QXSRExample::parseXML",
                              xmlReader.errorString(),
                              QMessageBox::Ok);
        return false;
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */


    xmlReader.clear();
    return true;
}

void jpsDatamanager::ParseLandmark(jpsRegion *actRegion, QXmlStreamReader &xmlReader)
{
    int id = xmlReader.attributes().value("id").toString().toInt();
    QString caption = xmlReader.attributes().value("caption").toString();
    QString type = xmlReader.attributes().value("type").toString();
    //int roomId = xmlReader.attributes().value("room1_id").toString().toInt();
    int subroomId = xmlReader.attributes().value("subroom1_id").toString().toInt();
    qreal real_x = xmlReader.attributes().value("pxreal").toString().toFloat();
    qreal real_y = xmlReader.attributes().value("pyreal").toString().toFloat();
    qreal x = xmlReader.attributes().value("px").toString().toFloat();
    qreal y = xmlReader.attributes().value("py").toString().toFloat();
    qreal rA = xmlReader.attributes().value("a").toString().toFloat();
    qreal rB = xmlReader.attributes().value("b").toString().toFloat();


    //create landmark incl. pixmap
    _mView->addLandmark(QPointF(real_x,real_y));

    _landmarks.back()->SetId(id);
    _landmarks.back()->SetCaption(caption);
    _landmarks.back()->SetType(type);

    for (jpsRoom* room:roomlist)
    {
        if (room->get_id()==subroomId)
        {
            _landmarks.back()->SetRoom(room);
            break;
        }

    }
    _landmarks.back()->SetRect(QRectF(QPointF(x-rA,y+rB),QPointF(x+rA,y-rB)));

    // Ellipse
    // show ellipse and text in graphics view
    QPen pen = QPen(Qt::blue,2);
    pen.setCosmetic(true);
    QGraphicsEllipseItem* ellipse = _mView->GetScene()->addEllipse(_landmarks.back()->GetRect(),pen);
    ellipse->setTransform(QTransform::fromTranslate(_mView->GetTranslationX(),_mView->GetTranslationY()), true);
    QGraphicsTextItem* text = _mView->GetScene()->addText(_landmarks.back()->GetCaption());
    text->setPos(_landmarks.back()->GetPos().x()+_mView->GetTranslationX(),_landmarks.back()->GetPos().y()+_mView->GetTranslationY());
    //text->setScale(gl_scale_f);
    text->setData(0,_mView->GetScaleF());
    text->setTransform(QTransform::fromScale(_mView->GetScaleF(),-_mView->GetScaleF()),true);
    _landmarks.back()->SetEllipseItem(ellipse);
    _landmarks.back()->SetTextItem(text);

    //register region and landmark (vice versa)
    actRegion->AddLandmark(_landmarks.back());
    _landmarks.back()->SetRegion(actRegion);

}

void jpsDatamanager::ParseConnection(jpsRegion *actRegion, QXmlStreamReader &xmlReader)
{
    //int id = xmlReader.attributes().value("id").toInt();
    //QString caption = xmlReader.attributes().value("caption").toString();
    //QString type = xmlReader.attributes().value("type").toString();
    int idLandmark1 = xmlReader.attributes().value("landmark1_id").toString().toInt();
    int idLandmark2 = xmlReader.attributes().value("landmark2_id").toString().toInt();

    jpsLandmark* landmark1 = nullptr;
    jpsLandmark* landmark2 = nullptr;

    for (jpsLandmark* landmark:actRegion->GetLandmarks())
    {
        if (landmark->GetId()==idLandmark1)
            landmark1=landmark;
        else if (landmark->GetId()==idLandmark2)
            landmark2=landmark;
    }

    jpsConnection* currentConnection =new jpsConnection(landmark1,landmark2);
    landmark1->NewConnection(currentConnection);
    landmark2->NewConnection(currentConnection);

    QLineF line = QLineF(landmark1->GetPos(),landmark2->GetPos());
    QPen pen = QPen(Qt::blue,2);
    pen.setCosmetic(true);
    QGraphicsLineItem* lineItem = _mView->GetScene()->addLine(line,pen);
    lineItem->setTransform(QTransform::fromTranslate(_mView->GetTranslationX(),_mView->GetTranslationY()), true);
    currentConnection->SetLineItem(lineItem);

    NewConnection(currentConnection);
}

bool jpsDatamanager::ReadLineFile(QFile &file)
{

    QVector<jpsLineItem*> lineItems = _mView->get_line_vector().toVector();
    QTextStream in(&file);

    while ( !in.atEnd() )
    {       
        QString Qline = in.readLine();
        if (Qline.startsWith("Room"))
        {
            this->new_room();
        }
        else
        {

            QStringList strList=Qline.split(",");

            QPointF point1=QPointF(strList[0].split("_")[0].toDouble(),strList[0].split("_")[1].toDouble());
            QPointF point2=QPointF(strList[1].split("_")[0].toDouble(),strList[1].split("_")[1].toDouble());

            QLineF line=QLineF(point1,point2);

            for (jpsLineItem* lineItem:lineItems)
            {
                if (LineIsEqual(lineItem->get_line()->line(),line))
                {
                  if (lineItem->is_Wall())
                    this->roomlist.back()->addWall(lineItem);
                  else if (lineItem->is_Door())
                  {
                      this->new_crossing(lineItem);

                      for (jpsCrossing* crossing:crossingList)
                      {
                          if (crossing->get_cLine()==lineItem)
                              crossing->SetRoom(this->roomlist.back());
                      }
                  }
                }
            }
        }
    }

    //for (jpsRoom* room:this->roomlist)
    //{
    //    _mView->show_hide_roomCaption(room->get_name(),room->get_center().x(),room->get_center().y());
    //}

    return true;

}

jpsRegion* jpsDatamanager::ParseRegion(QXmlStreamReader &xmlReader)
{
    int id = xmlReader.attributes().value("id").toString().toInt();
    QString caption = xmlReader.attributes().value("caption").toString();
    qreal x = xmlReader.attributes().value("px").toString().toFloat();
    qreal y = xmlReader.attributes().value("py").toString().toFloat();
    qreal rA = xmlReader.attributes().value("a").toString().toFloat();
    qreal rB = xmlReader.attributes().value("b").toString().toFloat();

    QPen pen = QPen(Qt::darkGreen,2);
    pen.setCosmetic(true);
    QRectF rect = QRectF(QPointF(x-rA,y+rB),QPointF(x+rA,y-rB));
    QGraphicsEllipseItem* ellipse = _mView->GetScene()->addEllipse(rect,pen);
    ellipse->setTransform(QTransform::fromTranslate(_mView->GetTranslationX(),_mView->GetTranslationY()), true);

    // create region
    jpsRegion* actRegion = new jpsRegion(id,caption,
                                      rect.center(),std::fabs(rect.width()/2.0),std::fabs(rect.height()/2.0));

    QGraphicsTextItem* text = _mView->GetScene()->addText(actRegion->GetCaption());
    text->setPos(rect.center().x()+_mView->GetTranslationX(),rect.center().y()+_mView->GetTranslationY());
    //text->setScale(gl_scale_f);
    text->setData(0,_mView->GetScaleF());
    text->setTransform(QTransform::fromScale(_mView->GetScaleF(),-_mView->GetScaleF()),true);

    actRegion->SetTextItem(text);
    actRegion->SetEllipse(ellipse);

    NewRegion(actRegion);

    return actRegion;

}

bool LineIsEqual(const QLineF& line1, const QLineF& line2, double eps)
{
   if ((line1.p1().x()>=line2.p1().x()-eps && line1.p1().x()<=line2.p1().x()+eps) &&
          (line1.p1().y()>=line2.p1().y()-eps && line1.p1().y()<=line2.p1().y()+eps) &&
           (line1.p2().x()>=line2.p2().x()-eps && line1.p2().x()<=line2.p2().x()+eps) &&
           (line1.p2().y()>=line2.p2().y()-eps && line1.p2().y()<=line2.p2().y()+eps))
   {
       return true;
   }
   else if ((line1.p1().x()>=line2.p2().x()-eps && line1.p1().x()<=line2.p2().x()+eps) &&
               (line1.p1().y()>=line2.p2().y()-eps && line1.p1().y()<=line2.p2().y()+eps) &&
                (line1.p2().x()>=line2.p1().x()-eps && line1.p2().x()<=line2.p1().x()+eps) &&
                (line1.p2().y()>=line2.p1().y()-eps && line1.p2().y()<=line2.p1().y()+eps))
   {
       return true;
   }
   else
       return false;
}

/*
    since 0.8.8

    For save and read sources


    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
    <JPScore project="JPS-Project" version="0.8">
        <agents_sources>
            <source />
        </agents_sources>
    </JPScore>

*/

void jpsDatamanager::writeSources(QXmlStreamWriter *stream, QList<JPSSource *> &sourcelist) {
    for(JPSSource* source:sourcelist)
    {
        if(source->isBeSaved())
        {
            stream->writeStartElement("source");
            stream->writeAttribute("id",QString::number(source->getId()));

            if(!source->getFrequency().isEmpty())
                stream->writeAttribute("frequency",source->getFrequency());

            if(!source->getN_create().isEmpty())
                stream->writeAttribute("N_create",source->getN_create());

            if(!source->getPercent().isEmpty())
                stream->writeAttribute("percent",source->getPercent());

            if(!source->getRate().isEmpty())
                stream->writeAttribute("rate",source->getRate());

            if(!source->getTime_min().isEmpty())
                stream->writeAttribute("time_min",source->getTime_min());

            if(!source->getTime_max().isEmpty())
                stream->writeAttribute("time_max",source->getTime_max());

            if(!source->getAgents_max().isEmpty())
                stream->writeAttribute("agents_max",source->getAgents_max());

            if(!source->getGroup_id().isEmpty())
                stream->writeAttribute("group_id",source->getGroup_id());

            if(!source->getCaption().isEmpty())
                stream->writeAttribute("caption",source->getCaption());

            if(!source->getGreedy().isEmpty())
                stream->writeAttribute("greedy",source->getGreedy());

            if(!source->getTime().isEmpty())
                stream->writeAttribute("time",source->getTime());

            if(!source->getStartX().isEmpty())
                stream->writeAttribute("startX",source->getStartX());

            if(!source->getStartY().isEmpty())
                stream->writeAttribute("startY",source->getStartY());

            stream->writeAttribute("x_min",QString::number(source->getX_min()));
            stream->writeAttribute("x_max",QString::number(source->getX_max()));
            stream->writeAttribute("y_min",QString::number(source->getY_min()));
            stream->writeAttribute("y_max",QString::number(source->getY_max()));
            stream->writeEndElement(); //end source
        }
    }
}

void jpsDatamanager::writeSourceXML(QFile &file) {
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);

    writeSourceHeader(stream);

    stream->writeStartElement("agents_sources");
    sourcelist.clear();
    sourcelist = getSourcelist();
    writeSources(stream, sourcelist);
    stream->writeEndElement(); //end sources

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
}

void jpsDatamanager::writeSourceHeader(QXmlStreamWriter *stream)
{
    qDebug("Enter jpsDatamanager::writeSourceHeader");
    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("JPScore");
    stream->writeAttribute("project","JPS-Project");
    stream->writeAttribute("version", "0.8");

    qDebug("Leave jpsDatamanager::writeSourceHeader");
}
/*
    since 0.8.8

    For save and read goals

    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
    <JPScore project="JPS-Project" version="0.8">
        <goals>
            <goal id="1" final="true" caption="goal1">
                <polygon>
                    <vertex px="3.6" py="-3.0" />
                    <vertex px="3.6" py="-4.0" />
                    <vertex px="5.2" py="-4.0" />
                    <vertex px="5.2" py="-3.0" />
                    <vertex px="3.6" py="-3.0" />
                </polygon>
            </goal>
        </goals>
    </JPScore>
*/

void jpsDatamanager::writeGoalXML(QFile &file)
{
    auto *stream = new QXmlStreamWriter(&file);

    stream->setAutoFormatting(true);

    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("JPScore");
    stream->writeAttribute("project","JPS-Project");
    stream->writeAttribute("version", "0.8");

    stream->writeStartElement("goals");
    goallist.clear();
    goallist = getGoallist();
    writeGoals(stream, goallist);
    stream->writeEndElement(); //end goals

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
}

void jpsDatamanager::writeGoals(QXmlStreamWriter *stream, QList<JPSGoal *> &goallist)
{
    for(JPSGoal* goal:goallist)
    {
        if(goal->getBeSaved() == "true")
        {
            stream->writeStartElement("goal");
            stream->writeAttribute("id",QString::number(goal->getId()));
            stream->writeAttribute("final",goal->getIsFinal());
            stream->writeAttribute("caption",goal->getCaption());
            stream->writeAttribute("x_min",goal->getXMin());
            stream->writeAttribute("x_max",goal->getXMax());
            stream->writeAttribute("y_min",goal->getYMin());
            stream->writeAttribute("y_max",goal->getYMax());

            stream->writeStartElement("polygon");

            stream->writeStartElement("vertex");
            stream->writeAttribute("px", QString::number(goal->rect().topLeft().x()));
            stream->writeAttribute("py", QString::number(goal->rect().topLeft().y()));
            stream->writeEndElement();

            stream->writeStartElement("vertex");
            stream->writeAttribute("px", QString::number(goal->rect().topRight().x()));
            stream->writeAttribute("py", QString::number(goal->rect().topRight().y()));
            stream->writeEndElement();

            stream->writeStartElement("vertex");
            stream->writeAttribute("px", QString::number(goal->rect().bottomRight().x()));
            stream->writeAttribute("py", QString::number(goal->rect().bottomRight().y()));
            stream->writeEndElement();

            stream->writeStartElement("vertex");
            stream->writeAttribute("px", QString::number(goal->rect().bottomLeft().x()));
            stream->writeAttribute("py", QString::number(goal->rect().bottomLeft().y()));
            stream->writeEndElement();

            stream->writeStartElement("vertex");
            stream->writeAttribute("px", QString::number(goal->rect().topLeft().x()));
            stream->writeAttribute("py", QString::number(goal->rect().topLeft().y()));
            stream->writeEndElement();

            stream->writeEndElement();//end goal
        }

        stream->writeEndElement();//end goals
    }
}

/*
    since v0.8.8

    Save transitions as external file

<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<JPScore>
  <transitions>
    <transition id="1" caption="exit" type="emergency" room1_id="0" subroom1_id="0" room2_id="2" subroom2_id="0">
      <vertex px="9.2" py="2.8" />
      <vertex px="9.2" py="6.4" />
    </transition>
    <transition id="2" caption="exit" type="emergency" room1_id="0" subroom1_id="0" room2_id="3" subroom2_id="0">
      <vertex px="3.2" py="9.2" />
      <vertex px="5.6" py="9.2" />
    </transition>
  </transitions>
</JPScore>

*/

void jpsDatamanager::writeTransitionXML(QFile &file)
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = _mView->get_line_vector();

    stream->setAutoFormatting(true);

    stream->writeStartDocument("1.0",true);
    stream->setCodec("UTF-8");

    stream->writeStartElement("JPScore");
    stream->writeStartElement("transitions");
    writeTransitions(stream,lines);
    exitList.clear();
    stream->writeEndElement();//transitions

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
}

const QList<JPSGoal *> &jpsDatamanager::getGoallist()
{
    goallist.clear();
    goallist = _mView->getGoals();
    return goallist;
}

/*
    Since v0.8.8

    Write traffic (doors) as a external xml file.

    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
    <JPScore project="JPS-Project" version="0.8">
        <traffic_constraints>
            <doors> <!-- doors states are: close or open -->
                <door trans_id="2" caption="NaN" state="open" />
                <door trans_id="6" caption="NaN" state="open" outflow="1.0" max_agents="20"/>
            </doors>
        </traffic_constraints>
    </JPScore>
 */

void jpsDatamanager::writeTrafficXML(QFile &file)
{
    QXmlStreamWriter *stream = new QXmlStreamWriter(&file);
    QList<jpsCrossing *> crossings =get_crossingList();
    QList<jpsCrossing *> doorlist;

    for(jpsCrossing *crossing:crossings)
    {
        if(crossing->IsExit())
            doorlist.append(crossing);
    }

    stream->setAutoFormatting(true);

    stream->writeStartDocument("1.0",true);
    stream->setCodec("UTF-8");

    stream->writeStartElement("JPScore");
    stream->writeAttribute("project","JPS-Project");
    stream->writeAttribute("version", "0.8");

    stream->writeStartElement("traffic_constrains");
    stream->writeStartElement("doors");
    writeTraffics(stream, doorlist);
    doorlist.clear();
    stream->writeEndElement(); //end doors

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
}

void jpsDatamanager::writeTraffics(QXmlStreamWriter *stream, QList<jpsCrossing *> const &doorlist)
{
    for(jpsCrossing* door:doorlist)
    {
        stream->writeStartElement("door");

        stream->writeAttribute("trans_id", QString::number(door->get_id()));
        stream->writeAttribute("caption", "NaN");

        if(door->isState())
            stream->writeAttribute("state", "open");
        else
            stream->writeAttribute("state", "close");

        if(!door->getOutflow().isEmpty())
            stream->writeAttribute("outflow", door->getOutflow());

        if(!door->getMaxAgents().isEmpty())
            stream->writeAttribute("max_agents", door->getMaxAgents());

        stream->writeEndElement(); //end door
    }
}

bool jpsDatamanager::readTrafficXML(QFile &file)
{

    QXmlStreamReader xmlReader(&file);

    xmlReader.readNext(); // read first token

    while (!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement() && xmlReader.name() == QLatin1String("doors"))
        {
            readDoor(xmlReader);
        } else
        {
            xmlReader.readNext();
        }
    }

    return true;
}

void jpsDatamanager::readDoor(QXmlStreamReader &xmlReader)
{
    bool state;
    QString max_agents;
    QString outflow;
    int id;

    while (xmlReader.readNextStartElement())
    {
        if(xmlReader.name() == QLatin1String("door"))
        {
            state = xmlReader.attributes().value("state") == "open" ? true : false;

            max_agents = xmlReader.attributes().value("max_agents").toString();
            outflow = xmlReader.attributes().value("outflow").toString();
            id = xmlReader.attributes().value("trans_id").toInt();

            for(jpsCrossing *door : crossingList)
            {
                if(door->get_id() == id && door->IsExit())
                {
                    door->setState(state);
                    door->setMaxAgents(max_agents);
                    door->setOutflow(outflow);
                }
            }

            // now token is end element, readNextStartElement() will return false. Have to use readNext
            // to go to next start element.
            xmlReader.readNext();
        }
    }
}

const QList<JPSSource *> &jpsDatamanager::getSourcelist()
{
    sourcelist.clear();
    sourcelist = _mView->getSources();

    return sourcelist;
}

