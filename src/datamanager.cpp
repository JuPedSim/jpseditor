#include "datamanager.h"
#include <iostream>


jpsDatamanager::jpsDatamanager(QWidget *parent)
{
    parent_widget=parent;
    room_id_counter=0;
    obs_id_counter=0;

}

void jpsDatamanager::new_room()
{
    jpsRoom* new_room = new jpsRoom(this->room_id_counter);
    roomlist.push_back(new_room);
    room_id_counter+=1;
}

void jpsDatamanager::remove_room(jpsRoom *room)
{
    if (roomlist.size()>0)
    {
        roomlist.removeOne(room);
        delete room;
    }
}

void jpsDatamanager::change_roomName(jpsRoom* room, QString name)
{
    room->change_name(name);
}

void jpsDatamanager::remove_all_rooms()
{

    for (int i=0; i<roomlist.size(); i++)
    {
        remove_room(roomlist[i]);
    }
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
        remove_obstacle(obstaclelist[i]);
    }
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
        jpsCrossing* newCrossing = new jpsCrossing(newCrossings[i]);
        crossingList.push_back(newCrossing);
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
        remove_crossing(crossingList[i]);
    }
}


QList<jpsExit *> jpsDatamanager::get_exitList()
{
    return exitList;
}

void jpsDatamanager::new_exit(QList <jpsLineItem *> newExits)
{
    for (int i=0; i<newExits.size(); i++)
    {
        jpsExit* newExit = new jpsExit(newExits[i]);
        exitList.push_back(newExit);
    }
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
        remove_exit(exitList[i]);
    }
}

void jpsDatamanager::writeXML(QFile &file,const qreal &min_x,const qreal &min_y)
{
    QXmlStreamWriter* stream = new QXmlStreamWriter(&file);


    writeHeader(stream);
    writeRooms(stream,min_x,min_y);
    writeTransitions(stream,min_x,min_y);

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

void jpsDatamanager::writeRooms(QXmlStreamWriter *stream,const qreal &min_x,const qreal &min_y)
{
    //rooms
    stream->writeStartElement("rooms");
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
            stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x1()-min_x));
            stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y1()-min_y));
            stream->writeEndElement(); //vertex

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x2()-min_x));
            stream->writeAttribute("py",QString::number(wallList[j]->get_line()->line().y2()-min_y));
            stream->writeEndElement(); //vertex

            stream->writeEndElement(); //polygon
        }



        //polygonzug
        /*
        QList<QPointF> vertices = roomlist[i]->get_vertices();

        for (int j=0; j<vertices.size(); j++)
        {

            stream->writeStartElement("vertex");
            stream->writeAttribute("px",QString::number(vertices[j].x()-min_x));
            stream->writeAttribute("py",QString::number(vertices[j].y()-min_y));
            stream->writeEndElement(); //vertex
        }*/




        for (int k=0; k<obstaclelist.size(); k++)
        {
            if (roomlist[i]==obstaclelist[k]->get_room())
            {
                writeObstacles(stream, min_x,min_y,obstaclelist[k]);
            }
        }

        stream->writeEndElement();//subroom
    }

    writeCrossings(stream,min_x,min_y);

    stream->writeEndElement();//room
    stream->writeEndElement();//rooms

}

void jpsDatamanager::writeCrossings(QXmlStreamWriter *stream,const qreal &min_x,const qreal &min_y)
{
    stream->writeStartElement("crossings");

    for (int i=0; i<crossingList.size(); i++)
    {
        stream->writeStartElement("crossing");

        stream->writeAttribute("id",QString::number(i));
        stream->writeAttribute("subroom1_id",QString::number(crossingList[i]->get_roomList()[0]->get_id()));
        stream->writeAttribute("subroom2_id",QString::number(crossingList[i]->get_roomList()[1]->get_id()));
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(crossingList[i]->get_cLine()->get_line()->line().x1()-min_x));
        stream->writeAttribute("py",QString::number(crossingList[i]->get_cLine()->get_line()->line().y1()-min_y));
        stream->writeEndElement(); //vertex
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(crossingList[i]->get_cLine()->get_line()->line().x2()-min_x));
        stream->writeAttribute("py",QString::number(crossingList[i]->get_cLine()->get_line()->line().y2()-min_y));
        stream->writeEndElement();//vertex

        stream->writeEndElement();//crossing
    }

    stream->writeEndElement();//crossings
}

void jpsDatamanager::writeTransitions(QXmlStreamWriter *stream,const qreal &min_x,const qreal &min_y)
{
    stream->writeStartElement("transitions");

    for (int i=0; i<exitList.size(); i++)
    {
        stream->writeStartElement("transition");

        stream->writeAttribute("id",QString::number(i));
        stream->writeAttribute("caption","main exit");
        stream->writeAttribute("type","emergency");
        stream->writeAttribute("room1_id","0");
        stream->writeAttribute("subroom1_id",QString::number(exitList[i]->get_roomList()[0]->get_id()));
        stream->writeAttribute("room2_id","-1");
        stream->writeAttribute("subroom2_id","-1");
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(exitList[i]->get_cLine()->get_line()->line().x1()-min_x));
        stream->writeAttribute("py",QString::number(exitList[i]->get_cLine()->get_line()->line().y1()-min_y));
        stream->writeEndElement(); //vertex
        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(exitList[i]->get_cLine()->get_line()->line().x2()-min_x));
        stream->writeAttribute("py",QString::number(exitList[i]->get_cLine()->get_line()->line().y2()-min_y));
        stream->writeEndElement();//vertex

        stream->writeEndElement();//transition
    }

    stream->writeEndElement();//transitions
}

void jpsDatamanager::writeObstacles(QXmlStreamWriter *stream, const qreal &min_x, const qreal &min_y, jpsObstacle* obs)
{
    stream->writeStartElement("obstacle");
    stream->writeAttribute("id",QString::number(obs->get_id()));
    stream->writeAttribute("caption",obs->get_name());
    stream->writeAttribute("status","closed");
    //stream->writeAttribute("height","1.0");  // height not implemented yet!

    //walls

    QList<jpsLineItem* > wallList = obs->get_listWalls();

    for (int j=0; j<wallList.size(); j++)
    {
        stream->writeStartElement("polygon");

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x1()-min_x));
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().y1()-min_y));
        stream->writeEndElement(); //vertex

        stream->writeStartElement("vertex");
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().x2()-min_x));
        stream->writeAttribute("px",QString::number(wallList[j]->get_line()->line().y2()-min_y));
        stream->writeEndElement(); //vertex

        stream->writeEndElement(); //polygon
    }

    stream->writeEndElement(); //obstacle

}

void jpsDatamanager::remove_all()
{
    remove_all_rooms();
    remove_all_crossings();
    remove_all_exits();
    remove_all_obstacles();
}





















