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
 * The class is responsible for handling and organisation of all elements and zones. Further more
 * it handles the reading and writing of dxf and xml files.
 *
 * The class should control all opreations for editing data. Editing data outside only by setter!
 *
 * \section Structure of JPSElement
 *
 **/


#include "datamanager.h"

using myClock = std::chrono::high_resolution_clock;


jpsDatamanager::jpsDatamanager(QWidget *parent, jpsGraphicsView *view)
{
    qDebug("Enter jpsDatamanager::jpsDatamanager");
    parent_widget=parent;

    _mView=view;
    obs_id_counter=0;
    _crossingIdCounter=1;
    //_frameRate=0;
    _landmarkCounter=0;
    _regionCounter=0;

    myClock::duration d = myClock::now().time_since_epoch();

    auto seed = d.count();

    _generator=std::default_random_engine(seed);


    // For zone
    roomlist = QList<JPSZone *> ();
    zone_id = 1;

    transition_id = 1;

//    sourcelist = _mView->getSources();
//    goallist = _mView->getGoals();
    qDebug("Leave jpsDatamanager::jpsDatamanager");
}

jpsDatamanager::~jpsDatamanager()
{
    qDebug("Enter jpsDatamanager::~jpsDatamanager");
    remove_all();
    qDebug("Leave jpsDatamanager::~jpsDatamanager");
}

/*
    since v0.8.9

    Create new Room;
 */
void jpsDatamanager::addRoom()
{
    qDebug("Enter jpsDatamanager::addRoom. now zone_id = %d", zone_id);
    JPSZone* new_room = new JPSZone(this->zone_id, nullptr, Room);
    roomlist.push_back(new_room);
    zone_id+=1;
    qDebug("Leave jpsDatamanager::addRoom. now zone_id = %d", zone_id);
}

void jpsDatamanager::addPlatform(JPSZone *father_room)
{
    qDebug("Enter jpsDatamanager::addPlatform");
    auto *new_Platform = new JPSZone(zone_id, father_room, Platform);

    if(father_room != nullptr)
        father_room->addZoneInList(new_Platform); // Platform belongs to the father room and saving in father room,

    // rather than datamanger;
    zone_id += 1;
    qDebug("Leave jpsDatamanager::addPlatform");
}

void jpsDatamanager::addCorridor(JPSZone *father_room)
{
    qDebug("Enter jpsDatamanager::addCorridor");
    auto *new_Corridor = new JPSZone(zone_id, father_room, Corridor);// Corridor belongs to the father room and
    // saving in father room, rather than datamanger;

    if(father_room != nullptr)
        father_room->addZoneInList(new_Corridor);

    zone_id += 1;
    qDebug("Leave jpsDatamanager::addCorridor");
}

void jpsDatamanager::addLobby(JPSZone *father_room)
{
    qDebug("Enter jpsDatamanager::addLobby");
    auto *new_Lobby = new JPSZone(zone_id, father_room, Lobby);// Lobby belongs to the father room and
    // saving in father room, rather than datamanger;

    if(father_room != nullptr)
        father_room->addZoneInList(new_Lobby);

    zone_id += 1;
    qDebug("Leave jpsDatamanager::addLobby");
}

void jpsDatamanager::addOffice(JPSZone *father_room)
{
    qDebug("Enter jpsDatamanager::addOffice");
    auto *new_Office = new JPSZone(zone_id, father_room, Office);// Office belongs to the father room and
    // saving in father room, rather than datamanger;

    if(father_room != nullptr)
        father_room->addZoneInList(new_Office);

    zone_id += 1;
    qDebug("Leave jpsDatamanager::addOffice");
}

void jpsDatamanager::addStair(JPSZone *father_room)
{
    qDebug("Enter jpsDatamanager::addStair");
    auto *new_Stair = new JPSZone(zone_id, father_room, Stair);// Corridor belongs to the father room and
    // saving in father room, rather than datamanger;

    if(father_room != nullptr)
        father_room->addZoneInList(new_Stair);

    zone_id += 1;
    qDebug("Leave jpsDatamanager::addStair");
}

void jpsDatamanager::remove_room(JPSZone *room)
{
    qDebug("Enter jpsDatamanager::remove_room. zone_id = %d", zone_id);
    //set room to nullptr in doors
    for (jpsCrossing* crossing : get_crossingList())
    {
        // will only be removed if room is in crossings roomlist
        crossing->removeSubRoom(room);
    }

    //if (roomlist.size()>0)
    //{
    int roomID=room->get_id();
    roomlist.removeOne(room);
    delete room;
    room = nullptr;
    //change IDs of other room with ID greater than roomID
    for (JPSZone* otherroom:roomlist)
        if (otherroom->get_id()>roomID)
            otherroom->set_id(otherroom->get_id()-1);


    //}
    qDebug("Leave jpsDatamanager::remove_room. zone_id = %d", zone_id);
}

