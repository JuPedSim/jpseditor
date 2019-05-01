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

    //Inside Tab Groups
    void on_spinBox_groups_1_valueChanged(int);
    void on_comboBox_groups_1_currentIndexChanged(int index);

    //Inside Tab Agents
    void on_spinBox_agents_gcfm_1_valueChanged(int);
    void on_spinBox_agents_gompertz_1_valueChanged(int);
    void on_spinBox_agents_tordeux_1_valueChanged(int);
    void on_spinBox_agents_gradnav_1_valueChanged(int);
    void on_spinBox_agents_krausz_1_valueChanged(int);

    //External file
    void pushButton_GeomeryClicked();
    void pushButton_GoalClicked();
    void pushButton_SourceClicked();
    void pushButton_TrafficClicked();
    void pushButton_RoutingClicked();

    //Read inifile
    bool readInifile(QXmlStreamReader *reader);
    QString errorString(QXmlStreamReader *reader) const;
    void readJuPedSim(QXmlStreamReader *reader);
    void readSeed(QXmlStreamReader *reader);
    void readGeometry(QXmlStreamReader *reader);
    void readMaxSimTime(QXmlStreamReader *reader);
    void readTrajectories(QXmlStreamReader *reader);
    void readTrajectoriesFile(QXmlStreamReader *reader);
    void readLogfile(QXmlStreamReader *reader);
    void readTrafficConstraints(QXmlStreamReader *reader);
    void readTrafficFile(QXmlStreamReader *reader);
    void readRouting(QXmlStreamReader *reader);
    void readRoutingFile(QXmlStreamReader *reader);
    void readAgents(QXmlStreamReader *reader);
    void readAgentsDistribution(QXmlStreamReader *reader);
    void readAgentsSources(QXmlStreamReader *reader);
    void readGroup(QXmlStreamReader *reader);
    void readSourceFile(QXmlStreamReader *reader);

private:
    Ui::InifileWidget *ui;

    jpsDatamanager *dataManager;

    bool CheckHeaderData();
    bool CheckTrafficData();
    bool CheckRoutingData();
    bool CheckSourceData();
    bool CheckAgentData();
    bool CheckModelGcfmData();
    bool CheckAgentGcfmData();
    bool CheckModelGompData();
    bool CheckAgentGompData();
    bool CheckModelTordData();
    bool CheckAgentTordData();
    bool CheckModelGradData();
    bool CheckAgentGradData();
    bool CheckModelKrauData();
    bool CheckAgentKrauData();
    bool CheckRouteChoiceData();

    void writeHeaderData(QXmlStreamWriter *stream, QFile &file);
    void writeRoutingData(QXmlStreamWriter *stream, QFile &file);
    void writeFFGlobalShortestModel(QXmlStreamWriter *stream, QFile &file);
    void writeGlobalShortestModel(QXmlStreamWriter *stream, QFile &file);
    void writeCognitiveMap(QXmlStreamWriter *stream, QFile &file);
    void writeTrafficData(QXmlStreamWriter *stream, QFile &file);
    void writeAgentData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGcfmData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGompData(QXmlStreamWriter *stream, QFile &file);
    void writeModelTordData(QXmlStreamWriter *stream, QFile &file);
    void writeModelGradData(QXmlStreamWriter *stream, QFile &file);
    void writeModelKrauData(QXmlStreamWriter *stream, QFile &file);
    void writeRouteChoiceData(QXmlStreamWriter *stream, QFile &file);

    void ReadJuPedSimData(TiXmlElement* JuPedSim);
    void ReadHeaderData(TiXmlElement* JuPedSim);
    void ReadAgentData(TiXmlElement* JuPedSim);
    void ReadModelData(TiXmlElement* JuPedSim);
    void ReadRouteChoiceData(TiXmlElement* JuPedSim);

signals:
    void inifileLoaded(QString, int);
};

#endif // INIFILEWIDGET_H
