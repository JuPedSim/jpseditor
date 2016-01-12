/**
 * \file        datamanager.cpp
 * \date        Jun 26, 2015
 * \version     v0.7
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
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



jpsDatamanager::jpsDatamanager(QWidget *parent, jpsGraphicsView *view)
{
    parent_widget=parent;
    mView=view;
    room_id_counter=0;
    obs_id_counter=0;
    _yahPointer=nullptr;
    _frameRate=0;

}

jpsDatamanager::~jpsDatamanager()
{
    //remove_all(); vmiew->delete_all() does this work
}

void jpsDatamanager::new_room()
{
    jpsRoom* new_room = new jpsRoom(this->room_id_counter);
    roomlist.push_back(new_room);
    room_id_counter+=1;
}

void jpsDatamanager::remove_room(jpsRoom *room)
{
    //if (roomlist.size()>0)
    //{
    int roomID=room->get_id();
    roomlist.removeOne(room);
    delete room;
    //change IDs of other room with ID greater than roomID
    for (jpsRoom* otherroom:roomlist)
        if (otherroom->get_id()>roomID)
            otherroom->set_id(otherroom->get_id()-1);

    room_id_counter-=1;
    //}
}

void jpsDatamanager::change_roomName(jpsRoom* room, QString name)
{
    room->change_name(name);
}

void jpsDatamanager::remove_all_rooms()
{
    //std::cout << roomlist.first()->get_name().toStdString() << std::endl;
    for (int i=0; i<roomlist.size(); i++)
    {
        delete roomlist[i];
    }
    roomlist.clear();


}

QList<jpsRoom *> jpsDatamanager::get_roomlist()
{
    return this->roomlist;
}



void jpsDatamanager::new_obstacle()
{
    jpsObstacle* new_obs = new jpsObstacle(this->obs_id_counter);
    obstaclelist.push_back(new_obs);
    obs_id_counter+=1;
}

void jpsDatamanager::remove_obstacle(jpsObstacle *obs)
{
    if (obstaclelist.size()>0)
    {
        obstaclelist.removeOne(obs);
        delete obs;
        obs_id_counter-=1;
    }
}

void jpsDatamanager::change_obstacleName(jpsObstacle* obs, QString name)
{
    obs->change_name(name);
}

void jpsDatamanager::remove_all_obstacles()
{

    for (int i=0; i<obstaclelist.size(); i++)
    {
        delete obstaclelist[i];
    }
    obstaclelist.clear();
}

QList<jpsObstacle *> jpsDatamanager::get_obstaclelist()
{
    return obstaclelist;
}

QList<jpsCrossing *> jpsDatamanager::get_crossingList()
{
    return crossingList;
}

void jpsDatamanager::new_crossing(QList <jpsLineItem *> newCrossings)
{
    for (int i=0; i<newCrossings.size(); i++)
    {
        if (newCrossings[i]->is_Door())
        {
            jpsCrossing* newCrossing = new jpsCrossing(newCrossings[i]);
            crossingList.push_back(newCrossing);
        }

    }
}

void jpsDatamanager::new_crossing(jpsLineItem *newCrossing)
{
    if (newCrossing->is_Door())
    {
        jpsCrossing* newCros = new jpsCrossing(newCrossing);
        crossingList.push_back(newCros);
    }
}

void jpsDatamanager::remove_crossing(jpsCrossing *crossing)
{
    if (crossingList.size()>0)
    {
        crossingList.removeOne(crossing);
        delete crossing;
    }
}

void jpsDatamanager::change_crossingName(jpsCrossing *crossing, QString name)
{
    crossingList[crossingList.indexOf(crossing)]->change_name(name);
}

void jpsDatamanager::remove_all_crossings()
{
    for (int i=0; i<crossingList.size(); i++)
    {
        delete crossingList[i];
    }
    crossingList.clear();
}


QList<jpsExit *> jpsDatamanager::get_exitList()
{
    return exitList;
}

void jpsDatamanager::new_exit(QList <jpsLineItem *> newExits)
{
    for (int i=0; i<newExits.size(); i++)
    {
        //if (newExits[i]->is_Exit())
        //{
        jpsExit* newExit = new jpsExit(newExits[i]);
        exitList.push_back(newExit);
        //}
    }
}

void jpsDatamanager::new_exit(jpsLineItem *newExit)
{
    jpsExit* newEx = new jpsExit(newExit);
    exitList.push_back(newEx);
}

void jpsDatamanager::remove_exit(jpsExit *exit)
{
    if (exitList.size()>0)
    {
        exitList.removeOne(exit);
        delete exit;

    }
}

void jpsDatamanager::remove_all_exits()
{
    for (int i=0; i<exitList.size(); i++)
    {
        delete exitList[i];
    }
    exitList.clear();
}

QList<jpsLandmark *> jpsDatamanager::get_landmarks()
{
    return landmarks;
}

void jpsDatamanager::new_landmark(jpsLandmark *newlandmark)
{
    landmarks.push_back(newlandmark);
}

void jpsDatamanager::remove_landmark(jpsLandmark *landmark)
{
    landmarks.removeOne(landmark);
}

void jpsDatamanager::change_LandmarkName(jpsLandmark *landmark, QString name)
{
    landmark->set_name(name);
}

void jpsDatamanager::remove_all_landmarks()
{
    landmarks.clear();
}

void jpsDatamanager::writeXML(QFile &file)
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = mView->get_line_vector();

    writeHeader(stream);
    stream->writeStartElement("rooms");
    writeRooms(stream,lines);
    stream->writeEndElement();//rooms

    stream->writeStartElement("transitions");
    writeTransitions(stream,lines);
    exitList.clear();
    stream->writeEndElement();//transitions

    stream->writeStartElement("landmarks");
    writeLandmarks(stream,landmarks);
    stream->writeEndElement();//landmarks

    stream->writeEndElement();//geometry

    stream->writeEndDocument();

    delete stream;
}

void jpsDatamanager::AutoSaveXML(QFile &file)
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);
    QList<jpsLineItem* > lines = mView->get_line_vector();

    writeHeader(stream);
    stream->writeStartElement("rooms");
    AutoSaveRooms(stream,lines);
    stream->writeEndElement();//rooms

    stream->writeStartElement("transitions");
    writeTransitions(stream,lines);
    writeNotAssignedExits(stream,lines);
    exitList.clear();
    stream->writeEndElement();//transitions

    stream->writeStartElement("landmarks");
    writeLandmarks(stream,landmarks);
    stream->writeEndElement();//landmarks

    stream->writeEndElement();//geometry

    stream->writeEndDocument();

    delete stream;
}

void jpsDatamanager::writeHeader(QXmlStreamWriter *stream)
{
    stream->setAutoFormatting(true);
    stream->writeStartDocument("1.0",true);

    stream->writeStartElement("geometry");
    stream->writeAttribute("version", "0.5");
    stream->writeAttribute("caption","corner");
    stream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/jps_geometry.xsd");
    stream->writeAttribute("unit","m");
}

void jpsDatamanager::writeRooms(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    ///rooms
    stream->writeStartElement("room");
    stream->writeAttribute("id","0");
    stream->writeAttribute("caption","hall");

    for (int i=0; i<roomlist.size(); i++)
    {
        stream->writeStartElement("subroom");
        stream->writeAttribute("id",QString::number(roomlist[i]->get_id()));
        stream->writeAttribute("caption",roomlist[i]->get_name());
        stream->writeAttribute("class",roomlist[i]->get_type());

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

        }


        for (int k=0; k<obstaclelist.size(); k++)
        {
            if (roomlist[i]==obstaclelist[k]->get_room())
            {
                writeObstacles(stream ,obstaclelist[k],lines);
            }
        }

        stream->writeEndElement();//subroom
    }


    ///Crossings
    writeCrossings(stream,lines);

    stream->writeEndElement();//crossings

    stream->writeEndElement();//room


}

void jpsDatamanager::AutoSaveRooms(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
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
    /// Not assigned lines
    writeNotAssignedWalls(stream,lines);

    ///Crossings
    writeCrossings(stream,lines);
    writeNotAssignedDoors(stream,lines);
    stream->writeEndElement();//crossings

    stream->writeEndElement();//room

}

void jpsDatamanager::writeCrossings(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    stream->writeStartElement("crossings");


    for (int i=0; i<crossingList.size(); i++)
    {
        if (crossingList[i]->IsExit()==false)
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
            stream->writeEndElement();//vertex

            stream->writeEndElement();//crossing


        }
        else
        {
            this->new_exit(crossingList[i]->get_cLine());
            exitList.back()->add_rooms(crossingList[i]->get_roomList()[0]);
        }
        lines.removeOne(crossingList[i]->get_cLine());
    }



}

void jpsDatamanager::writeTransitions(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{


    for (int i=0; i<exitList.size(); i++)
    {
        stream->writeStartElement("transition");

        stream->writeAttribute("id",QString::number(i));
        stream->writeAttribute("caption","NaN");
        stream->writeAttribute("type","NaN");
        stream->writeAttribute("room1_id","0");
        stream->writeAttribute("subroom1_id",QString::number(exitList[i]->get_roomList()[0]->get_id()));
        stream->writeAttribute("room2_id","-1");
        stream->writeAttribute("subroom2_id","-1");
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(exitList[i]->get_cLine()->get_line()->line().x1()));
        stream->writeAttribute("py",QString::number(exitList[i]->get_cLine()->get_line()->line().y1()));
        stream->writeEndElement(); //vertex
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(exitList[i]->get_cLine()->get_line()->line().x2()));
        stream->writeAttribute("py",QString::number(exitList[i]->get_cLine()->get_line()->line().y2()));
        stream->writeEndElement();//vertex

        stream->writeEndElement();//transition
        lines.removeOne(exitList[i]->get_cLine());
    }


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


}

void jpsDatamanager::writeNotAssignedDoors(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines)
{
    for (jpsLineItem* line:lines)
    {
        if (line->is_Door())
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

void jpsDatamanager::writeLandmarks(QXmlStreamWriter *stream, QList<jpsLandmark *> &landmarks)
{
    int m=0;
    int n=0;
    for (jpsLandmark* landmark:landmarks)
    {
        stream->writeStartElement("landmark");

        stream->writeAttribute("id",QString::number(n));
        stream->writeAttribute("caption",landmark->get_name());
        stream->writeAttribute("type","NaN");
        stream->writeAttribute("room1_id","0");
        if (landmark->get_room()!=nullptr)
            stream->writeAttribute("subroom1_id",QString::number(landmark->get_room()->get_id()));
        else
            stream->writeAttribute("subroom1_id","NaN");
        stream->writeAttribute("px",QString::number(landmark->get_pos().x()));
        stream->writeAttribute("py",QString::number(landmark->get_pos().y()));
        stream->writeStartElement("associations");

        for (ptrWaypoint waypoint:landmark->GetWaypoints())
        {
            stream->writeStartElement("association");
            stream->writeAttribute("id",QString::number(m));
            stream->writeAttribute("caption","Waypoint");
            stream->writeAttribute("type","NaN");
            stream->writeAttribute("px",QString::number(waypoint->GetPos().x()));
            stream->writeAttribute("py",QString::number(waypoint->GetPos().y()));
            stream->writeAttribute("a",QString::number(waypoint->GetA()));
            stream->writeAttribute("b",QString::number(waypoint->GetB()));
            stream->writeEndElement();//association
            m++;
        }
        stream->writeEndElement();//associations
        stream->writeEndElement();//landmark
        n++;
    }

}

void jpsDatamanager::remove_all()
{
    remove_all_crossings();
    remove_all_exits();
    remove_all_obstacles();
    remove_all_rooms();
    remove_all_landmarks();
    room_id_counter=0;
    obs_id_counter=0;

}

void jpsDatamanager::remove_marked_lines()
{
    QList<jpsLineItem* > marked_lines = mView->get_markedLines();
    for (int i=0; i<marked_lines.size(); i++)
    {
        if (marked_lines[i]->is_Wall()==true)
        {
            QList<jpsRoom* > cList= this->get_roomlist();
            for (int j=0; j<cList.size(); j++)
            {   QList<jpsLineItem* > delete_candidates;
                for (int k=0; k<cList[j]->get_listWalls().size(); k++)
                {
                    if (marked_lines[i]==cList[j]->get_listWalls()[k])
                    {
                        delete_candidates.push_back(cList[j]->get_listWalls()[k]);
                    }
                }
                cList[j]->removeWall(delete_candidates);
            }
        }

        else
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

        }

    }
}

void jpsDatamanager::set_view(jpsGraphicsView *view)
{
    mView=view;
}

jpsGraphicsView * jpsDatamanager::get_view()
{
    return mView;
}

void jpsDatamanager::AutoAssignCrossings()
{
    QList<jpsCrossing* > crossings = crossingList;

    for (jpsCrossing *crossing: crossings)
    {
        int roomCounter=0;
        for (jpsRoom *room : roomlist)
        {
            if (room->ContainsDoor(crossing->get_cLine()) && roomCounter==0)
            {
                crossing->add_rooms(room);
                roomCounter++;

            }
            else if (room->ContainsDoor(crossing->get_cLine()) && roomCounter==1)
            {
                crossing->add_rooms(crossing->get_roomList()[0],room);
                crossings.removeOne(crossing);
                break;
            }
        }
    }



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



}

void jpsDatamanager::AutoAssignExits()
{
    for (jpsExit *exit: exitList)
    {
        for (jpsRoom *room: roomlist)
        {
            QList<jpsLineItem* > walls = room->get_listWalls();

            int pointCounter = 0;

            for (jpsLineItem* wall: walls)
            {
                if (wall->get_line()->line().p1()==exit->get_cLine()->get_line()->line().p1() ||
                       wall->get_line()->line().p1()==exit->get_cLine()->get_line()->line().p2() ||
                        wall->get_line()->line().p2()==exit->get_cLine()->get_line()->line().p1() ||
                        wall->get_line()->line().p2()==exit->get_cLine()->get_line()->line().p2())
                {
                    pointCounter++;
                }
            }

            if (pointCounter==2)
            {
                exit->add_rooms(room);
                break;
            }
        }
    }

}

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
        QMessageBox::critical(mView,
                              "QXSRExample::parseXML",
                              xmlReader.errorString(),
                              QMessageBox::Ok);
        return false;
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xmlReader.clear();

    ///AutoZoom to drawing
    mView->AutoZoom();

    return true;
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
            jpsLineItem* lineItem = mView->addLineItem(x1,y1,x2,y2,"Wall");

            room->addWall(lineItem);

        }

    }

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
            jpsLineItem* lineItem = mView->addLineItem(x1,y1,x2,y2,"Wall");

            room->addWall(lineItem);

        }
    }
}


void jpsDatamanager::parseCrossings(QXmlStreamReader &xmlReader)
{
    int id = xmlReader.attributes().value("id").toString().toInt();
    int room_id1 = xmlReader.attributes().value("subroom1_id").toString().toInt();
    int room_id2 = xmlReader.attributes().value("subroom2_id").toString().toInt();

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

    jpsLineItem* lineItem = mView->addLineItem(x1,y1,x2,y2,"Door");

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
        if (room1!=nullptr && room2!=nullptr)
            crossing->add_rooms(room1,room2);

        crossingList.push_back(crossing);
    }

}

void jpsDatamanager::parseTransitions(QXmlStreamReader &xmlReader)
{
    int id = xmlReader.attributes().value("id").toString().toInt();
    QString caption = xmlReader.attributes().value("caption").toString();
    QString type = xmlReader.attributes().value("type").toString();
    int room_id = xmlReader.attributes().value("subroom1_id").toString().toInt();

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

    jpsLineItem* lineItem = mView->addLineItem(x1,y1,x2,y2,"Exit");
    if (id!=-2)
    {
        jpsCrossing* exit = new jpsCrossing(lineItem);
        exit->set_id(id);
        exit->change_name(caption);
        //exit->set_type(type);

        for (int i=0; i<roomlist.size(); i++)
        {
            if (roomlist[i]->get_id()==room_id)
            {
                exit->add_rooms(roomlist[i]);
            }
        }
        exit->SetStatExit(true);
        crossingList.push_back(exit);
    }

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
        mView->AutoZoom();
        return true;
    }

}

void jpsDatamanager::addLine(const DL_LineData &d)
{
    DL_Attributes attributes = DL_CreationInterface::getAttributes();
    if (attributes.getLayer()=="Wall" || attributes.getLayer()=="Door")
        mView->addLineItem(d.x1,d.y1,d.x2,d.y2,QString::fromStdString(attributes.getLayer()));
    else
        mView->addLineItem(d.x1,d.y1,d.x2,d.y2);
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


    QList<jpsLineItem* > lines = mView->get_line_vector();

    DL_Attributes attribute("mainlayer", 256, 256, -1, "BYLAYER");

    for (int i=0; i<lines.size(); i++)
    {
        DL_LineData linedata(lines[i]->get_line()->line().x1(),
                    lines[i]->get_line()->line().y1(),
                    0.0,
                    lines[i]->get_line()->line().x2(),
                    lines[i]->get_line()->line().y2(),
                    0.0);
        dxf->writeLine(*dw,linedata,attribute);

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
    if (roomlist.size()<1)
    {
        QString string = "No rooms defined! Save XML-file not possible!";
        return string;
    }
    for (int i=0; i<crossingList.size(); i++)
    {
        if (crossingList[i]->get_roomList().size() < 2 && !crossingList[i]->IsExit())
        {
            QString string = "There are crossings which are not assigned to a room! Save XML-file not possible!";
            return string;
        }
    }
    for (int i=0; i<exitList.size(); i++)
    {
        if (exitList[i]->get_roomList().size() < 1)
        {
            QString string = "There are exits which are not assigned to a room! Save XML-file not possible!";
            return string;
        }

    }
    for (int i=0; i<obstaclelist.size(); i++)
    {
        if (obstaclelist[i]->get_room()==0L)
        {
            QString string = "There are obstacles which are not assigned to a room! Save XML-file not possible!";
            return string;
        }

    }


    return "";
}

bool jpsDatamanager::ParseCogMap(QFile &file)
{
    QXmlStreamReader xmlReader(&file);

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
            if(xmlReader.name() == "header")
            {
                continue;
            }
            if(xmlReader.name() == "frameRate")
            {
                _frameRate=xmlReader.readElementText().toFloat();
            }
            if(xmlReader.name() == "frame")
            {
                ParseFrames(xmlReader);
            }
        }
    }
    /* Error handling. */
    if(xmlReader.hasError())
    {
        QMessageBox::critical(mView,
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

void jpsDatamanager::ParseFrames(QXmlStreamReader &xmlReader)
{
      int frameID= xmlReader.attributes().value("ID").toString().toInt();

    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                xmlReader.name() == "frame"))
    {
        if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                         xmlReader.name() == "YAHPointer")
        {
            ParseYAHPointer(xmlReader, frameID);
        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                    xmlReader.name() == "landmark")
        {
            ParseLandmarksInCMap(xmlReader, frameID);
        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                    xmlReader.name() == "waypoint")
        {
            ParseWaypointInCMap(xmlReader, frameID);
        }
        else if (xmlReader.tokenType()==QXmlStreamReader::StartElement &&
                    xmlReader.name() == "connection")
        {
            ParseConnectionsInCMap(xmlReader, frameID);
        }
        xmlReader.readNext();

    }

    _lastCMapFrame=frameID;
}