void jpsDatamanager::change_roomName(JPSZone* room, QString name)
{
     qDebug("Enter jpsDatamanager::change_roomName with name=<%s>", name.toStdString().c_str());
    room->setName(name);
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

QList<JPSZone *> jpsDatamanager::get_roomlist()
{
    qDebug("Enter/return jpsDatamanager::get_roomlist");
    return this->roomlist;
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
    obs->setName(name);
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
        if ((newCrossings[i]->is_Crossing() || newCrossings[i]->is_Transition())
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
    if (newCrossing->is_Crossing() && !isInCrossingList(newCrossing))
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
    if (roomlist.size()>0)
    {
        for (JPSZone *room : roomlist)
        {
            // door will only be removed if belongs to room (see method removeDoor of JPSZone)
            room->removeCrossing(crossing);
        }
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


QList<jpsTransition *> jpsDatamanager::getTransitionList()
{
     qDebug("Enter/Return jpsDatamanager::getTransitionList");
    return transition_list;
}

void jpsDatamanager::new_exit(QList <jpsLineItem *> newExits)
{
    qDebug("Enter jpsDatamanager::newTransition QList");
    for (int i=0; i<newExits.size(); i++)
    {
        //if (newExits[i]->is_Transition())
        //{
        jpsTransition* newExit = new jpsTransition(newExits[i]);
        transition_list.push_back(newExit);
        //}
    }
    qDebug("Leave jpsDatamanager::newTransition QList");
}

void jpsDatamanager::newTransition(jpsLineItem *transition)
{
    qDebug("Enter jpsDatamanager::newTransition");
    auto *newTr = new jpsTransition(transition);
    newTr->set_id(transition_id);
    transition_id += 1;
    transition_list.push_back(newTr);
    qDebug("Leave jpsDatamanager::newTransition");
}

void jpsDatamanager::remove_exit(jpsTransition *exit)
{
     qDebug("Enter jpsDatamanager::remove_exit");
    if (transition_list.size()>0)
    {
        transition_list.removeOne(exit);
        delete exit;
    }
    qDebug("Leave jpsDatamanager::remove_exit");
}

void jpsDatamanager::removeAllTransition()
{
     qDebug("Enter jpsDatamanager::removeAllTransition");
    for (int i=0; i<transition_list.size(); i++)
    {
        delete transition_list[i]; // delete jpsTransition
        transition_list[i] = nullptr;
    }
    transition_list.clear();

    qDebug("Leave jpsDatamanager::removeAllTransition");
}

QList<jpsLandmark *> jpsDatamanager::get_landmarks()
{
    qDebug("Enter/Return jpsDatamanager::get_landmarks");
    return _landmarks;
}

void jpsDatamanager::new_landmark(jpsLandmark *newlandmark)
{
    qDebug("Enter jpsDatamanager::new_landmark");
    newlandmark->SetId(_landmarkCounter);
    _landmarkCounter++;
    _landmarks.push_back(newlandmark);
    qDebug("Leave jpsDatamanager::new_landmark");
}

void jpsDatamanager::remove_landmark(jpsLandmark *landmark)
{
    qDebug("Enter jpsDatamanager::remove_landmark");
    _landmarks.removeOne(landmark);
    for (jpsConnection* connection:landmark->GetConnections())
    {
        RemoveConnection(connection);
    }
    delete landmark;
    _landmarkCounter++;
    qDebug("Leave jpsDatamanager::remove_landmark");
}

void jpsDatamanager::change_LandmarkName(jpsLandmark *landmark, QString name)
{
    qDebug("Enter/Return jpsDatamanager::change_LandmarkName");
    landmark->SetCaption(name);
}

void jpsDatamanager::remove_all_landmarks()
{
    qDebug("Enter jpsDatamanager::remove_all_landmarks");
    if(_landmarks.isEmpty())
        return;

    for (jpsLandmark* landmark:_landmarks)
    {
        delete landmark;
        landmark = nullptr;
    }
    _landmarks.clear();
    qDebug("Leave jpsDatamanager::remove_all_landmarks");
}

const int &jpsDatamanager::GetLandmarkCounter() const
{
    qDebug("Enter/Return jpsDatamanager::GetLandmarkCounter");
    return _landmarkCounter;
}

const QList<jpsConnection *> &jpsDatamanager::GetAllConnections() const
{
    qDebug("Enter/Return jpsDatamanager::GetAllConnections");
    return _landmarkConnections;
}

void jpsDatamanager::NewConnection(jpsConnection *newConnection)
{
    qDebug("Enter jpsDatamanager::NewConnection");
    _landmarkConnections.push_back(newConnection);
    qDebug("Leave jpsDatamanager::NewConnection");
}

void jpsDatamanager::RemoveConnection(jpsConnection *connection)
{
    qDebug("Enter jpsDatamanager::RemoveConnection");
    _landmarkConnections.removeOne(connection);
    connection->GetLandmarks().first->RemoveConnection(connection);
    connection->GetLandmarks().second->RemoveConnection(connection);
    delete connection->GetLineItem();
    delete connection;
    qDebug("Leave jpsDatamanager::RemoveConnection");
}

void jpsDatamanager::RemoveAllConnections()
{
    qDebug("Enter jpsDatamanager::RemoveAllConnections");
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
    qDebug("Leave jpsDatamanager::RemoveAllConnections");
}

const QList<jpsRegion *> &jpsDatamanager::GetRegions() const
{
    qDebug("Enter/Return jpsDatamanager::GetRegions");
    return _regions;
}

void jpsDatamanager::NewRegion(jpsRegion *region)
{
    qDebug("Enter jpsDatamanager::NewRegion");
    _regions.push_back(region);
    _regionCounter++;
    qDebug("Leave jpsDatamanager::NewRegion");
}

void jpsDatamanager::RemoveRegion(jpsRegion *region)
{
    qDebug("Enter jpsDatamanager::RemoveRegion");
    _regions.removeOne(region);
    delete region;
    qDebug("Leave jpsDatamanager::RemoveRegion");

}

void jpsDatamanager::RemoveAllRegions()
{
    qDebug("Enter jpsDatamanager::RemoveAllRegions");
    for (jpsRegion* region:_regions)
    {
        delete region;
    }
    _regions.clear();
    qDebug("Leave jpsDatamanager::RemoveAllRegions");
}

const int &jpsDatamanager::GetRegionCounter() const
{
    qDebug("Enter/Return jpsDatamanager::GetRegionCounter");
    return _regionCounter;
}

void jpsDatamanager::writeXML(QFile &file)
{
    qDebug("Enter jpsDatamanager::writeXML");
    auto *stream = new QXmlStreamWriter(&file);

    QList<jpsLineItem* > lines = _mView->get_line_vector(); //lines are all unassign jpsLineItem

    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    // Header for geometry
    writeHeader(stream);

    //write rooms
    stream->writeStartElement("rooms");
    writeRooms(stream,lines);
    stream->writeEndElement(); // End rooms

    // Write transitions
    stream->writeStartElement("transitions");
    writeTransitions(stream,lines);
    stream->writeEndElement();// End transitions

    // write unassignd lines
    stream->writeStartElement("undefine");
    writeNotAssignedLines(stream, lines);
    stream->writeEndElement(); // End undefine

    stream->writeEndElement(); // End geometry

    stream->writeEndDocument(); // End document

    delete stream;
    stream = nullptr;
    qDebug("Leave jpsDatamanager::writeXML");
}

void jpsDatamanager::writeRoutingXML(QFile &file) // Construction side
{
    qDebug("Enter jpsDatamanager::writeRoutingXML");
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
    qDebug("Leave jpsDatamanager::writeRoutingXML");

}

void jpsDatamanager::writeLineItems(QFile &file)
{
    qDebug("Enter jpsDatamanager::writeItems");

    for (jpsLineItem* line:_mView->get_line_vector())
    {
        QString string = QString::number(line->get_line()->line().p1().x())+ "," + QString::number(line->get_line()->line().p1().y())+ ";\n"+
                QString::number(line->get_line()->line().p2().x())+ "," + QString::number(line->get_line()->line().p2().y())+ "; \n";
        file.write(string.toUtf8());
    }
    qDebug("Leave jpsDatamanager::writeItems");
}

void jpsDatamanager::WriteCognitiveMapXML(QFile &file, bool fuzzy)
{
    qDebug("Enter jpsDatamanager::WriteCognitiveMapXML");
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    WriteCognitiveMapHeader(stream);

    //write regions incl. their landmarks and connections
    stream->writeStartElement("regions");
    WriteRegions(stream,fuzzy);
    stream->writeEndElement();//regions

    stream->writeEndElement();//cognitiveMap

    stream->writeEndDocument();

    delete stream;
    qDebug("Leave jpsDatamanager::WriteCognitiveMapXML");
}

void jpsDatamanager::WriteCognitiveMapXML(QFile &file, int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::WriteCognitiveMapXML");
    QXmlStreamWriter stream(&file);
    WriteCognitiveMapHeader(&stream);

    //write regions incl. their landmarks and connections
    stream.writeStartElement("regions");
    WriteRegions(&stream,k,m,p0);
    stream.writeEndElement();//regions

    stream.writeEndElement();//cognitiveMap

    stream.writeEndDocument();
    qDebug("Leave jpsDatamanager::WriteCognitiveMapXML");
}

void jpsDatamanager::WriteCognitiveMapHeader(QXmlStreamWriter *stream)
{
    qDebug("Enter WriteCognitiveMapHeader");
    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("cognitiveMap");
    stream->writeAttribute("version", "0.81");
    stream->writeAttribute("caption","cogMap");
    stream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/jps_geometry.xsd");
    stream->writeAttribute("unit","m");
    qDebug("Leave WriteCognitiveMapHeader");
}

void jpsDatamanager::WriteRegions(QXmlStreamWriter *stream, bool fuzzy)
{
    qDebug("Enter jpsDatamanager::WriteRegions");
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
    qDebug("Leave jpsDatamanager::WriteRegions");
}

void jpsDatamanager::WriteRegions(QXmlStreamWriter *stream, int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::WriteRegions");
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
    qDebug("Leave jpsDatamanager::WriteRegions");
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
    qDebug("Enter jpsDatamanager::writeHLines");
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
    qDebug("Leave jpsDatamanager::writeHLines");
}

QString jpsDatamanager::RoomIDHLine(jpsLineItem *lineItem)
{
    qDebug("Enter jpsDatamanager::RoomIDHLine");
    for (JPSZone* room:roomlist)
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
    qDebug("Leave jpsDatamanager::RoomIDHLine");
    return "Warning! HLine outside geometry";

}

void jpsDatamanager::writeRooms(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    qDebug(" Enter jpsDatamanager::writeRooms");

    for (JPSZone* room:roomlist)
    {
        stream->writeStartElement("room");
        stream->writeAttribute("id", QString::number(room->get_id()));
        stream->writeAttribute("caption",room->getName());

        writeSubRoom(stream,room,lines);

        writeCrossings(stream, room, lines);

        stream->writeEndElement();//room
    }
    qDebug(" Leave jpsDatamanager::writeRooms");
}

void jpsDatamanager::writeSubRoom(QXmlStreamWriter *stream, JPSZone *room, QList<jpsLineItem *> &lines)
{
    qDebug("Enter jpsDatamanager::writeSubRoom");

    for(QList<JPSZone *> zonelist:room->getZoneList())
    {
        for(JPSZone *zone:zonelist)
        {
            stream->writeStartElement("subroom");

            stream->writeAttribute("id",QString::number(zone->get_id()));
            stream->writeAttribute("caption",zone->getName());
            stream->writeAttribute("class", zone->getTypeInString());

            zone->correctPlaneCoefficients(getTransitionInSubroom(zone));

            stream->writeAttribute("A_x",QString::number(zone->get_ax()));
            stream->writeAttribute("B_y",QString::number(zone->get_by()));
            stream->writeAttribute("C_z",QString::number(zone->get_cz()));

            //walls, for all zone except platform
            QList<jpsLineItem *> wallList = zone->get_listWalls();
            QList<JPSTrack *> tracklist = zone->getTrackList();

            if(zone->getType() == Platform)
            {
                for(JPSTrack *track:zone->getTrackList())
                {
                    stream->writeStartElement("polygon");
                    stream->writeAttribute("caption","track");
                    stream->writeAttribute("type","track-"+QString::number(track->getType()));

                    stream->writeStartElement("vertex");
                    stream->writeAttribute("px",QString::number(track->getLine()->get_line()->line().x1()));
                    stream->writeAttribute("py",QString::number(track->getLine()->get_line()->line().y1()));
                    stream->writeEndElement(); //vertex

                    stream->writeStartElement("vertex");
                    stream->writeAttribute("px",QString::number(track->getLine()->get_line()->line().x2()));
                    stream->writeAttribute("py",QString::number(track->getLine()->get_line()->line().y2()));
                    stream->writeEndElement(); //vertex

                    stream->writeEndElement(); //polygon

                    lines.removeOne(track->getLine());
                }
            }
            else
            {
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

                    //remove wall from line vector
                    lines.removeOne(wallList[j]);
                }
            }

            for (int k=0; k<obstaclelist.size(); k++)
            {
                if (room==obstaclelist[k]->get_room())
                {
                    writeObstacles(stream ,obstaclelist[k],lines);
                }
            }

            // up and down, for stair
            if (zone->getType() == Stair)
            {
                // <up>
                stream->writeStartElement("up");
                stream->writeAttribute("px",QString::number( zone->get_up().x()));
                stream->writeAttribute("py",QString::number( zone->get_up().y()));
                stream->writeEndElement();
                // </up>
                // <down>
                stream->writeStartElement("down");
                stream->writeAttribute("px",QString::number( zone->get_down().x()));
                stream->writeAttribute("py",QString::number( zone->get_down().y()));
                stream->writeEndElement();
                // </down>
            }

            stream->writeEndElement();//subroom
        }
    }

    qDebug("Leave jpsDatamanager::writeSubRoom");
}

void jpsDatamanager::writeCrossings(QXmlStreamWriter *stream, JPSZone *room, QList<jpsLineItem *> &lines)
{
    qDebug("Enter jpsDatamanager::writeCrossings");
    stream->writeStartElement("crossings");

    if(room == nullptr)
        return;

    QString string = "";

    for (jpsCrossing *crossing : room->getCrossingList())
    {

        if (crossing->get_roomList().size() == 2 // A crossing must between two subrooms or romm and stair
            && !(crossing->get_roomList()[0]->getType()==Stair
            && crossing->get_roomList()[1]->getType()==Stair)) // both sides can't be stair at the same time
        {
            stream->writeStartElement("crossing");
            stream->writeAttribute("id", QString::number(crossing->get_id()));

            stream->writeAttribute("subroom1_id",QString::number(crossing->get_roomList()[0]->get_id()));
            stream->writeAttribute("subroom2_id",QString::number(crossing->get_roomList()[1]->get_id()));

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossing->get_cLine()->get_line()->line().x1()));
            stream->writeAttribute("py",QString::number(crossing->get_cLine()->get_line()->line().y1()));
            stream->writeEndElement(); //vertex

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(crossing->get_cLine()->get_line()->line().x2()));
            stream->writeAttribute("py",QString::number(crossing->get_cLine()->get_line()->line().y2()));
            stream->writeEndElement(); //vertex

            stream->writeEndElement(); //crossing

            lines.removeOne(crossing->get_cLine());
        }
        else
        {
            string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                    crossing->get_cLine()->get_line()->line().x1(),
                    crossing->get_cLine()->get_line()->line().x2(),
                    crossing->get_cLine()->get_line()->line().y1(),
                    crossing->get_cLine()->get_line()->line().y2());

            QMessageBox msgBox;
            msgBox.setText("This crossing incorrect \n It will be saved in unsigned lines");
            msgBox.setDetailedText(string);
            msgBox.setStandardButtons(QMessageBox::Ok);

            int ret = msgBox.exec();

            switch (ret)
            {
                case QMessageBox::Ok:
                    // Ok was clicked
                    break;
                case QMessageBox::Cancel:
                    break;
                default:
                    // should never be reached
                    break;
            }
        }
    }
    stream->writeEndElement();//crossings
    qDebug("Leave jpsDatamanager::writeCrossings");
}

