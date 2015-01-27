#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include<QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QMessageBox>
#include "rooms.h"
#include "jpscrossing.h"
#include "jpsLineItem.h"
#include "jpsexit.h"
#include "jpsobstacle.h"
#include "GraphicView.h"


#include "../dxflib/src/dl_creationadapter.h"
#include "../dxflib/src/dl_dxf.h"


class jpsDatamanager: public DL_CreationAdapter
{
public:
    jpsDatamanager(QWidget* parent=0L, jpsGraphicsView* view=0L);
    ~jpsDatamanager(){}
    QList<jpsRoom *> get_roomlist();
    void new_room();
    void remove_room(jpsRoom* room);
    void change_roomName(jpsRoom* room, QString name);
    void remove_all_rooms();
    QList<jpsObstacle *> get_obstaclelist();
    void new_obstacle();
    void remove_obstacle(jpsObstacle* obs);
    void change_obstacleName(jpsObstacle* obs, QString name);
    void remove_all_obstacles();
    QList<jpsCrossing *> get_crossingList();
    void new_crossing(QList<jpsLineItem *> newCrossing);
    void remove_crossing(jpsCrossing* crossing);
    void change_crossingName(jpsCrossing* crossing, QString name);
    void remove_all_crossings();
    QList<jpsExit *> get_exitList();
    void new_exit(QList<jpsLineItem *> newExits);
    void remove_exit(jpsExit* exit);
    void change_exitName(jpsExit* exit, QString name);
    void remove_all_exits();

    void remove_all();
    void remove_marked_lines();
    void set_view(jpsGraphicsView* view);
    jpsGraphicsView* get_view();
    // Read XML
    bool readXML(QFile &file);
    void parseSubRoom(QXmlStreamReader &xmlReader);
    void parseWalls(QXmlStreamReader &xmlReader,jpsRoom *room);
    void parseCrossings(QXmlStreamReader &xmlReader);
    void parseTransitions(QXmlStreamReader &xmlReader);

    // Write XML
    void writeXML(QFile &file);
    void writeHeader(QXmlStreamWriter *stream);
    void writeRooms(QXmlStreamWriter *stream);
    void writeCrossings(QXmlStreamWriter *stream);
    void writeTransitions(QXmlStreamWriter *stream);
    void writeObstacles(QXmlStreamWriter *stream, jpsObstacle *obs);

    // Read DXF
    bool readDXF(std::string filename);
    virtual void addLine(const DL_LineData& d);
    //write DXF
    void writeDXF(std::string filename);
    void writeDXFHeader(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFTables(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFBlocks(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFEntities(DL_Dxf* dxf, DL_WriterA *dw);
    void writeDXFObjects(DL_Dxf* dxf, DL_WriterA *dw);
    QString check_printAbility();

private:
    QList<jpsRoom *> roomlist;
    QList<jpsObstacle *> obstaclelist;
    QList<jpsCrossing *> crossingList;
    QList<jpsExit *> exitList;
    int room_id_counter;
    int obs_id_counter;
    QWidget* parent_widget;
    jpsGraphicsView* mView;



};


#endif // DATAMANAGER_H
