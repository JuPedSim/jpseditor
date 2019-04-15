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

private slots:
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

private:
    Ui::InifileWidget *ui;

    jpsDatamanager *dataManager;

    bool CheckHeaderData();
    bool CheckTrafficData();
    bool CheckRoutingData();
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

    QString WriteHeaderData();
    void WriteRoutingData(QFile &file);
    QString WriteAgentData();
    QString WriteModelGcfmData();
    QString WriteModelGompData();
    QString WriteModelTordData();
    QString WriteModelGradData();
    QString WriteModelKrauData();
    QString WriteRouteChoiceData();

    void ReadJuPedSimData(TiXmlElement* JuPedSim);
    void ReadHeaderData(TiXmlElement* JuPedSim);
    void ReadAgentData(TiXmlElement* JuPedSim);
    void ReadModelData(TiXmlElement* JuPedSim);
    void ReadRouteChoiceData(TiXmlElement* JuPedSim);
};

#endif // INIFILEWIDGET_H