void jpsDatamanager::writeTransitions(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    qDebug("Enter jpsDatamanager::writeTransitions");

    for(jpsTransition *transition : transition_list)
    {
        stream->writeStartElement("transition");
        stream->writeAttribute("id",QString::number(transition->get_id()));
        stream->writeAttribute("caption","exit");
        stream->writeAttribute("type","emergency");

        // room 1
        if(transition->get_roomList().isEmpty() || transition->get_roomList()[0] == nullptr)
        {
            //outside
            stream->writeAttribute("room1_id","-1");
            stream->writeAttribute("subroom1_id","-1");
        } else
        {
            //room
            stream->writeAttribute("room1_id",QString::number(transition->get_roomList()[0]->getFatherRoom()->get_id()));
            stream->writeAttribute("subroom1_id",QString::number(transition->get_roomList()[0]->get_id()));
        }

        // room 2
        if(transition->get_roomList().isEmpty() || transition->get_roomList()[1] == nullptr)
        {
            //outside
            stream->writeAttribute("room2_id","-1");
            stream->writeAttribute("subroom2_id","-1");
        } else
        {
            //room
            stream->writeAttribute("room2_id",QString::number(transition->get_roomList()[1]->getFatherRoom()->get_id()));
            stream->writeAttribute("subroom2_id",QString::number(transition->get_roomList()[1]->get_id()));
        }

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(transition->get_cLine()->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(transition->get_cLine()->get_line()->line().y1()));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(transition->get_cLine()->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(transition->get_cLine()->get_line()->line().y2()));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //transition

        lines.removeOne(transition->get_cLine());
    }
    qDebug("Leave jpsDatamanager::writeTransitions");
}

