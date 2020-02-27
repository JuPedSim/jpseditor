#include "roomidentification.h"
#include "../jpscrossing.h"
#include <QGraphicsLineItem>
#include <fstream>
#include <QFileDialog>
#include <QTextStream>
#include <numeric>
#include <iostream>
#include <cmath>


RoomIdentification::RoomIdentification()
{

}

RoomIdentification::RoomIdentification(const QList<JPSZone *> &rooms):_rooms(rooms)
{

}

void RoomIdentification::IdentifyRooms()
{

    for (JPSZone * room:_rooms)
    {
        if (room->GetArea()==0.0)
            room->IdentifyInnerOuter();
        double area =  room->GetArea();
        QRectF bBox =  room->CalculateBoundingBox();
        double ratioBBoxArea= (bBox.width()*bBox.height())/area;
        double ratioBBox=0.0;
        if (bBox.width()<bBox.height())
            ratioBBox=bBox.width()/bBox.height();
        else
            ratioBBox=bBox.height()/bBox.width();

        int numberDoors = room->getCrossingList().size();


        double penalty_common=0.0;

        double diffArea=std::fabs(area-_commonRoom._meanArea);
        penalty_common+=diffArea/_commonRoom._stdArea;
        double diffRatioBBoxArea=std::fabs(ratioBBoxArea-_commonRoom._meanRatioBBoxArea);
        penalty_common+=diffRatioBBoxArea/_commonRoom._stdRatioBBoxArea;
        double diffRatioBBox=std::fabs(ratioBBox-_commonRoom._meanRatioBBox);
        penalty_common+=diffRatioBBox/_commonRoom._stdRatioBBox;
        double diffNumDoors=std::fabs(numberDoors-_commonRoom._meanNumDoors);
        penalty_common+=diffNumDoors/_commonRoom._stdNumDoors;

        double penalty_circ=0.0;

        diffArea=std::fabs(area-_circRoom._meanArea);
        penalty_circ+=diffArea/_circRoom._stdArea;
        diffRatioBBoxArea=std::fabs(ratioBBoxArea-_circRoom._meanRatioBBoxArea);
        penalty_circ+=diffRatioBBoxArea/_circRoom._stdRatioBBoxArea;
        diffRatioBBox=std::fabs(ratioBBox-_circRoom._meanRatioBBox);
        penalty_circ+=diffRatioBBox/_circRoom._stdRatioBBox;
        diffNumDoors=std::fabs(numberDoors-_circRoom._meanNumDoors);
        penalty_circ+=diffNumDoors/_circRoom._stdNumDoors;

        // add 2.3 to penalty_circ to increase accuracy of correct declaration. The value has been found by trial
        if (2.3+penalty_circ>penalty_common)
        {
            //std::cout << "Office" << std::endl;
            //std::cout << "Circ " << penalty_circ << std::endl;
            //std::cout << "Common " << penalty_common << std::endl;
        }
        else
        {
            //std::cout << "Corridor" << std::endl;


        }

    }
}

void RoomIdentification::GatherData()
{
    for (JPSZone* room:_rooms)
    {
        room->IdentifyInnerOuter();
        double area= room->GetArea();
        QRectF bBox = room->CalculateBoundingBox();
        int numberDoors = room->getCrossingList().size();

        double ratioBBox=0.0;
        if (bBox.width()<bBox.height())
            ratioBBox=bBox.width()/bBox.height();
        else
            ratioBBox=bBox.height()/bBox.width();

        int numberGreatDoors = 0; // number of doors greater than 1.25 meters
        for (jpsCrossing* door : room->getCrossingList())
        {
            if (door->get_cLine()->get_line()->line().length()>1.25)
            {
                ++numberGreatDoors;
            }
        }
        // type; area; ratio bounding box- area; ratio bounding box

        QString type = QString(room->getType());


        std::ofstream data;
        std::string filename = "roomtypedata.txt";
        data.open(filename , std::ofstream::out | std::ofstream::app);
        data << type.toStdString() << ";" << std::to_string(area) << ";"
                   << std::to_string((bBox.width()*bBox.height())/area) << ";" << std::to_string(ratioBBox) << ";"
                   << std::to_string(numberDoors) << std::endl;
        data.close();


    }
}

void RoomIdentification::LoadDataFile()
{

    QString fileName="roomtypedata.txt";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);


    while ( !in.atEnd() )
    {
        QString Qline = in.readLine();
        QStringList strList=Qline.split(";");
        if (strList[0]=="Corridor" || strList[0]=="Lobby")
        {
            _circRoom._areas.push_back(strList[1].toDouble());
            _circRoom._ratiosBBoxArea.push_back(strList[2].toDouble());
            _circRoom._ratiosBBox.push_back(strList[3].toDouble());
            _circRoom._numsDoors.push_back(strList[4].toDouble());

        }
        else if (strList[0]=="Office")
        {
            _commonRoom._areas.push_back(strList[1].toDouble());
            _commonRoom._ratiosBBoxArea.push_back(strList[2].toDouble());
            _commonRoom._ratiosBBox.push_back(strList[3].toDouble());
            _commonRoom._numsDoors.push_back(strList[4].toDouble());

        }
    }

    file.close();



}

void RoomIdentification::CalcMeansAndStds()
{
    _commonRoom._meanArea=CalcMean(_commonRoom._areas);
    _commonRoom._stdArea=CalcStd(_commonRoom._areas,_commonRoom._meanArea);
    _commonRoom._meanRatioBBoxArea=CalcMean(_commonRoom._ratiosBBoxArea);
    _commonRoom._stdRatioBBoxArea=CalcStd(_commonRoom._ratiosBBoxArea,_commonRoom._meanRatioBBoxArea);
    _commonRoom._meanRatioBBox=CalcMean(_commonRoom._ratiosBBox);
    _commonRoom._stdRatioBBox=CalcStd(_commonRoom._ratiosBBox,_commonRoom._meanRatioBBox);
    _commonRoom._meanNumDoors=CalcMean(_commonRoom._numsDoors);
    _commonRoom._stdNumDoors=CalcStd(_commonRoom._numsDoors,_commonRoom._meanNumDoors);

    _circRoom._meanArea=CalcMean(_circRoom._areas);
    _circRoom._stdArea=CalcStd(_circRoom._areas,_circRoom._meanArea);
    _circRoom._meanRatioBBoxArea=CalcMean(_circRoom._ratiosBBoxArea);
    _circRoom._stdRatioBBoxArea=CalcStd(_circRoom._ratiosBBoxArea,_circRoom._meanRatioBBoxArea);
    _circRoom._meanRatioBBox=CalcMean(_circRoom._ratiosBBox);
    _circRoom._stdRatioBBox=CalcStd(_circRoom._ratiosBBox,_circRoom._meanRatioBBox);
    _circRoom._meanNumDoors=CalcMean(_circRoom._numsDoors);
    _circRoom._stdNumDoors=CalcStd(_circRoom._numsDoors,_circRoom._meanNumDoors);

}

double RoomIdentification::CalcMean(const std::vector<double> &v)
{
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();

    return mean;
}

double RoomIdentification::CalcStd(const std::vector<double> &v, double mean)
{
    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size() - mean * mean);

    return stdev;
}
