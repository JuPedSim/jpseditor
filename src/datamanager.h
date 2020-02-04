/**
 * \file        datamanager.h
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


#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QtGui>
#include <QVariant>
#include <QApplication>
#include <QGraphicsView>
#include <QMessageBox>
#include <random>
#include <QtXml>

#include "jpszone.h"
#include "jpscrossing.h"
#include "jpsLineItem.h"
#include "jpstransition.h"
#include "jpsobstacle.h"
#include "GraphicView.h"
#include "jpsconnection.h"
#include "jpsregion.h"
#include "jpssource.h"
#include "src/global.h"

#include "../dxflib/src/dl_creationadapter.h"
#include "../dxflib/src/dl_dxf.h"

using ptrConnection = std::shared_ptr<jpsConnection>;

class jpsDatamanager: public DL_CreationAdapter
{
public:
    explicit jpsDatamanager(QWidget* parent=nullptr, jpsGraphicsView* view=nullptr);
    ~jpsDatamanager() override;

    ///Zone
    QList<JPSZone *> get_roomlist();
    void addRoom();
    bool isRepeatedRoomName(QString name);
    void remove_room(JPSZone* room);
    void change_roomName(JPSZone* room, QString name);
    void remove_all_rooms();
    JPSZone *getSubroomWithID(QString room_id, QString subroom_id);

    ///Obstacle
    QList<jpsObstacle *> get_obstaclelist();
    void new_obstacle();
    void remove_obstacle(jpsObstacle* obs);
    void change_obstacleName(jpsObstacle* obs, QString name);
    void remove_all_obstacles();

    ///Transition
    QList<jpsTransition *> getTransitionList();
    void new_exit(QList<jpsLineItem *> newExits);
    void newTransition(jpsLineItem *transition);
    void removeAllTransition();
    QList<jpsTransition *> getTransitionInSubroom(JPSZone* subroom);
    void recognizeRoomForTransition(jpsTransition *transition);

    ///Landmark
    QList<jpsLandmark *> get_landmarks();
    void new_landmark(jpsLandmark * newlandmark);
    void remove_landmark(jpsLandmark* landmark);
    void change_LandmarkName(jpsLandmark* landmark, QString name);
    void remove_all_landmarks();
    const int& GetLandmarkCounter() const;

    ///Connection
    const QList<jpsConnection*>& GetAllConnections() const;
    void NewConnection(jpsConnection* newConnection);
    void RemoveConnection(jpsConnection* connection);
    void RemoveAllConnections();

    ///Regions
    const QList<jpsRegion*>& GetRegions() const;
    void NewRegion(jpsRegion* region);
    void RemoveRegion(jpsRegion* region);
    void RemoveAllRegions();
    const int& GetRegionCounter() const;

    ///Sources
    void writeSourceXML(QFile &file);
    void writeSourceHeader(QXmlStreamWriter *stream);
    void writeSources(QXmlStreamWriter *stream, QList<JPSSource *>& sourcelist);
    const QList<JPSSource *> &getSourcelist();
    void removeAllSource();

    ///Goals
    void writeGoalXML(QFile &file);
    void writeGoals(QXmlStreamWriter *stream, QList<JPSGoal *>& goallist);
    const QList<JPSGoal *> &getGoallist();
    void removeAllGoal();

    ///Traffic
    void writeTrafficXML(QFile &file);
    void writeTraffics(QXmlStreamWriter *stream, QList<jpsCrossing *> const &doorlist);
    bool readTrafficXML(QFile &file);
    void readDoor(QXmlStreamReader &xmlReader);

    void remove_all();
    void remove_marked_lines();
    void set_view(jpsGraphicsView* view);
    jpsGraphicsView* get_view();

    // Read geometry XML
    bool readXML(QFile &file);
    void parseGeometry(const QDomElement &element);
    void parseRooms(const QDomElement &element);
    void parseTransitions(const QDomElement &element);
    void parseUndefine(const QDomElement &element);
    void parseRoom(const QDomElement &element);
    void parseSubRoom(const QDomElement &element);
    void parseCrossings(const QDomElement &element);

    void parseObstacles(QXmlStreamReader &xmlReader, JPSZone *room);

    // Read routing XML
    bool readRoutingXML(QFile &file);
    void parseHline(QXmlStreamReader &xmlReader);

    // Write XML
    void writeXML(QFile &file);

    void writeLineItems(QFile & file);
    void writeHeader(QXmlStreamWriter *stream);
    void writeRoutingHeader(QXmlStreamWriter *stream);
    void writeHLines(QXmlStreamWriter *stream, QList<jpsLineItem* >& hLines);
    QString RoomIDHLine(jpsLineItem* lineItem);
    void writeRooms(QXmlStreamWriter *stream, QList<jpsLineItem* >& lines);
    void writeSubRoom(QXmlStreamWriter *stream, JPSZone* room, QList<jpsLineItem* >& lines);
    void writeCrossings(QXmlStreamWriter *stream, JPSZone *room, QList<jpsLineItem *> &lines);
    void writeTransitions(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines);
    void writeObstacles(QXmlStreamWriter *stream, jpsObstacle *obs, QList<jpsLineItem *> &lines);
    void writeNotAssignedLines(QXmlStreamWriter *stream, QList<jpsLineItem *> &lines);
    void writeTransitionXML(QFile &file);

    // Write routing XML
    void writeRoutingXML(QFile &file);

    // Write Cognitive Map XML
    void WriteCognitiveMapXML(QFile &file, bool fuzzy=false);
    void WriteCognitiveMapXML(QFile &file, int k, double m, double p0);
    void WriteCognitiveMapHeader(QXmlStreamWriter *stream);
    void WriteRegions(QXmlStreamWriter *stream, bool fuzzy=false);
    void WriteRegions(QXmlStreamWriter *stream, int k, double m, double p0);
    void WriteLandmarks(jpsRegion *cRegion, QXmlStreamWriter *stream, bool fuzzy=false);
    void WriteLandmarks(jpsRegion *cRegion, QXmlStreamWriter *stream, int k, double m, double p0);
    void CutOutLandmarks();
    void CutOutLandmarks(int k, double m, double p0);
    double GetProbability(int k, double m, double p0);
    void BridgeLostLandmark(jpsLandmark* landmark);
    void WriteConnections(jpsRegion *cRegion, QXmlStreamWriter *stream);
    void CreateAndSaveASimilarCogMap(const int &id);
    void CreateAndSaveASimilarCogMap(int id, int k, double m, double p0);
    qreal MakeItFuzzy(const qreal &mean, const qreal& std);
    int GetNumberOfMainTargets() const;

    // Read Cognitive Map
    bool ParseCogMap(QFile &file);
    jpsRegion* ParseRegion(QXmlStreamReader &xmlReader);
    void ParseLandmark(jpsRegion* actRegion, QXmlStreamReader &xmlReader);
    void ParseConnection(jpsRegion* actRegion, QXmlStreamReader &xmlReader);

    // Read DXF
    bool readDXF(std::string filename);
    void addLine(const DL_LineData& d) override;
    QStringList unimported_layer;

    // write DXF
    void writeDXF(std::string filename);
    void writeDXFHeader(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFTables(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFBlocks(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFEntities(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFObjects(DL_Dxf* dxf, DL_WriterA *dw);

    // read line file
    bool ReadLineFile(QFile &file);

    // convert string to ZoneTyep
    ZoneType convertToZoneType(const QString string);

    // getter for JPSZones;
    const QList<JPSZone *> &getRoomlist() const;

    // add zones
    void addPlatform(JPSZone *father_room);
    void addCorridor(JPSZone *father_room);
    void addLobby(JPSZone *father_room);
    void addOffice(JPSZone *father_room);
    void addStair(JPSZone *father_room);

    // delete zones
    void removeRoom(JPSZone *room);
    void removeZone(JPSZone *room, JPSZone *zone);

    // Deprecated! datamanager doesn't control crossing anymore
    QList<jpsCrossing *> get_crossingList();
    void new_crossing(QList<jpsLineItem *> newCrossing);
    void new_crossing(jpsLineItem* newCrossing);
    void remove_crossing(jpsCrossing* crossing);
    void change_crossingName(jpsCrossing* crossing, QString name);

    // Transition
    void removeTransition(jpsTransition *transition);

/*
    //Show Cognitive Map
    void ShowCMapFrame(const int& frame) const;
    const double& GetCMapFrameRate() const;
    const int& GetLastCMapFrame() const;
*/