void jpsDatamanager::writeObstacles(QXmlStreamWriter *stream, jpsObstacle* obs, QList<jpsLineItem* >& lines)
{
    qDebug("Enter jpsDatamanager::writeObstacles");
    stream->writeStartElement("obstacle");
    stream->writeAttribute("id",QString::number(obs->get_id()));
    stream->writeAttribute("caption",obs->getName());
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
    qDebug("Leave jpsDatamanager::writeObstacles");

}

void jpsDatamanager::writeNotAssignedLines(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
     qDebug("Enter jpsDatamanager::writeNotAssignedLines");

    /// save lines which are not assigned to a room yet
    stream->writeStartElement("Room");

    stream->writeAttribute("id",QString::number(-1));
    stream->writeAttribute("caption","not assigned lines");
    stream->writeAttribute("class","container");

    stream->writeStartElement("subroom");

    stream->writeAttribute("id",QString::number(-1));

    for (jpsLineItem* line:lines)
    {
        stream->writeStartElement("polygon");
        stream->writeAttribute("caption",line->getType());

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
    stream->writeEndElement(); //subroom
    stream->writeEndElement(); //room
    qDebug("Leave jpsDatamanager::writeNotAssignedLines");
}

void jpsDatamanager::WriteLandmarks(jpsRegion* cRegion, QXmlStreamWriter *stream, bool fuzzy)
{
    qDebug("Enter jpsDatamanager::WriteLandmarks");
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
    qDebug("Leave jpsDatamanager::WriteLandmarks");


}

void jpsDatamanager::WriteLandmarks(jpsRegion *cRegion, QXmlStreamWriter *stream, int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::WriteLandmarks");
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
    qDebug("Leave jpsDatamanager::WriteLandmarks");
}

void jpsDatamanager::CutOutLandmarks()
{
    qDebug("Enter jpsDatamanager::CutOutLandmarks");
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
    qDebug("Leave jpsDatamanager::CutOutLandmarks");

}

void jpsDatamanager::CutOutLandmarks(int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::CutOutLandmarks");

    //int numberMainTargets = GetNumberOfMainTargets();
    //int statcutMainTarget= numberMainTargets;

    int number=-1;


    QList<jpsLandmark* > cLandmarks = _landmarksAfterLoose;
    double prob=GetProbability(k,m,p0);



    for (jpsLandmark* landmark:cLandmarks)
    {
//        // at least one main target will be kept
//        if (landmark->GetType()=="main" || landmark->getType()=="Main Target")
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
    qDebug("Leave jpsDatamanager::CutOutLandmarks");
}

double jpsDatamanager::GetProbability(int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::GetProbability");
    double val = p0+m*k;
    if (val>1)
        val=1;
    else if (val<0)
        val=0;
    qDebug("Leave jpsDatamanager::GetProbability");
    return val;
}

void jpsDatamanager::BridgeLostLandmark(jpsLandmark *landmark)
{
    qDebug("Enter jpsDatamanager::BridgeLostLandmark");
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
    qDebug("Leave jpsDatamanager::BridgeLostLandmark");
}

void jpsDatamanager::WriteConnections(jpsRegion* cRegion, QXmlStreamWriter *stream)
{
    qDebug("Enter jpsDatamanager::WriteConnections");
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
    qDebug("Leave jpsDatamanager::WriteConnections");
}

void jpsDatamanager::CreateAndSaveASimilarCogMap(const int& id)
{
    qDebug("Enter jpsDatamanager::CreateAndSaveASimilarCogMap");
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
    qDebug("Leave jpsDatamanager::CreateAndSaveASimilarCogMap");

}

void jpsDatamanager::CreateAndSaveASimilarCogMap(int id, int k, double m, double p0)
{
    qDebug("Enter jpsDatamanager::CreateAndSaveASimilarCogMap");
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
    qDebug("Leave jpsDatamanager::CreateAndSaveASimilarCogMap");

}

qreal jpsDatamanager::MakeItFuzzy(const qreal& mean, const qreal &std)
{
    qDebug("Enter jpsDatamanager::MakeItFuzzy");
    using myClock = std::chrono::high_resolution_clock;
    myClock::duration d = myClock::now().time_since_epoch();

    auto seed = d.count();

    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(mean,std);

    double number = distribution(generator);
    qDebug("Leave jpsDatamanager::MakeItFuzzy");

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

    // Delete JPSelements in data manager, not the QGraphicsitem
    removeAllTransition();
    removeAllGoal();
    removeAllSource();

    remove_all_rooms();
    remove_all_obstacles();

    remove_all_landmarks();
    RemoveAllConnections();
    RemoveAllRegions();

    zone_id=1;
    obs_id_counter=0;
    qDebug("Leave jpsDatamanager::remove_all");
}

void jpsDatamanager::remove_marked_lines()
{
    qDebug("Enter jpsDatamanager::remove_marked_lines");
    QList<jpsLineItem* > marked_lines = _mView->get_markedLines();
//    QList<jpsObstacle*> obstacle_list = this->get_obstaclelist();
    QList<JPSZone* > cList = this->get_roomlist();

    for (int i=0; i<marked_lines.size(); i++)
    {
//        // delete wall which in obstacle
//        if (marked_lines[i]->getType() == "wall" && obstacle_list.size()>0)
//        {
//            for (int m=0; m<obstacle_list.size(); m++)
//            {
//                QList<jpsLineItem* > deleted_obstacle_lines;
//                for (int n=0; n<obstacle_list[m]->get_listWalls().size(); n++)
//                {
//                    if(marked_lines[i]==obstacle_list[m]->get_listWalls()[n])
//                    {
//                        deleted_obstacle_lines.push_back(obstacle_list[m]->get_listWalls()[n]);
//                    }
//                }
//                obstacle_list[m]->removeWall(deleted_obstacle_lines);
//                qDebug()<< "jpsDatamanager::remove_marked_lines(): Obstacle line is deleted!";
//            }
//        }
//        else
//        {
//            qDebug()<< "jpsDatamanager::remove_marked_lines(): Marked line isn't in obstacle!";
//        }

        // delete wall
        if (marked_lines[i]->getType() == "wall" && cList.size()>0)
        {
            QList<jpsLineItem *> walls;

            for(auto room : cList)
            {
                for (auto corridor : room->getCorridorList())
                {
                    for(auto line : corridor->get_listWalls())
                    {
                        if(marked_lines[i] == line)
                        {
                            corridor->removeWall(line);
                            delete line;
                            line = nullptr;

                        }

                    }
                }

                for (auto lobby : room->getLobbyList())
                {
                    for(auto line : lobby->get_listWalls())
                    {
                        if(marked_lines[i] == line)
                        {
                            lobby->removeWall(line);
                            delete line;
                            line = nullptr;
                        }

                    }
                }

                for (auto office : room->getOfficeList())
                {
                    for(auto line : office->get_listWalls())
                    {
                        if(marked_lines[i] == line)
                        {
                            office->removeWall(line);
                            delete line;
                            line = nullptr;
                        }

                    }
                }

                for (auto stair : room->getStairList())
                {
                    for(auto line : stair->get_listWalls())
                    {
                        if(marked_lines[i] == line)
                        {
                            stair->removeWall(line);
                            delete line;
                            line = nullptr;
                        }

                    }
                }
            }

            qDebug()<< "jpsDatamanager::remove_marked_lines: Wall is removed" ;
        }

        // Delete track
        else if (marked_lines[i]->getType() == "track" && cList.size()>0)
        {
            for (auto room : cList)
            {
                for(auto platform : cList)
                {
                    for(auto track : platform->getTrackList())
                    {
                        if(marked_lines[i] == track->getLine())
                        {
                            platform->removeTrack(track);
                            delete track;
                            track = nullptr;
                        }
                    }
                }
            }
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Track is deleted!";
        }

        // Delete crossing
        else if (marked_lines[i]->getType() == "crossing" && cList.size()>0)
        {
            foreach(JPSZone *zone, cList)
            {
                foreach(jpsCrossing* crossing, zone->getCrossingList())
                {
                    if(marked_lines[i] == crossing->get_cLine())
                    {
                        zone->removeCrossing(crossing); // remove crossing from room

                        foreach(JPSZone* subroom, crossing->get_roomList())
                        {
                            subroom->removeEnterOrExit(crossing); // remove crossing from subroom
                        }

                        delete crossing;
                        crossing = nullptr;
                    }
                }
            }
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Crossing is deleted!";
        }

        // Delete transition
        else if(marked_lines[i]->getType() == "transition")
        {
            for (jpsTransition *tran : transition_list)
            {
                if(marked_lines[i] == tran->get_cLine())
                {
                    transition_list.removeOne(tran);
                    delete tran;
                    tran = nullptr;
                }
            }
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Transition is deleted!";
        }
        else
        {
            qDebug()<< "jpsDatamanager::remove_marked_lines(): Marked line isn't defined!";
        }
    }

    qDebug("Leave jpsDatamanager::remove_marked_lines()");
}

void jpsDatamanager::set_view(jpsGraphicsView *view)
{
    qDebug("Enter/Return jpsDatamanager::set_view");
    _mView=view;
}

jpsGraphicsView *jpsDatamanager::get_view()
{
    qDebug("Enter/Return jpsDatamanager::get_view");
    return _mView;
}

bool jpsDatamanager::readXML(QFile &file)
{
    qDebug("Enter jpsDatamanager::readXML");
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;

    if (!doc.setContent(&file, false, &errorStr, &errorLine,&errorColumn))
    {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
        return false;
    }

    QDomElement root = doc.documentElement();

    if (root.tagName() != "geometry") {
        std::cerr << "Error: Not a geometry file" << std::endl;
        return false;
    }

    parseGeometry(root);

    qDebug("Leave jpsDatamanager::readXML");
    return true;
}

void jpsDatamanager::parseGeometry(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseGeometry");
    QDomNode child = element.firstChild();

    while (!child.isNull())
    {
        if (child.toElement().tagName() == "rooms")
        {
            parseRooms(child.toElement());
        }
        else if (child.toElement().tagName() == "transitions")
        {
            parseTransitions(child.toElement());
        }
        else if (child.toElement().tagName() == "undefine")
        {
            parseUndefine(child.toElement());
        }
        child = child.nextSibling();
    }
    qDebug("Leave jpsDatamanager::parseGeometry");
}

void jpsDatamanager::parseRooms(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseRooms");
    QDomNode child = element.firstChild();

    while (!child.isNull())
    {
        if (child.toElement().tagName() == "room")
        {
            parseRoom(child.toElement());
        }

        child = child.nextSibling();
    }
    qDebug("Leave jpsDatamanager::parseRooms");
}

void jpsDatamanager::parseTransitions(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseTransitions");
    QDomNodeList xTransitionsList=element.elementsByTagName("transition");

    for (int i = 0; i < xTransitionsList.length(); i++) {
        QDomElement xTransition = xTransitionsList.item(i).toElement();
        QDomNodeList xVertices=xTransition.elementsByTagName("vertex");

        double x1=xVertices.item(0).toElement().attribute("px", "0").toDouble();
        double y1=xVertices.item(0).toElement().attribute("py", "0").toDouble();

        double x2=xVertices.item(1).toElement().attribute("px", "0").toDouble();
        double y2=xVertices.item(1).toElement().attribute("py", "0").toDouble();

        jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"transition");
        auto *transition = new jpsTransition(lineItem);

        transition->set_id(xTransition.attribute("id").toInt());
        transition->set_type(xTransition.attribute("type"));

        QString room1_id = xTransition.attribute("room1_id");
        QString room2_id = xTransition.attribute("room2_id");
        QString subroom1_id = xTransition.attribute("subroom1_id");
        QString subroom2_id = xTransition.attribute("subroom2_id");

        JPSZone *subroom1 = getSubroomWithID(room1_id, subroom1_id);
        JPSZone *subroom2 = getSubroomWithID(room2_id, subroom2_id);

        transition_list.append(transition);

        transition->set_rooms(subroom1,subroom2);
    }
    qDebug("Leave jpsDatamanager::parseTransitions");
}

void jpsDatamanager::parseUndefine(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseUndefine");

    QDomNodeList polygons = element.elementsByTagName("polygon");

    for (int i=0; i<polygons.size();i++)
    {
        QDomElement polygon = polygons.item(i).toElement();
        QDomNodeList xVertices = polygon.elementsByTagName("vertex");

        double x1=xVertices.item(0).toElement().attribute("px", "0").toDouble();
        double y1=xVertices.item(0).toElement().attribute("py", "0").toDouble();

        double x2=xVertices.item(1).toElement().attribute("px", "0").toDouble();
        double y2=xVertices.item(1).toElement().attribute("py", "0").toDouble();

        QString type = polygon.attribute("caption");
        _mView->addLineItem(x1,y1,x2,y2,"wall");
    }

    qDebug("Leave jpsDatamanager::parseUndefine");
}

void jpsDatamanager::parseRoom(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseRoom");
    addRoom(); // Create room

    roomlist.last()->set_id(element.attribute("id").toInt());
    roomlist.last()->setName(element.attribute("caption"));

    QDomNode child = element.firstChild();

    while (!child.isNull()) {
        if (child.toElement().tagName() == "subroom")
        {
            parseSubRoom(child.toElement());
        }
        else if (child.toElement().tagName() == "crossings")
        {
            parseCrossings(child.toElement());
        }
        child = child.nextSibling();
    }
    qDebug("Leave jpsDatamanager::parseRoom");
}

void jpsDatamanager::parseSubRoom(const QDomElement &element)
{
    qDebug("Enter jpsDatamanager::parseSubRoom");

    // Setup subroom
    JPSZone *current_subroom = nullptr;

    if(element.attribute("class") == "corridor" || element.attribute("class") == "subroom")
    {
        addCorridor(roomlist.last());
        current_subroom = roomlist.last()->getCorridorList().last();
    }else if(element.attribute("class") == "platform")
    {
        addPlatform(roomlist.last());
        current_subroom = roomlist.last()->getPlatfromList().last();
    }else if(element.attribute("class") == "lobby")
    {
        addLobby(roomlist.last());
        current_subroom = roomlist.last()->getLobbyList().last();
    }else if(element.attribute("class") == "office")
    {
        addOffice(roomlist.last());
        current_subroom = roomlist.last()->getOfficeList().last();
    }else if(element.attribute("class") == "stair")
    {
        addStair(roomlist.last());
        current_subroom = roomlist.last()->getStairList().last();
    } else{
        qDebug("Not vaild subroom type");
        return;
    }

    current_subroom->set_id(element.attribute("id").toInt());

    // Set subroom name
    if(element.hasAttribute("caption"))
        current_subroom->setName(element.attribute("caption"));
    else
        current_subroom->setName("room id:"
        + QString::number(roomlist.last()->get_id())
        +", subroom id:"
        + element.attribute("id"));

    // Set X Y Z
    if(element.hasAttribute("A_x"))
        current_subroom->set_ax(element.attribute("A_x").toFloat());
    else
        current_subroom->set_ax(0.0);

    if(element.hasAttribute("B_y"))
        current_subroom->set_by(element.attribute("B_y").toFloat());
    else
        current_subroom->set_by(0.0);

    auto elevation = element.attribute("C_z").toFloat();

    current_subroom->set_cz(elevation);
    current_subroom->set_elevation(elevation);

    // Parse walls
    QDomElement polygon = element.firstChildElement("polygon");
    double pos_count=1;

    while(!polygon.isNull()) {
        QDomNodeList xVertices=polygon.elementsByTagName("vertex");
        pos_count+=xVertices.count()-1;

        for( int i=0; i<xVertices.count()-1; i++)
        {
            //all unit are converted in cm
            double x1=xVertices.item(i).toElement().attribute("px", "0").toDouble();
            double y1=xVertices.item(i).toElement().attribute("py", "0").toDouble();
            double x2=xVertices.item(i+1).toElement().attribute("px", "0").toDouble();
            double y2=xVertices.item(i+1).toElement().attribute("py", "0").toDouble();

            if(polygon.attribute("caption") == "track")
            {
                jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"track");
                QString number = polygon.attribute("type").split("-").last();
                current_subroom->addTrack(lineItem, number);
            } else
            {
                jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"wall");
                current_subroom->addWall(lineItem);
            }
        }
        polygon = polygon.nextSiblingElement("polygon");
    }

    // Parse up and down for stairs
    if(current_subroom->getType() == Stair)
    {
        QDomNode child = element.firstChild();
        while (!child.isNull())
        {
            if (child.toElement().tagName() == "up")
            {
                double px = child.toElement().attribute("px").toDouble();
                double py = child.toElement().attribute("py").toDouble();
                current_subroom->set_up(QPointF(px, py));
            }
            else if (child.toElement().tagName() == "down")
            {
                double px = child.toElement().attribute("px").toDouble();
                double py = child.toElement().attribute("py").toDouble();
                current_subroom->set_down(QPointF(px, py));
            }

            child = child.nextSibling();
        }
    }
    qDebug("Leave jpsDatamanager::parseSubRoom");
}