void jpsDatamanager::ParseYAHPointer(QXmlStreamReader &xmlReader, const int& frame)
{
    qreal x = xmlReader.attributes().value("x").toString().toFloat();
    qreal y = xmlReader.attributes().value("y").toString().toFloat();
    qreal angle = xmlReader.attributes().value("dir").toString().toFloat();
    if (_yahPointer==nullptr)
    {
        _yahPointer = new jpsYAHPointer(x,y,angle);
        _yahPointer->SetFirstFrame(frame);
        _yahPointer->SetLastFrame(frame);
    }
    else
    {
        _yahPointer->SetPos(QPointF(x,y));
        _yahPointer->SetDirection(angle);
        _yahPointer->SetLastFrame(frame);
    }

}

void jpsDatamanager::ParseLandmarksInCMap(QXmlStreamReader &xmlReader, const int& frame)
{

    bool wayPInList=false;

    int id = xmlReader.attributes().value("ID").toString().toInt();
    qreal x = xmlReader.attributes().value("x").toString().toFloat();
    qreal y = xmlReader.attributes().value("y").toString().toFloat();
    qreal rA = xmlReader.attributes().value("rA").toString().toFloat();
    qreal rB = xmlReader.attributes().value("rB").toString().toFloat();
    QString caption = xmlReader.attributes().value("caption").toString();

    for (ptrWaypoint waypoint:_waypointsInCMap)
    {
        if (waypoint->GetId()==id && waypoint->GetType()=="Landmark")
        {
            wayPInList = true;
            waypoint->SetLastFrame(frame);
        }
    }

    if (!wayPInList)
    {
        _waypointsInCMap.push_back(std::make_shared<jpsWaypoint>(QPointF(x,y),rA,rB,id,"Landmark"));
        _waypointsInCMap.back()->SetFirstFrame(frame);
        _waypointsInCMap.back()->SetLastFrame(frame);
        _waypointsInCMap.back()->SetCaption(caption);
    }
}

