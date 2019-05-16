#ifndef INIFILEWIDGET_H
#define INIFILEWIDGET_H

#include <QWidget>
#include "src/tinyxml/tinyxml.h"
#include "src/datamanager.h"

namespace Ui {
class InifileWidget;
}

class InifileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InifileWidget(QWidget *parent = nullptr, jpsDatamanager* dmanager = nullptr);
    ~InifileWidget();

public slots:
    //Inside Inifilewidget
    void on_pushButton_write_clicked();
    void on_pushButton_read_clicked();

    //Inside Tab General

    void on_comboBox_groups_1_currentIndexChanged(int index);

    //External file
    void pushButton_GeomeryClicked();
    void pushButton_GoalClicked();
    void pushButton_SourceClicked();
    void pushButton_TrafficClicked();
    void pushButton_RoutingClicked();
    void pushButton_addGroupRowClicked();
    void pushButton_deleteGrouprowClicked();
    void pushButton_addAgentRowClicked();
    void pushButton_deleteAgentRowClicked();

    //Read inifile
    bool readInifile(QXmlStreamReader *reader);
    QString errorString(QXmlStreamReader *reader) const;
    void readJuPedSim(QXmlStreamReader *reader);
    void readSeed(QXmlStreamReader *reader);
    void readThread(QXmlStreamReader *reader);
    void readGeometry(QXmlStreamReader *reader);
    void readMaxSimTime(QXmlStreamReader *reader);
    void readTrajectories(QXmlStreamReader *reader);
    void readTrajectoriesFile(QXmlStreamReader *reader);
    void readLogfile(QXmlStreamReader *reader);
    void readFire(QXmlStreamReader *reader);
    void readSmoke(QXmlStreamReader *reader);
    void readWalking(QXmlStreamReader *reader);
    void readToxicity(QXmlStreamReader *reader);
    void readTrafficConstraints(QXmlStreamReader *reader);
    void readTrafficFile(QXmlStreamReader *reader);
    void readRouting(QXmlStreamReader *reader);
    void readRoutingFile(QXmlStreamReader *reader);
    void readAgents(QXmlStreamReader *reader);
    void readAgentsDistribution(QXmlStreamReader *reader);
    void readAgentsSources(QXmlStreamReader *reader);
    void readGroup(QXmlStreamReader *reader);
    void readSourceFile(QXmlStreamReader *reader);
    void readModels(QXmlStreamReader *reader);
    void readModel(QXmlStreamReader *reader);
    void readModelParameters(QXmlStreamReader *reader, int modelindex);
    void readSlover(QXmlStreamReader *reader, int modelindex);
    void readStepsize(QXmlStreamReader *reader, int modelindex);
    void readExit(QXmlStreamReader *reader, int modelindex);
    void readLinkedcells(QXmlStreamReader *reader, int modelindex);
    void readPeriodic(QXmlStreamReader *reader, int modelindex);
    void readForcePed(QXmlStreamReader *reader, int modelindex);
    void readForceWall(QXmlStreamReader *reader, int modelindex);
    void readAntiClipping(QXmlStreamReader *reader);
    void readFloorfield(QXmlStreamReader *reader);
    void readAgentParamaters(QXmlStreamReader *reader);
    void readV0(QXmlStreamReader *reader);
    void readV0Upstairs(QXmlStreamReader *reader);
    void readV0Downstars(QXmlStreamReader *reader);
    void readV0IdleUpstairs(QXmlStreamReader *reader);
    void readV0Idledownstairs(QXmlStreamReader *reader);
    void readBmax(QXmlStreamReader *reader);
    void readBmin(QXmlStreamReader *reader);
    void readAmin(QXmlStreamReader *reader);
    void readTau(QXmlStreamReader *reader);
    void readAtou(QXmlStreamReader *reader);
    void readT(QXmlStreamReader *reader);
    void readSway(QXmlStreamReader *reader);

    void readRouteChoiceModels(QXmlStreamReader *reader);
    void readRouter(QXmlStreamReader *reader);
    void readRouterParameters(QXmlStreamReader *reader);
    void readNavigationLines(QXmlStreamReader *reader);
    void readVTK(QXmlStreamReader *reader);


private:
    Ui::InifileWidget *ui;

    jpsDatamanager *dataManager;

    void writeHeaderData(QXmlStreamWriter *stream, QFile &file);
    void writeRoutingData(QXmlStreamWriter *stream, QFile &file);
    void writeFFGlobalShortestRoute(QXmlStreamWriter *stream, QFile &file);
    void writeGlobalShortestRoute(QXmlStreamWriter *stream, QFile &file);
    void writeCognitiveMap(QXmlStreamWriter *stream, QFile &file);
    void writeTripsRoute(QXmlStreamWriter *stream, QFile &file);
    void writeTrafficData(QXmlStreamWriter *stream, QFile &file);
    void writeAgentData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGcfmData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGompData(QXmlStreamWriter *stream, QFile &file);
    void writeModelTordData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGradData(QXmlStreamWriter *stream, QFile &file);
    void writeModelKrauData(QXmlStreamWriter *stream, QFile &file);
    void writeAgentParameters(QXmlStreamWriter *stream, QFile &file);
    void writeRouteChoiceData(QXmlStreamWriter *stream, QFile &file);
    void writeFire(QXmlStreamWriter *stream, QFile &file);

signals:
    void inifileLoaded(QString, int);
};

#endif // INIFILEWIDGET_H