bool jpsDatamanager::readRoutingXML(QFile &file)
{
    qDebug("Enter jpsDatamanager::readRoutingXML");
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

    qDebug("Leave jpsDatamanager::readRoutingXML");
    return true;
}

void jpsDatamanager::parseHline(QXmlStreamReader &xmlReader)
{

    qDebug("Enter jpsDatamanager::parseHline");

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
            _mView->addLineItem(x1,y1,x2,y2,"hline");
        }
    }
    qDebug("Leave jpsDatamanager::parseHline");
}

void jpsDatamanager::parseCrossings(const QDomElement &element)
{
    qDebug("Leave jpsDatamanager::parseCrossings");
    QDomNodeList crossings = element.elementsByTagName("crossing");

    for (int i = 0; i < crossings.length(); i++)
    {
        QDomElement xCrossing = crossings.item(i).toElement();

        QDomNodeList xVertices=xCrossing.elementsByTagName("vertex");

        QString id= xCrossing.attribute("id","-1");

        double x1=xVertices.item(0).toElement().attribute("px", "0").toDouble();
        double y1=xVertices.item(0).toElement().attribute("py", "0").toDouble();

        double x2=xVertices.item(1).toElement().attribute("px", "0").toDouble();
        double y2=xVertices.item(1).toElement().attribute("py", "0").toDouble();

        jpsLineItem* lineItem = _mView->addLineItem(x1,y1,x2,y2,"crossing");
        auto *crossing = new jpsCrossing(lineItem);

        QString subroom1_id = xCrossing.attribute("subroom1_id");
        QString subroom2_id = xCrossing.attribute("subroom2_id");

        JPSZone *subroom1 = getSubroomWithID(QString::number(roomlist.last()->get_id()), subroom1_id);
        JPSZone *subroom2 = getSubroomWithID(QString::number(roomlist.last()->get_id()), subroom2_id);

        // Add crossing in crossing_list of the room
        roomlist.last()->addCrossing(crossing);
        roomlist.last()->addCrossing(crossing);

        // Add crossing in enterAndExitList
        if(subroom1 != nullptr)
        {
            subroom1->addInEnterAndExitList(crossing);
        }

        if(subroom2 != nullptr)
        {
            subroom2->addInEnterAndExitList(crossing);
        }
    }
    qDebug("Leave jpsDatamanager::parseCrossings");
}