void jpsDatamanager::ParseWaypointInCMap(QXmlStreamReader &xmlReader, const int& frame)
{
    bool wayPInList=false;

    int id = xmlReader.attributes().value("ID").toString().toInt();
    qreal x = xmlReader.attributes().value("x").toString().toFloat();
    qreal y = xmlReader.attributes().value("y").toString().toFloat();
    qreal rA = xmlReader.attributes().value("rA").toString().toFloat();
    qreal rB = xmlReader.attributes().value("rB").toString().toFloat();
    QString caption = xmlReader.attributes().value("caption").toString();
    bool current = xmlReader.attributes().value("current").toString().toInt();

    for (ptrWaypoint waypoint:_waypointsInCMap)
    {
        if (waypoint->GetId()==id && waypoint->GetType()=="Waypoint")
        {
            wayPInList = true;
            waypoint->SetLastFrame(frame);

            if (current!=waypoint->IsCurrent())
            {
                waypoint->ChangeCurrentness(frame);

            }

        }
    }

    if (!wayPInList)
    {
        _waypointsInCMap.push_back(std::make_shared<jpsWaypoint>(QPointF(x,y),rA,rB,id,"Waypoint"));
        _waypointsInCMap.back()->SetFirstFrame(frame);
        _waypointsInCMap.back()->SetLastFrame(frame);
        _waypointsInCMap.back()->SetCurrentness(current,frame);
        _waypointsInCMap.back()->SetCaption(caption);
    }

}