private:
    //Geometry
    QList<JPSZone *> roomlist; // zontType is room

    int zone_id; // For identify zone

    int transition_id; // for identiy transition

    QList<jpsObstacle *> obstaclelist;

    QList<JPSSource *> sourcelist;
    QList<JPSGoal *> goallist;
    QList<jpsTransition *> transition_list;

    QList<jpsLandmark* > _landmarks;
    QList<jpsConnection* > _landmarkConnections;
    QList<jpsLandmark* > _landmarksAfterLoose;
    QList<jpsConnection* > _ConnectionsAfterLandmarkLoose;
    QList<jpsRegion* > _regions;

    QList<JPSTrack *> track_list; // fow saving tracks

    int obs_id_counter;
    int _crossingIdCounter;
    QWidget* parent_widget;
    jpsGraphicsView* _mView;

    //CognitiveMap
    //QList<jpsLandmark* > _landmarksInCMap;
    //QList<ptrConnection> _connectionsInCMap;
    //double _frameRate;
    //int _lastCMapFrame;
    int _landmarkCounter;
    int _regionCounter;

    QString _currentCogMapFileName;

    std::default_random_engine _generator;

    bool isInCrossingList(jpsLineItem *markedLine);

    // For DXF import
    QList<QPointF> points; // Points in rects of sources or goals

    // Deprecated
    QList<jpsCrossing *> crossingList;

};

//check if two lines are equal with certain tolerance; heading direction of line does not matter
bool LineIsEqual(const QLineF& line1, const QLineF& line2, double eps=0.1);

#endif // DATAMANAGER_H