void jpsDatamanager::parseObstacles(QXmlStreamReader &xmlReader, JPSZone *room)
{
//    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
//                xmlReader.name() == "subroom"))
//    {
//
//
//        if (xmlReader.tokenType() == QXmlStreamReader::StartElement && xmlReader.name()=="obstacle")
//        {
//          int id = xmlReader.attributes().value("id").toString().toInt();
//            QString caption = xmlReader.attributes().value("caption").toString();
//
//            jpsObstacle* obs = new jpsObstacle(id);
//            obs->setName(caption);
//            obs->set_room(room);
//
//           // while (xmlReader.name()=="obstacle")
//           // {
//            //    xmlReader.readNext();
//            //}
//            this->parseWalls(xmlReader,obs);
//
//            obstaclelist.push_back(obs);
//        }
//
//        xmlReader.readNext();
//    }

}

bool jpsDatamanager::readDXF(std::string filename)
{
    qDebug("Enter jpsDatamanager::readDXF");
    DL_Dxf dxf;
    if (!dxf.in(filename, this))
    {
        qDebug("Leave jpsDatamanager::readDXF");
        return false;
    }
    else
    {
        _mView->AutoZoom();

        // Print unimported layer
        QMessageBox msgBox;
        QString detailied_text;

        QStringListIterator javaStyleIterator(unimported_layer);
        while (javaStyleIterator.hasNext())
        {
            detailied_text += javaStyleIterator.next() + "\n";
        }

        msgBox.setText("Geometry is loaded, but lines in these layer aren't loaded.");
        msgBox.setDetailedText(detailied_text);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        qDebug("Leave jpsDatamanager::readDXF");
        return true;
    }
}