void jpsDatamanager::ParseConnectionsInCMap(QXmlStreamReader &xmlReader, const int &frame)
{
    int id1 = xmlReader.attributes().value("Landmark_WaypointID1").toString().toInt();
    int id2 = xmlReader.attributes().value("Landmark_WaypointID2").toString().toInt();
    std::shared_ptr<jpsWaypoint> waypoint1 = nullptr;
    std::shared_ptr<jpsWaypoint> waypoint2 = nullptr;


    for (ptrWaypoint waypoint:_waypointsInCMap)
    {
        if (waypoint->GetId()==id1)
        {
            waypoint1=waypoint;

        }
        else if (waypoint->GetId()==id2)
        {
            waypoint2=waypoint;

        }

    }

    ptrConnection con = std::make_shared<jpsConnection>(waypoint1,waypoint2,frame);
    if (waypoint1!=nullptr && waypoint2!=nullptr)
    {
        for (ptrConnection conInMap : _connectionsInCMap)
        {
            if (con->operator ==(conInMap))
            {
                conInMap->SetLastFrame(frame);

                return;
            }
        }

        _connectionsInCMap.push_back(con);
    }


}

void jpsDatamanager::ShowCMapFrame(const int& frame) const
{
    mView->ShowYAHPointer(_yahPointer->GetPosWhenFrame(frame),_yahPointer->GetDirWhenFrame(frame));

    QList<ptrWaypoint > wayPCandidates;

    for (ptrWaypoint waypoint:_waypointsInCMap)
    {
        if (waypoint->OccursInFrame(frame))
        {
            wayPCandidates.push_back(waypoint);

            if (waypoint->IsCurrentInFrame(frame))
                waypoint->SetText("Next target");
            else if (waypoint->Visited(frame))
                waypoint->SetText("Already visited");
            else
                waypoint->SetText("");
        }
    }
    QList<ptrConnection> conCandidates;
    for (ptrConnection connection:_connectionsInCMap)
    {
        if (connection->OccursInFrame(frame))
        {
            conCandidates.push_back(connection);
        }
    }

    mView->ShowConnections(conCandidates);
    mView->ShowWaypoints(wayPCandidates);

}


const double &jpsDatamanager::GetCMapFrameRate() const
{
    return _frameRate;
}

const int &jpsDatamanager::GetLastCMapFrame() const
{
    return _lastCMapFrame;
}





















