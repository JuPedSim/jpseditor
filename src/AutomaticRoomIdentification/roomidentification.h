#ifndef ROOMIDENTIFICATION_H
#define ROOMIDENTIFICATION_H

#include "src/jpszone.h"

struct AverageRoomData
{
    AverageRoomData():_meanArea(0.0),_stdArea(0.0),
        _meanRatioBBoxArea(0.0),_stdRatioBBoxArea(0.0),
        _meanRatioBBox(0.0),_stdRatioBBox(0.0),_meanNumDoors(0.0),_stdNumDoors(0.0){}
    double _meanArea;
    double _stdArea;
    double _meanRatioBBoxArea;
    double _stdRatioBBoxArea;
    double _meanRatioBBox;
    double _stdRatioBBox;
    double _meanNumDoors;
    double _stdNumDoors;

    std::vector<double> _areas;
    std::vector<double> _ratiosBBoxArea;
    std::vector<double> _ratiosBBox;
    std::vector<double> _numsDoors;
};



class RoomIdentification
{
public:
    explicit RoomIdentification();
    explicit RoomIdentification(const QList<JPSZone *>& rooms);

    void IdentifyRooms();
    void GatherData();
    void LoadDataFile();
    void CalcMeansAndStds();
    double CalcMean(const std::vector<double>& v);
    double CalcStd(const std::vector<double>& v, double mean);

private:
    QList<JPSZone *> _rooms;
    AverageRoomData _commonRoom;
    AverageRoomData _circRoom;


};

#endif // ROOMIDENTIFICATION_H