void jpsDatamanager::addLine(const DL_LineData &d)
{
    qDebug("Enter jpsDatamanager::addLine");
    DL_Attributes attributes = DL_CreationInterface::getAttributes();
    auto layername = QString::fromStdString(attributes.getLayer());

    if (layername.contains("wall"))
    {
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"wall");
    }
    else if (layername.contains("crossing"))
    {
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"crossing");
    }
    else if (layername.contains("transition"))
    {
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"transition");
    }
    else if (layername.contains("track"))
    {
        _mView->addLineItem(d.x1,d.y1,d.x2,d.y2,"track");
    }
    else
    {
        if(!unimported_layer.contains(layername))
        {
            unimported_layer.append(layername);
        }
    }
    qDebug("Leave jpsDatamanager::addLine");
}

void jpsDatamanager::writeDXF(std::string filename)
{
    qDebug("Enter jpsDatamanager::writeDXF");
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
    qDebug("Leave jpsDatamanager::writeDXF");
}

void jpsDatamanager::writeDXFHeader(DL_Dxf *dxf, DL_WriterA *dw)
{
    qDebug("Enter jpsDatamanager::writeDXFHeader");
    dxf->writeHeader(*dw);
    dw->sectionEnd();
    qDebug("Leave jpsDatamanager::writeDXFHeader");
}

void jpsDatamanager::writeDXFTables(DL_Dxf *dxf, DL_WriterA *dw)
{
    qDebug("Enter jpsDatamanager::writeDXFTables");
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
    qDebug("Leave jpsDatamanager::writeDXFTables");



}

void jpsDatamanager::writeDXFBlocks(DL_Dxf *dxf, DL_WriterA *dw)
{
    qDebug("Enter jpsDatamanager::writeDXFBlocks");
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
    qDebug("Leave jpsDatamanager::writeDXFBlocks");
}

void jpsDatamanager::writeDXFEntities(DL_Dxf *dxf, DL_WriterA *dw)
{
    qDebug("Enter jpsDatamanager::writeDXFEntities");
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
    qDebug("Leave jpsDatamanager::writeDXFEntities");
}

void jpsDatamanager::writeDXFObjects(DL_Dxf *dxf, DL_WriterA *dw)
{
    qDebug("Enter jpsDatamanager::writeDXFObjects");
    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);
    qDebug("Leave jpsDatamanager::writeDXFObjects");
}

bool jpsDatamanager::ParseCogMap(QFile &file)
{
    qDebug("Enter jpsDatamanager::ParseCogMap");
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
    qDebug("Leave jpsDatamanager::ParseCogMap");
    return true;
}

void jpsDatamanager::ParseLandmark(jpsRegion *actRegion, QXmlStreamReader &xmlReader)
{
    qDebug("Enter jpsDatamanager::ParseLandmark");
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
    _mView->drawLandmark(QPointF(real_x, real_y));

    _landmarks.back()->SetId(id);
    _landmarks.back()->SetCaption(caption);
    _landmarks.back()->SetType(type);

    for (JPSZone* room:roomlist)
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
    qDebug("Leave jpsDatamanager::ParseLandmark");

}

void jpsDatamanager::ParseConnection(jpsRegion *actRegion, QXmlStreamReader &xmlReader)
{
    qDebug("Enter jpsDatamanager::ParseConnection");
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
    qDebug("Leave jpsDatamanager::ParseConnection");
}

bool jpsDatamanager::ReadLineFile(QFile &file)
{
    qDebug("Enter jpsDatamanager::ReadLineFile");
    QVector<jpsLineItem*> lineItems = _mView->get_line_vector().toVector();
    QTextStream in(&file);

    while ( !in.atEnd() )
    {       
        QString Qline = in.readLine();
        if (Qline.startsWith("Room"))
        {
            this->addRoom();
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
                  else if (lineItem->is_Crossing())
                  {
                      this->new_crossing(lineItem);

                      for (jpsCrossing* crossing:crossingList)
                      {
                          if (crossing->get_cLine()==lineItem)
                              crossing->setSubroom(this->roomlist.back());
                      }
                  }
                }
            }
        }
    }

    //for (JPSZone* room:this->roomlist)
    //{
    //    _mView->show_hide_roomCaption(room->getName(),room->get_center().x(),room->get_center().y());
    //}
    qDebug("Leave jpsDatamanager::ReadLineFile");
    return true;

}

jpsRegion* jpsDatamanager::ParseRegion(QXmlStreamReader &xmlReader)
{
    qDebug("Enter jpsDatamanager::ParseRegion");
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

    qDebug("Leave jpsDatamanager::ParseRegion");
    return actRegion;

}

bool LineIsEqual(const QLineF& line1, const QLineF& line2, double eps)
{
    qDebug("Enter LineIsEqual");
   if ((line1.p1().x()>=line2.p1().x()-eps && line1.p1().x()<=line2.p1().x()+eps) &&
          (line1.p1().y()>=line2.p1().y()-eps && line1.p1().y()<=line2.p1().y()+eps) &&
           (line1.p2().x()>=line2.p2().x()-eps && line1.p2().x()<=line2.p2().x()+eps) &&
           (line1.p2().y()>=line2.p2().y()-eps && line1.p2().y()<=line2.p2().y()+eps))
   {
       qDebug("Leave LineIsEqual");
       return true;
   }
   else if ((line1.p1().x()>=line2.p2().x()-eps && line1.p1().x()<=line2.p2().x()+eps) &&
               (line1.p1().y()>=line2.p2().y()-eps && line1.p1().y()<=line2.p2().y()+eps) &&
                (line1.p2().x()>=line2.p1().x()-eps && line1.p2().x()<=line2.p1().x()+eps) &&
                (line1.p2().y()>=line2.p1().y()-eps && line1.p2().y()<=line2.p1().y()+eps))
   {
       qDebug("Leave LineIsEqual");
       return true;
   }
   else
       qDebug("Leave LineIsEqual");
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
        qDebug("Enter jpsDatamanager::writeSources");
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
    qDebug("Leave jpsDatamanager::writeSources");
}

void jpsDatamanager::writeSourceXML(QFile &file) {
    qDebug("Enter jpsDatamanager::writeSourceXML");
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
    qDebug("Leave jpsDatamanager::writeSourceXML");
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
    qDebug("Enter jpsDatamanager::writeGoalXML");
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
    qDebug("jpsDatamanager::writeGoalXML");
}

void jpsDatamanager::writeGoals(QXmlStreamWriter *stream, QList<JPSGoal *> &goallist)
{
    qDebug("Enter jpsDatamanager::writeGoals");
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

            stream->writeEndElement(); // End polygon

            stream->writeEndElement(); // End goal
        }
    }
    qDebug("Leave jpsDatamanager::writeGoals");
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
    qDebug("Enter jpsDatamanager::writeTransitionXML");
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = _mView->get_line_vector();

    stream->setAutoFormatting(true);

    stream->writeStartDocument("1.0",true);
    stream->setCodec("UTF-8");

    stream->writeStartElement("JPScore");
    stream->writeStartElement("transitions");
    writeTransitions(stream, lines);
    stream->writeEndElement();//transitions

    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
    qDebug("Leave jpsDatamanager::writeTransitionXML");
}

const QList<JPSGoal *> &jpsDatamanager::getGoallist()
{
    qDebug("Enter jpsDatamanager::getGoallist");
    goallist.clear();
    goallist = _mView->getGoals();
    qDebug("Leave jpsDatamanager::getGoallist");
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
    qDebug("Enter jpsDatamanager::writeTrafficXML");
    auto *stream = new QXmlStreamWriter(&file);

    QList<jpsCrossing *> doorlist;

    for(JPSZone *room : roomlist)
    {
        for(jpsCrossing *door : room->getCrossingList())
        {
            doorlist.append(door);
        }
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

    stream->writeEndElement(); //end doors
    stream->writeEndDocument();

    delete stream;
    stream = nullptr;
    qDebug("Leave jpsDatamanager::writeTrafficXML");
}

void jpsDatamanager::writeTraffics(QXmlStreamWriter *stream, QList<jpsCrossing *> const &doorlist)
{
    qDebug("Enter jpsDatamanager::writeTraffics");
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
    qDebug("Leave jpsDatamanager::writeTraffics");
}

bool jpsDatamanager::readTrafficXML(QFile &file)
{
    qDebug("Enter jpsDatamanager::readTrafficXML");
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
    qDebug("Leave jpsDatamanager::readTrafficXML");
    return true;
}

void jpsDatamanager::readDoor(QXmlStreamReader &xmlReader)
{
    qDebug("Enter jpsDatamanager::readDoor");
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
//                if(door->get_id() == id && door->IsExit())
//                {
//                    door->setState(state);
//                    door->setMaxAgents(max_agents);
//                    door->setOutflow(outflow);
//                }
            }

            // now token is end element, readNextStartElement() will return false. Have to use readNext
            // to go to next start element.
            xmlReader.readNext();
        }
    }
    qDebug("Leave jpsDatamanager::readDoor");
}

const QList<JPSSource *> &jpsDatamanager::getSourcelist()
{
    qDebug("Enter jpsDatamanager::getSourcelist");
    sourcelist.clear();
    sourcelist = _mView->getSources();
    qDebug("Leave jpsDatamanager::getSourcelist");
    return sourcelist;
}

ZoneType jpsDatamanager::convertToZoneType(const QString string)
{
    qDebug("Enter jpsDatamanager::convertToZoneType");
    if(string == "Room")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Room;
    }
    else if(string == "Corridor")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Corridor;
    }
    else if(string == "Office")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Office;
    }
    else if(string == "Lobby")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Lobby;
    }
    else if(string == "Entrance")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Entrance;
    }
    else if(string == "Stair")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Stair;
    }
    else if(string == "Obstacle")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Obstacle;
    }
    else if(string == "Platform")
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return Platform;
    }
    else
    {
        qDebug("Leave jpsDatamanager::convertToZoneType");
        return NotAssigned;
    }
}

const QList<JPSZone *> &jpsDatamanager::getRoomlist() const
{
    qDebug("Enter jpsDatamanager::getRoomlist()");
    return roomlist;
    qDebug("Leave jpsDatamanager::getRoomlist()");
}

/*
    Purpose: Remove selected room from roomlist

    Note: Assemable elements will be deleted, but drawing elements will be kept in datamanager
*/
void jpsDatamanager::removeRoom(JPSZone *room)
{
    qDebug("Enter jpsDatamanager::removeRoom");
    if(room == nullptr)
        return;

    roomlist.removeOne(room); //removed from roomlist

    delete room;
    room = nullptr; //removed from memory
    qDebug("Leave jpsDatamanager::removeRoom");
}


void jpsDatamanager::removeZone(JPSZone *room, JPSZone *zone)
{
    qDebug("Enter jpsDatamanager::removeZone");
    if(zone == nullptr)
    {
        return;
    } else
    {
        room->removeZoneFromList(zone);
    }
    qDebug("Leave jpsDatamanager::removeZone");
}

JPSZone *jpsDatamanager::getSubroomWithID(QString room_id, QString subroom_id)
{
    qDebug("Enter JPSZone::getSubroomWithID");
    JPSZone *current_room = nullptr;

    for(JPSZone *room : roomlist)
    {
        if(room->get_id() == room_id.toInt())
        {
            current_room = room;
            break;
        }

    }

    if(current_room == nullptr)
    {
        qDebug("No right room. Leave JPSZone::getSubroomWithID");
        return nullptr;
    }
    else
    {
        for(JPSZone *corridor : current_room->getCorridorList())
        {
            if(corridor->get_id() == subroom_id.toInt())
            {
                qDebug("Found corridor. Leave JPSZone::getSubroomWithID");
                return corridor;
            }
        }

        for(JPSZone *lobby : current_room->getLobbyList())
        {
            if(lobby->get_id() == subroom_id.toInt())
            {
                qDebug("Found lobby. Leave JPSZone::getSubroomWithID");
                return lobby;
            }
        }

        for(JPSZone *office : current_room->getOfficeList())
        {
            if(office->get_id() == subroom_id.toInt())
            {
                qDebug("Found office. Leave JPSZone::getSubroomWithID");
                return office;
            }
        }

        for(JPSZone *stair : current_room->getStairList())
        {
            if(stair->get_id() == subroom_id.toInt())
            {
                qDebug("Found stair. Leave JPSZone::getSubroomWithID");
                return stair;
            }
        }

        for(JPSZone *platfrom : current_room->getPlatfromList())
        {
            if(platfrom->get_id() == subroom_id.toInt())
            {
                qDebug("Found platfrom. Leave JPSZone::getSubroomWithID");
                return platfrom;
            }
        }
    }

    qDebug("Leave JPSZone::getSubroomWithID");
    return nullptr;
}

void jpsDatamanager::removeTransition(jpsTransition *transition)
{
    qDebug("Enter jpsDatamanager::removeTransition");
    transition_list.removeOne(transition);
    qDebug("Leave jpsDatamanager::removeTransition");
}

void jpsDatamanager::removeAllSource()
{
    qDebug("Enter jpsDatamanager::removeAllSource");
    if(!sourcelist.isEmpty())
    {
        for(JPSSource *source : sourcelist)
        {
            delete source; // delete JPSSource
            source = nullptr;
        }

        sourcelist.clear();
    }
    qDebug("Leave jpsDatamanager::removeAllSource");
}

void jpsDatamanager::removeAllGoal()
{
    qDebug("Enter jpsDatamanager::removeAllGoal");
    if(!goallist.isEmpty())
    {
        for(JPSGoal *goal : goallist)
        {
            delete goal; // delete JPSGoal
            goal = nullptr;
        }

        goallist.clear();
    }
    qDebug("Leave jpsDatamanager::removeAllGoal");
}

QList<jpsTransition *> jpsDatamanager::getTransitionInSubroom(JPSZone *subroom)
{
    qDebug("Enter jpsDatamanager::getTransitionInSubroom");
    if(subroom == nullptr)
        return QList<jpsTransition *>{};

    QList<jpsTransition *> transitions;

    for(jpsTransition *transition : transition_list)
    {
        for(JPSZone *zone : transition->get_roomList())
        {
            if(zone == subroom)
                transitions.append(transition);
        }
    }

    return transitions;

    qDebug("Leave jpsDatamanager::getTransitionInSubroom");
}
