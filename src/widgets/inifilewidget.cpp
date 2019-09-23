/***************************************************************
 *
 * \file inifilewidget.cpp
 * \date 2019-05-02
 * \version v0.8.9
 * \author Tao Zhong
 * \copyright <2009-2019> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section Lincense
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
 * This class define a widget for inifile
 *
****************************************************************/

#include <QDebug>
#include <QFile>
#include <QtWidgets>

#include "inifilewidget.h"
#include "ui_inifilewidget.h"

#include "src/tinyxml/tinyxml.h"

InifileWidget::InifileWidget(QWidget *parent, jpsDatamanager *dmanager) :
    QWidget(parent),
    ui(new Ui::InifileWidget)
{
    ui->setupUi(this);

    // set InifileWidget as a Window
    setWindowFlag(Qt::Dialog);

    dataManager = dmanager;

    // Set model gompertz invisivle
    ui->tabWidget->removeTab(3);

    // Set model tordeux invisivle
    ui->tabWidget->removeTab(3);

    // Set model gradnav invisivle
    ui->tabWidget->removeTab(3);

    // Set model krausz invisivle
    ui->tabWidget->removeTab(3);

    //signals and slots connection
    ui->lineEdit_general_07->setReadOnly(true);
    ui->lineEdit_GoalFile->setReadOnly(true);
    ui->lineEdit_SourceFile->setReadOnly(true);
    ui->lineEdit_TrafficFile->setReadOnly(true);

    connect(this, SIGNAL(inifileLoaded(QString, int)), parent, SLOT(showStatusBarMessage(QString, int)));

    connect(ui->pushButton_chooseTemplate, SIGNAL(clicked(bool)), this, SLOT(pushButton_chooseTemplateClicked()));

    connect(ui->pushButton_Geometry, SIGNAL(clicked(bool)), this, SLOT(pushButton_GeomeryClicked()));
    connect(ui->pushButton_Goal, SIGNAL(clicked(bool)), this, SLOT(pushButton_GoalClicked()));
    connect(ui->pushButton_Source, SIGNAL(clicked(bool)), this, SLOT(pushButton_SourceClicked()));
    connect(ui->pushButton_Traffic, SIGNAL(clicked(bool)), this, SLOT(pushButton_TrafficClicked()));
    connect(ui->pushButton_Routing, SIGNAL(clicked(bool)), this, SLOT(pushButton_RoutingClicked()));

    connect(ui->pushButton_addGroupRow, SIGNAL(clicked(bool)), this, SLOT(pushButton_addGroupRowClicked()));
    connect(ui->pushButton_deleteGroupRow, SIGNAL(clicked(bool)), this, SLOT(pushButton_deleteGrouprowClicked()));
    connect(ui->pushButton_addAgentRow, SIGNAL(clicked(bool)), this, SLOT(pushButton_addAgentRowClicked()));
    connect(ui->pushButton_deleteAgentRow, SIGNAL(clicked(bool)), this, SLOT(pushButton_deleteAgentRowClicked()));

    // template widget
    templateWidget = nullptr;

}
InifileWidget::~InifileWidget()
{
    delete ui;
}

// Set models and agents visible or invisible
void InifileWidget::on_comboBox_groups_1_currentIndexChanged(int index)
{
    // Set gcfm visible
    if (index+1 == 1)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gcfm, "Model Gcfm");
    }

    // Set gompertz visible
    if (index+1 == 2)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gompertz, "Model Gompertz");
    }

    // Set tordeux visible
    if (index+1 == 3)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_tordeux, "Model Tordeux");
    }

    // Set gradnav visible
    if (index+1 == 4)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gradnav, "Model Gradnav");
    }

    // Set krausz visible
    if (index+1 == 5)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_krausz, "Model Krausz");
    }
}


// Create ini.xml on button push
void InifileWidget::on_pushButton_write_clicked()
{
    //save to file
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Create ini"),
                                                     "",
                                                     tr("XML-Files (*.xml)"));
    QFile file(file_name);


    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        auto *stream = new QXmlStreamWriter(&file);
        stream->setAutoFormatting(true);

        stream->writeStartDocument("1.0");
        stream->setCodec("UTF-8");

        stream->writeStartElement("JuPedSim");
        stream->writeAttribute("project","JPS-Project");
        stream->writeAttribute("version", "0.8");
        stream->writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        stream->writeAttribute("xsi:noNamespaceSchemaLocation","http://xsd.jupedsim.org/0.6/jps_ini_core.xsd");

        writeHeaderData(stream, file);

        writeFire(stream, file);

        writeTrafficData(stream, file);

        writeRoutingData(stream, file);

        writeAgentData(stream, file);

        int model = ui->comboBox_groups_1->currentIndex();

        switch(model)
        {
            case 0:
                writeModelGcfmData(stream, file);
                break;
            case 1:
                writeModelGompData(stream, file);
                break;
            case 2:
                writeModelTordData(stream, file);
                break;
            case 3:
                writeModelGradData(stream, file);
                break;
            case 4:
                writeModelKrauData(stream, file);
                break;
            default:
                break;
        }

        writeRouteChoiceData(stream, file);

        stream->writeEndElement(); //end JuPedSim
        stream->writeEndDocument();
        delete stream;
        stream = nullptr;
    }
    file.close();
}

void InifileWidget::pushButton_addGroupRowClicked()
{
    int row = ui->tableWidget_groups_1->rowCount();
    ui->tableWidget_groups_1->setRowCount(row+1);
}

void InifileWidget::pushButton_deleteGrouprowClicked()
{
    int current_row = ui->tableWidget_groups_1->currentRow();
    ui->tableWidget_groups_1->removeRow(current_row);
}

void InifileWidget::pushButton_addAgentRowClicked()
{
    int row = ui->tableWidget_agents->rowCount();
    ui->tableWidget_agents->setRowCount(row+1);
}

void InifileWidget::pushButton_deleteAgentRowClicked()
{
    int current_row = ui->tableWidget_agents->currentRow();
    ui->tableWidget_agents->removeRow(current_row);
}

/*
    <!-- seed used for initialising random generator -->
    <seed>12542</seed>

    <!-- geometry file -->
    <geometry>geometry.xml</geometry>

    <max_sim_time>50</max_sim_time>

    <!-- trajectories file and format -->
    <trajectories format="xml-plain" fps="16" color_mode="group">
        <file location="trajectories.xml" />
        <!-- <socket_ hostname="127.0.0.1" port="8989" /> -->
    </trajectories>

    <!-- where to store the logs -->
    <logfile>log.txt</logfile>
*/

void InifileWidget::writeHeaderData(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeComment("number of cores used");
    stream->writeStartElement("num_threads");
    if(!ui->lineEdit_general_03->text().isEmpty())
        stream->writeCharacters(ui->lineEdit_general_03->text());
    stream->writeEndElement();

    stream->writeComment("seed used for initialising random generator");
    stream->writeStartElement("seed");
    if(!ui->lineEdit_general_05->text().isEmpty())
        stream->writeCharacters(ui->lineEdit_general_05->text());
    stream->writeEndElement();

    stream->writeComment("geometry file");
    stream->writeStartElement("geometry");
    if(!ui->lineEdit_general_07->text().isEmpty())
        stream->writeCharacters(ui->lineEdit_general_07->text().split("/").last());
    stream->writeEndElement();

    stream->writeComment("simulationtime");
    stream->writeStartElement("max_sim_time");
    if(!ui->lineEdit_general_06->text().isEmpty())
        stream->writeCharacters(ui->lineEdit_general_06->text());
    stream->writeEndElement();

    stream->writeComment("trajectories file and format");
    stream->writeStartElement("trajectories");
    if(!ui->comboBox_general_02->currentText().isEmpty())
        stream->writeAttribute("format",ui->comboBox_general_02->currentText());
    if(!ui->lineEdit_general_10->text().isEmpty())
        stream->writeAttribute("fps", ui->lineEdit_general_10->text());
    if(!ui->comboBox_general_03->currentText().isEmpty())
        stream->writeAttribute("color_mode",ui->comboBox_general_03->currentText());
    stream->writeStartElement("file");
    if(!ui->lineEdit_general_12->text().isEmpty())
        stream->writeAttribute("location", ui->lineEdit_general_12->text());
    stream->writeEndElement(); //end file
    stream->writeEndElement(); //end trajectories

    stream->writeComment("where to store the logs");
    stream->writeStartElement("logfile");
    if(!ui->lineEdit_general_08->text().isEmpty())
        stream->writeCharacters(ui->lineEdit_general_08->text());
    stream->writeEndElement();

    stream->writeComment("statistics");
    stream->writeStartElement("show_statistics");
    if(!ui->comboBox_general_01->currentText().isEmpty())
        stream->writeCharacters(ui->comboBox_general_01->currentText());
    stream->writeEndElement();
}

void InifileWidget::writeFire(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("JPSfire");

    if(ui->checkBox_A->isChecked())
    {
        stream->writeStartElement("A_smoke_sensor");
        if(!ui->lineEdit_A_grids->text().isEmpty())
            stream->writeAttribute("smoke_factor_grids", ui->lineEdit_A_grids->text());
        if(!ui->lineEdit_A_update->text().isEmpty())
            stream->writeAttribute("update_time", ui->lineEdit_A_update->text());
        if(!ui->lineEdit_A_final->text().isEmpty())
            stream->writeAttribute("final_time", ui->lineEdit_A_final->text());
        stream->writeEndElement(); //end smoke
    }

    if(ui->checkBox_B->isChecked())
    {
        stream->writeStartElement("B_walking_speed");
        if(!ui->lineEdit_B_grids->text().isEmpty())
            stream->writeAttribute("smoke_factor_grids", ui->lineEdit_B_grids->text());
        if(!ui->lineEdit_B_update->text().isEmpty())
            stream->writeAttribute("update_time", ui->lineEdit_B_update->text());
        if(!ui->lineEdit_B_final->text().isEmpty())
            stream->writeAttribute("final_time", ui->lineEdit_B_final->text());
        stream->writeEndElement(); //end walking
    }

    if(ui->checkBox_C->isChecked())
    {
        stream->writeStartElement("C_toxicity_analysis");
        if(!ui->lineEdit_C_grids->text().isEmpty())
            stream->writeAttribute("smoke_factor_grids", ui->lineEdit_C_grids->text());
        if(!ui->lineEdit_C_update->text().isEmpty())
            stream->writeAttribute("update_time", ui->lineEdit_C_update->text());
        if(!ui->lineEdit_C_final->text().isEmpty())
            stream->writeAttribute("final_time", ui->lineEdit_C_final->text());
        stream->writeEndElement(); //end toxicity
    }
    stream->writeEndElement(); //end JPSfire

}



/*
<routing>
    <goals>
        <goal id="0" final="false" caption="goal 1">
            <polygon>
                <vertex px="-5.0" py="-5.0" />
                <vertex px="-5.0" py="-2.0" />
                <vertex px="-3.0" py="-2.0" />
                <vertex px="-3.0" py="-5.0" />
                <vertex px="-5.0" py="-5.0" />
            </polygon>
        </goal>
        <file>goals.xml</file>
    </goals>
</routing>
*/

void InifileWidget::writeRoutingData(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeComment("routing");
    stream->writeStartElement("routing");

    stream->writeStartElement("goals");
    QList<JPSGoal *> goallist = dataManager->getGoallist();
    dataManager->writeGoals(stream, goallist);

    auto goal_FileName = ui->lineEdit_GoalFile->text().split("/").last();

    stream->writeStartElement("file");
    if(!goal_FileName.isEmpty())
        stream->writeCharacters(goal_FileName);

    stream->writeEndElement(); //end files
    stream->writeEndElement(); //end goals
    stream->writeEndElement(); //end routing
}

/*
    since v0.8.8

    <traffic_constraints>
        <doors>
          <door trans_id="1" caption="NaN" state="open" />
        </doors>
        <file>traffic.xml</file>
    </traffic_constraints>
*/

void InifileWidget::writeTrafficData(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeComment("traffic information: e.g closed doors or smoked rooms");
    stream->writeStartElement("traffic_constraints");

    stream->writeStartElement("doors");
    QList<jpsCrossing *> crossings = dataManager->get_crossingList();
    QList<jpsCrossing *> doorlist;

    for(jpsCrossing *crossing:crossings)
    {
//        if(crossing->IsExit())
//            doorlist.append(crossing);
    }

    dataManager->writeTraffics(stream, doorlist);

    auto traffic_FileName = ui->lineEdit_TrafficFile->text().split("/").last();
    stream->writeStartElement("file");
    if(!traffic_FileName.isEmpty())
        stream->writeCharacters(traffic_FileName);

    stream->writeEndElement(); //end files
    stream->writeEndElement(); //end doors
    stream->writeEndElement(); //end traffic_constraints
}

void InifileWidget::writeAgentData(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeComment("persons information and distribution");
    stream->writeStartElement("agents");
    stream->writeAttribute("operational_model_id", QString::number(ui->comboBox_groups_1->currentIndex() + 1));

    stream->writeStartElement("agents_distribution");

    for(int i = 0; i < ui->tableWidget_groups_1->rowCount(); i++)
    {
        stream->writeStartElement("group");

        // item(0,0) isn't a nullptr ever！
        if(!ui->tableWidget_groups_1->item(i,0)->text().isEmpty())
            stream->writeAttribute("group_id", ui->tableWidget_groups_1->item(i,0)->text());

        if(ui->tableWidget_groups_1->item(i,1) != nullptr)
            stream->writeAttribute("agent_parameter_id", ui->tableWidget_groups_1->item(i,1)->text());

        if(ui->tableWidget_groups_1->item(i,2) != nullptr)
            stream->writeAttribute("room_id", ui->tableWidget_groups_1->item(i,2)->text());

        if(ui->tableWidget_groups_1->item(i,3) != nullptr)
            stream->writeAttribute("subroom_id",ui->tableWidget_groups_1->item(i,3)->text());

        if(ui->tableWidget_groups_1->item(i,4) != nullptr)
            stream->writeAttribute("number",ui->tableWidget_groups_1->item(i,4)->text());

        if(ui->tableWidget_groups_1->item(i,5) != nullptr)
            stream->writeAttribute("goal_id",ui->tableWidget_groups_1->item(i,5)->text());

        if(ui->tableWidget_groups_1->item(i,6) != nullptr)
            stream->writeAttribute("router_id",ui->tableWidget_groups_1->item(i,6)->text());

        if(ui->tableWidget_groups_1->item(i,7) != nullptr)
            stream->writeAttribute("x_min",ui->tableWidget_groups_1->item(i,7)->text());

        if(ui->tableWidget_groups_1->item(i,8) != nullptr)
            stream->writeAttribute("x_max",ui->tableWidget_groups_1->item(i,8)->text());

        if(ui->tableWidget_groups_1->item(i,9) != nullptr)
            stream->writeAttribute("y_min",ui->tableWidget_groups_1->item(i,9)->text());

        if(ui->tableWidget_groups_1->item(i,10) != nullptr)
            stream->writeAttribute("y_max",ui->tableWidget_groups_1->item(i,10)->text());

        if(ui->tableWidget_groups_1->item(i,11) != nullptr)
            stream->writeAttribute("pre_movement_mean",ui->tableWidget_groups_1->item(i,11)->text());

        if(ui->tableWidget_groups_1->item(i,12) != nullptr)
            stream->writeAttribute("pre_movement_sigma",ui->tableWidget_groups_1->item(i,12)->text());

        if(ui->tableWidget_groups_1->item(i,13) != nullptr)
            stream->writeAttribute("risk_tolerance_mean",ui->tableWidget_groups_1->item(i,13)->text());

        if(ui->tableWidget_groups_1->item(i,14) != nullptr)
            stream->writeAttribute("risk_tolerance_sigma",ui->tableWidget_groups_1->item(i,14)->text());

        stream->writeEndElement(); //end group
    }

    stream->writeEndElement(); //end agents_distribution;

    // write agents sources
    stream->writeComment("frequency in persons/seconds");
    stream->writeStartElement("agents_sources");

    QList<JPSSource *> sources = dataManager->getSourcelist();

    dataManager->writeSources(stream, sources);

    auto source_FileName = ui->lineEdit_SourceFile->text().split("/").last();
    stream->writeStartElement("file");
    if(!source_FileName.isEmpty())
        stream->writeCharacters(source_FileName);
    stream->writeEndElement(); //end files

    stream->writeEndElement(); //end agents_sources
    stream->writeEndElement(); //end agents

}

void InifileWidget::writeModelGcfmData(QXmlStreamWriter *stream, QFile &file)
{
    //operational model and agent parameters - gcfm
    stream->writeComment("operational models");
    stream->writeStartElement("operational_models");

    stream->writeStartElement("model");
    stream->writeAttribute("operational_model_id","1");
    stream->writeAttribute("description", "gcfm");

    stream->writeStartElement("model_parameters");
    if(!ui->lineEdit_model_gcfm_01->text().isEmpty())
        stream->writeTextElement("solver", ui->lineEdit_model_gcfm_01->text());
    if(!ui->lineEdit_model_gcfm_02->text().isEmpty())
        stream->writeTextElement("stepsize", ui->lineEdit_model_gcfm_02->text());
    if(!ui->lineEdit_model_gcfm_03->text().isEmpty())
        stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gcfm_03->text());
    if(!ui->lineEdit_model_gcfm_14->text().isEmpty())
        stream->writeTextElement("periodic", ui->lineEdit_model_gcfm_14->text());


    stream->writeStartElement("linkedcells");
    if(!ui->lineEdit_model_gcfm_04->text().isEmpty())
        stream->writeAttribute("enabled", ui->lineEdit_model_gcfm_04->text());
    if(!ui->lineEdit_model_gcfm_05->text().isEmpty())
        stream->writeAttribute("cell_size", ui->lineEdit_model_gcfm_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    if(!ui->lineEdit_model_gcfm_06->text().isEmpty())
        stream->writeAttribute("nu", ui->lineEdit_model_gcfm_06->text());
    if(!ui->lineEdit_model_gcfm_07->text().isEmpty())
        stream->writeAttribute("dist_max", ui->lineEdit_model_gcfm_07->text());
    if(!ui->lineEdit_model_gcfm_08->text().isEmpty())
        stream->writeAttribute("desteff_max", ui->lineEdit_model_gcfm_08->text());
    if(!ui->lineEdit_model_gcfm_09->text().isEmpty())
        stream->writeAttribute("interpolation_width", ui->lineEdit_model_gcfm_09->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    if(!ui->lineEdit_model_gcfm_10->text().isEmpty())
        stream->writeAttribute("nu",ui->lineEdit_model_gcfm_10->text());
    if(!ui->lineEdit_model_gcfm_11->text().isEmpty())
        stream->writeAttribute("dist_max", ui->lineEdit_model_gcfm_11->text());
    if(!ui->lineEdit_model_gcfm_12->text().isEmpty())
        stream->writeAttribute("desteff_max", ui->lineEdit_model_gcfm_12->text());
    if(!ui->lineEdit_model_gcfm_13->text().isEmpty())
        stream->writeAttribute("interpolation_width", ui->lineEdit_model_gcfm_13->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    writeAgentParameters(stream, file);

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model
}

void InifileWidget::writeModelGompData(QXmlStreamWriter *stream, QFile &file)
{
    //operational model and agent parameters - gcfm
    stream->writeComment("operational model");
    stream->writeStartElement("operational_models");

    stream->writeStartElement("model");
    stream->writeAttribute("operational_model_id","2");
    stream->writeAttribute("description", "gompertz");

    stream->writeStartElement("model_parameters");
    if(!ui->lineEdit_model_gompertz_01->text().isEmpty())
        stream->writeTextElement("solver", ui->lineEdit_model_gompertz_01->text());
    if(!ui->lineEdit_model_gompertz_02->text().isEmpty())
        stream->writeTextElement("stepsize", ui->lineEdit_model_gompertz_02->text());
    if(!ui->lineEdit_model_gompertz_03->text().isEmpty())
        stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gompertz_03->text());
    if(!ui->lineEdit_model_gompertz_12->text().isEmpty())
        stream->writeTextElement("periodic", ui->lineEdit_model_gompertz_12->text());

    stream->writeStartElement("linkedcells");
    if(!ui->lineEdit_model_gompertz_04->text().isEmpty())
        stream->writeAttribute("enabled", ui->lineEdit_model_gompertz_04->text());
    if(!ui->lineEdit_model_gompertz_05->text().isEmpty())
        stream->writeAttribute("cell_size", ui->lineEdit_model_gompertz_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    if(!ui->lineEdit_model_gompertz_06->text().isEmpty())
        stream->writeAttribute("nu", ui->lineEdit_model_gompertz_06->text());
    if(!ui->lineEdit_model_gompertz_07->text().isEmpty())
        stream->writeAttribute("b", ui->lineEdit_model_gompertz_07->text());
    if(!ui->lineEdit_model_gompertz_08->text().isEmpty())
        stream->writeAttribute("c", ui->lineEdit_model_gompertz_08->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    if(!ui->lineEdit_model_gompertz_09->text().isEmpty())
        stream->writeAttribute("nu",ui->lineEdit_model_gompertz_09->text());
    if(!ui->lineEdit_model_gompertz_10->text().isEmpty())
        stream->writeAttribute("b", ui->lineEdit_model_gompertz_10->text());
    if(!ui->lineEdit_model_gompertz_11->text().isEmpty())
        stream->writeAttribute("c", ui->lineEdit_model_gompertz_11->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    writeAgentParameters(stream, file);

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model
}

void InifileWidget::writeModelTordData(QXmlStreamWriter *stream, QFile &file)
{
    //operational model and agent parameters - gcfm
    stream->writeComment("operational model");
    stream->writeStartElement("operational_models");

    stream->writeStartElement("model");
    stream->writeAttribute("operational_model_id","3");
    stream->writeAttribute("description", "Tordeux2015");

    stream->writeStartElement("model_parameters");
    if(!ui->lineEdit_model_tordeux_01->text().isEmpty())
        stream->writeTextElement("solver", ui->lineEdit_model_tordeux_01->text());
    if(!ui->lineEdit_model_tordeux_02->text().isEmpty())
        stream->writeTextElement("stepsize", ui->lineEdit_model_tordeux_02->text());
    if(!ui->lineEdit_model_tordeux_03->text().isEmpty())
        stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_tordeux_03->text());
    if(!ui->lineEdit_model_tordeux_10->text().isEmpty())
        stream->writeTextElement("periodic", ui->lineEdit_model_tordeux_10->text());

    stream->writeStartElement("linkedcells");
    if(!ui->lineEdit_model_tordeux_04->text().isEmpty())
        stream->writeAttribute("enabled", ui->lineEdit_model_tordeux_04->text());
    if(!ui->lineEdit_model_tordeux_05->text().isEmpty())
        stream->writeAttribute("cell_size", ui->lineEdit_model_tordeux_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    if(!ui->lineEdit_model_tordeux_06->text().isEmpty())
        stream->writeAttribute("a", ui->lineEdit_model_tordeux_06->text());
    if(!ui->lineEdit_model_tordeux_07->text().isEmpty())
        stream->writeAttribute("D", ui->lineEdit_model_tordeux_07->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    if(!ui->lineEdit_model_tordeux_08->text().isEmpty())
        stream->writeAttribute("a",ui->lineEdit_model_tordeux_08->text());
    if(!ui->lineEdit_model_tordeux_09->text().isEmpty())
        stream->writeAttribute("D", ui->lineEdit_model_tordeux_09->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    writeAgentParameters(stream, file);

    stream->writeEndElement(); //end model

    stream->writeEndElement(); //end operational model
}

void InifileWidget::writeModelGradData(QXmlStreamWriter *stream, QFile &file)
{
    //operational model and agent parameters - gcfm
    stream->writeComment("operational model");
    stream->writeStartElement("operational_models");

    stream->writeStartElement("model");
    stream->writeAttribute("operational_model_id","4");
    stream->writeAttribute("description", "gradnav");

    stream->writeStartElement("model_parameters");
    if(!ui->lineEdit_model_gradnav_01->text().isEmpty())
        stream->writeTextElement("solver", ui->lineEdit_model_gradnav_01->text());
    if(!ui->lineEdit_model_gradnav_02->text().isEmpty())
        stream->writeTextElement("stepsize", ui->lineEdit_model_gradnav_02->text());
    if(!ui->lineEdit_model_gradnav_03->text().isEmpty())
        stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gradnav_03->text());
    if(!ui->lineEdit_model_gradnav_16->text().isEmpty())
        stream->writeTextElement("periodic", ui->lineEdit_model_gradnav_16->text());

    stream->writeStartElement("linkedcells");
    if(!ui->lineEdit_model_gradnav_04->text().isEmpty())
        stream->writeAttribute("enabled", ui->lineEdit_model_gradnav_04->text());
    if(!ui->lineEdit_model_gradnav_05->text().isEmpty())
        stream->writeAttribute("cell_size", ui->lineEdit_model_gradnav_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("floorfield");
    if(!ui->lineEdit_model_gradnav_06->text().isEmpty())
        stream->writeAttribute("delta_h", ui->lineEdit_model_gradnav_06->text());
    if(!ui->lineEdit_model_gradnav_07->text().isEmpty())
        stream->writeAttribute("wall_avoid_distance", ui->lineEdit_model_gradnav_07->text());
    if(!ui->lineEdit_model_gradnav_08->text().isEmpty())
        stream->writeAttribute("use_wall_avoidance", ui->lineEdit_model_gradnav_08->text());
    stream->writeEndElement(); //end floorfield

    stream->writeStartElement("anti_clipping");
    if(!ui->lineEdit_model_gradnav_09->text().isEmpty())
        stream->writeAttribute("slow_down_distance", ui->lineEdit_model_gradnav_09->text());
    stream->writeEndElement(); //end anti_clipping

    stream->writeStartElement("force_ped");
    if(!ui->lineEdit_model_gradnav_10->text().isEmpty())
        stream->writeAttribute("nu", ui->lineEdit_model_gradnav_10->text());
    if(!ui->lineEdit_model_gradnav_11->text().isEmpty())
        stream->writeAttribute("b", ui->lineEdit_model_gradnav_11->text());
    if(!ui->lineEdit_model_gradnav_12->text().isEmpty())
        stream->writeAttribute("c", ui->lineEdit_model_gradnav_12->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    if(!ui->lineEdit_model_gradnav_13->text().isEmpty())
        stream->writeAttribute("nu", ui->lineEdit_model_gradnav_13->text());
    if(!ui->lineEdit_model_gradnav_14->text().isEmpty())
        stream->writeAttribute("b", ui->lineEdit_model_gradnav_14->text());
    if(!ui->lineEdit_model_gradnav_15->text().isEmpty())
        stream->writeAttribute("c", ui->lineEdit_model_gradnav_15->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    writeAgentParameters(stream, file);

    stream->writeEndElement(); //end model

    stream->writeEndElement(); //end operational model
}

void InifileWidget::writeModelKrauData(QXmlStreamWriter *stream, QFile &file)
{
    //operational model and agent parameters - gcfm
    stream->writeComment("operational model");
    stream->writeStartElement("operational_models");

    stream->writeStartElement("model");
    stream->writeAttribute("operational_model_id","5");
    stream->writeAttribute("description", "krausz");

    stream->writeStartElement("model_parameters");
    if(!ui->lineEdit_model_krausz_01->text().isEmpty())
        stream->writeTextElement("solver", ui->lineEdit_model_krausz_01->text());
    if(!ui->lineEdit_model_krausz_02->text().isEmpty())
        stream->writeTextElement("stepsize", ui->lineEdit_model_krausz_02->text());
    if(!ui->lineEdit_model_krausz_03->text().isEmpty())
        stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_krausz_03->text());
    if(!ui->lineEdit_model_krausz_06->text().isEmpty())
        stream->writeTextElement("periodic", ui->lineEdit_model_krausz_06->text());

    stream->writeStartElement("linkedcells");
    if(!ui->lineEdit_model_krausz_04->text().isEmpty())
        stream->writeAttribute("enabled", ui->lineEdit_model_krausz_04->text());
    if(!ui->lineEdit_model_krausz_05->text().isEmpty())
        stream->writeAttribute("cell_size", ui->lineEdit_model_krausz_05->text());
    stream->writeEndElement(); //end linkedcells

    writeAgentParameters(stream, file);

    stream->writeEndElement(); //end model

    stream->writeEndElement(); //end operational model
}

void InifileWidget::writeAgentParameters(QXmlStreamWriter *stream, QFile &file)
{
    for(int i = 0; i < ui->tableWidget_agents->rowCount(); i++) // // start agent_parameters
    {
        stream->writeStartElement("agent_parameters");
        if(ui->tableWidget_agents->item(i,0) != nullptr)
            stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents->item(i,0)->text());

        stream->writeStartElement("v0");
        if(ui->tableWidget_agents->item(i,1) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,1)->text());

        if(ui->tableWidget_agents->item(i,2) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");

        if(ui->tableWidget_agents->item(i,3) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,3)->text());

        if(ui->tableWidget_agents->item(i,4) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,4)->text());

        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");

        if(ui->tableWidget_agents->item(i,5) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,5)->text());

        if(ui->tableWidget_agents->item(i,6) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,6)->text());

        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");

        if(ui->tableWidget_agents->item(i,7) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,7)->text());

        if(ui->tableWidget_agents->item(i,8) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,8)->text());

        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");

        if(ui->tableWidget_agents->item(i,9) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,9)->text());

        if(ui->tableWidget_agents->item(i,10) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,10)->text());

        stream->writeEndElement();

        stream->writeStartElement("bmax");

        if(ui->tableWidget_agents->item(i,11) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,11)->text());

        if(ui->tableWidget_agents->item(i,12) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,12)->text());

        stream->writeEndElement();

        stream->writeStartElement("bmin");

        if(ui->tableWidget_agents->item(i,13) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,13)->text());

        if(ui->tableWidget_agents->item(i,14) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,14)->text());

        stream->writeEndElement();

        stream->writeStartElement("amin");

        if(ui->tableWidget_agents->item(i,15) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,15)->text());

        if(ui->tableWidget_agents->item(i,16) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,16)->text());

        stream->writeEndElement();

        stream->writeStartElement("tau");

        if(ui->tableWidget_agents->item(i,17) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,17)->text());

        if(ui->tableWidget_agents->item(i,17) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");

        if(ui->tableWidget_agents->item(i,19) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,19)->text());

        if(ui->tableWidget_agents->item(i,20) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,20)->text());

        stream->writeEndElement(); // end atau

        stream->writeStartElement("T");

        if(ui->tableWidget_agents->item(i,21) != nullptr)
            stream->writeAttribute("mu", ui->tableWidget_agents->item(i,21)->text());

        if(ui->tableWidget_agents->item(i,22) != nullptr)
            stream->writeAttribute("sigma", ui->tableWidget_agents->item(i,22)->text());

        stream->writeEndElement(); // end T

        stream->writeStartElement("sway");

        if(ui->tableWidget_agents->item(i,23) != nullptr)
            stream->writeAttribute("ampA", ui->tableWidget_agents->item(i,23)->text());

        if(ui->tableWidget_agents->item(i,24) != nullptr)
            stream->writeAttribute("ampB", ui->tableWidget_agents->item(i,24)->text());

        if(ui->tableWidget_agents->item(i,25) != nullptr)
            stream->writeAttribute("freqA", ui->tableWidget_agents->item(i,25)->text());

        if(ui->tableWidget_agents->item(i,26) != nullptr)
            stream->writeAttribute("freqB", ui->tableWidget_agents->item(i,26)->text());

        stream->writeEndElement(); // end sway

        stream->writeEndElement(); // end agent_parameters
    }

}

void InifileWidget::writeRouteChoiceData(QXmlStreamWriter *stream, QFile &file)
{

    stream->writeComment("route chice model");

    stream->writeStartElement("route_choice_models");

    if(ui->checkBox_ff->isChecked())
    {
        writeFFGlobalShortestRoute(stream, file);
    }

    if(ui->checkBox_global->isChecked())
    {
        writeGlobalShortestRoute(stream, file);
    }

    if(ui->checkBox_trips->isChecked())
    {
        writeTripsRoute(stream, file);
    }

    stream->writeEndElement();

}

void InifileWidget::writeFFGlobalShortestRoute(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_ff_global_ID->text());
    stream->writeAttribute("description","ff_global_shortest");

    if(ui->comboBox_ff_global->currentText() == "True")
        stream->writeTextElement("write_VTK_files", "true");
    else
        stream->writeTextElement("write_VTK_files", "false");

    stream->writeEndElement();
}

void InifileWidget::writeGlobalShortestRoute(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_global_ID->text());
    stream->writeAttribute("description","global_shortest");

    stream->writeStartElement("parameters");
    stream->writeStartElement("navigation_lines");
    stream->writeAttribute("file", ui->lineEdit_global_navi->text());
    stream->writeEndElement(); //end navigation lines
    stream->writeEndElement(); //end patameters
    stream->writeEndElement(); //end router
}

void InifileWidget::writeCognitiveMap(QXmlStreamWriter *stream, QFile &file)
{
//    stream->writeStartElement("router");
//    stream->writeAttribute("router_id", ui->lineEdit_ID->text());
//    stream->writeAttribute("description","cognitive_map");
//    stream->writeStartElement("sensors");
//    stream->writeEndElement();//end sensors
//
//    stream->writeStartElement("cognitive_map");
//    stream->writeAttribute("status", "complete");
//    stream->writeEndElement(); //end cognitive map
//    stream->writeEndElement(); //router
}

void InifileWidget::writeTripsRoute(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_trips_ID->text());
    stream->writeAttribute("description","ff_global_shortest_trips");
    stream->writeEndElement(); //end router
}

// Read ini.xml on button push
void InifileWidget::on_pushButton_read_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Read ini"),"",tr("XML-Files (*.xml)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    auto reader = new QXmlStreamReader(&file);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this, tr("JPSeditor"),
                             tr("Cannot read file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(fileName),
                                          file.errorString()));
        return;
    }

    if (!readInifile(reader)) {
        QMessageBox::warning(this, tr("JPSeditor"),
                             tr("Parse error in file %1:\n\n%2")
                                     .arg(QDir::toNativeSeparators(fileName),
                                          errorString(reader)));
    } else {
        emit inifileLoaded(tr("inifile loaded"), 2000);
    }
}

bool InifileWidget::readInifile(QXmlStreamReader *reader)
{
    if (reader->readNextStartElement())
    {
        if (reader->name() == QLatin1String("JuPedSim")
            && reader->attributes().value("version") == QLatin1String("0.8"))
        {
            readJuPedSim(reader);
        }
        else {
            return false;
//            reader->raiseError(QObject::tr("The file is not an JuPedSim version 0.8 file."));
        }
    }

    return true;
//    return reader->error();
}

void InifileWidget::readFromTemplate(QString name) {
    qDebug("Enter InifileWidget::readFromTemplate");
    QString fileName = ":/inifile_templates/" + name + ".xml";

    QFile file(fileName);

    auto reader = new QXmlStreamReader(&file);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this, tr("JPSeditor"),
                             tr("Cannot read file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(fileName),
                                          file.errorString()));
        return;
    }

    if (!readInifile(reader)) {
        QMessageBox::warning(this, tr("JPSeditor"),
                             tr("Parse error in file %1:\n\n%2")
                                     .arg(QDir::toNativeSeparators(fileName),
                                          errorString(reader)));
    } else {
        emit inifileLoaded(tr("inifile loaded"), 2000);
    }
    qDebug("Leave InifileWidget::readFromTemplate");
}

QString InifileWidget::errorString(QXmlStreamReader *reader) const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(reader->errorString())
            .arg(reader->lineNumber())
            .arg(reader->columnNumber());
}

void InifileWidget::readJuPedSim(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("JuPedSim"));

    QString project = reader->attributes().value("project").toString();
    QString version = reader->attributes().value("version").toString();

    ui->lineEdit_general_01->setText(project);
    ui->lineEdit_general_02->setText(version);

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("seed"))
            readSeed(reader);
        else if (reader->name() == QLatin1String("num_threads"))
            readThread(reader);
        else if (reader->name() == QLatin1String("geometry"))
            readGeometry(reader);
        else if (reader->name() == QLatin1String("max_sim_time"))
            readMaxSimTime(reader);
        else if (reader->name() == QLatin1String("trajectories"))
            readTrajectories(reader);
        else if (reader->name() == QLatin1String("logfile"))
            readLogfile(reader);
        else if (reader->name() == QLatin1String("JPSfire"))
            readFire(reader);
        else if (reader->name() == QLatin1String("traffic_constraints"))
            readTrafficConstraints(reader);
        else if (reader->name() == QLatin1String("routing"))
            readRouting(reader);
        else if (reader->name() == QLatin1String("agents"))
            readAgents(reader);
        else if (reader->name() == QLatin1String("operational_models"))
            readModels(reader);
        else if (reader->name() == QLatin1String("route_choice_models"))
            readRouteChoiceModels(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readSeed(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("seed"));

    QString title = reader->readElementText();
    ui->lineEdit_general_05->setText(title);
}

void InifileWidget::readThread(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("num_threads"));

    QString title = reader->readElementText();
    ui->lineEdit_general_03->setText(title);
}

void InifileWidget::readGeometry(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("geometry"));

    QString title = reader->readElementText();
    ui->lineEdit_general_07->setText(title);
}

void InifileWidget::readMaxSimTime(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("max_sim_time"));

    QString title = reader->readElementText();
    ui->lineEdit_general_06->setText(title);
}

void InifileWidget::readTrajectories(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("trajectories"));

    QString format = reader->attributes().value("format").toString();
    QString fps = reader->attributes().value("fps").toString();
    QString color_mode = reader->attributes().value("color_mode").toString();

    ui->comboBox_general_02->setCurrentText(format);
    ui->lineEdit_general_10->setText(fps);
    ui->comboBox_general_03->setCurrentText(color_mode);

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("file"))
            readTrajectoriesFile(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readTrajectoriesFile(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("file"));

    QString file = reader->attributes().value("location").toString();
    ui->lineEdit_general_12->setText(file);

    reader->readElementText(); //move current token to end element, so that readNextStartElement can work fine.
}

void InifileWidget::readLogfile(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("logfile"));

    QString logtxt = reader->readElementText();
    ui->lineEdit_general_08->setText(logtxt);
}

/*
    <JPSfire>
      <A_smoke_sensor smoke_factor_grids="/path/tp//3_sfgrids/" update_time="10.0" final_time="100.0" />
      <B_walking_speed/>
      <C_toxicity_analysis/>
    </JPSfire>
*/

void InifileWidget::readFire(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("JPSfire"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("A_smoke_sensor"))
            readSmoke(reader);
        else if(reader->name() == QLatin1String("B_walking_speed"))
            readWalking(reader);
        else if(reader->name() == QLatin1String("C_toxicity_analysis"))
            readToxicity(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readSmoke(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("A_smoke_sensor"));

    ui->checkBox_A->setChecked(true);

    QString grids = reader->attributes().value("smoke_factor_grids").toString();
    QString update_time = reader->attributes().value("update_time").toString();
    QString final_time = reader->attributes().value("final_time").toString();

    ui->lineEdit_A_grids->setText(grids);
    ui->lineEdit_A_update->setText(update_time);
    ui->lineEdit_A_final->setText(final_time);

    reader->readNext();//now is attribute element, move to end element
}

void InifileWidget::readWalking(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("B_walking_speed"));

    ui->checkBox_B->setChecked(true);

    QString grids = reader->attributes().value("smoke_factor_grids").toString();
    QString update_time = reader->attributes().value("update_time").toString();
    QString final_time = reader->attributes().value("final_time").toString();

    ui->lineEdit_B_grids->setText(grids);
    ui->lineEdit_B_update->setText(update_time);
    ui->lineEdit_B_final->setText(final_time);

    reader->readNext();//now is attribute element, move to end element
}

void InifileWidget::readToxicity(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("C_toxicity_analysis"));

    ui->checkBox_C->setChecked(true);

    QString grids = reader->attributes().value("smoke_factor_grids").toString();
    QString update_time = reader->attributes().value("update_time").toString();
    QString final_time = reader->attributes().value("final_time").toString();

    ui->lineEdit_C_grids->setText(grids);
    ui->lineEdit_C_update->setText(update_time);
    ui->lineEdit_C_final->setText(final_time);

    reader->readNext();//now is attribute element, move to end element
}

/*
    <traffic_constraints>
        <doors>
            <door trans_id="1" caption="NaN" state="open" />
        </doors>
        <file>traffic.xml</file>
    </traffic_constraints>
*/
void InifileWidget::readTrafficConstraints(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("traffic_constraints"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("file"))
            readTrafficFile(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readTrafficFile(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("file"));

    QString file = reader->readElementText();
    ui->lineEdit_TrafficFile->setText(file);
}

/*
    <routing>
        <goals>
          <goal id="3" final="true" caption="goal3">
            <polygon>
              <vertex px="3.2" py="12.0" />
              <vertex px="3.2" py="13.0" />
            </polygon>
          </goal>
        </goals>
        <file>goals.xml</file>
    </routing>
*/

void InifileWidget::readRouting(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("routing"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("file"))
            readRoutingFile(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readRoutingFile(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("file"));

    QString file = reader->readElementText();
    ui->lineEdit_GoalFile->setText(file);
}

/*
  <agents operational_model_id="3">
    <agents_distribution>
      <group group_id="5" room_id="0" subroom_id="0" number="10" router_id="1" agent_parameter_id="1" x_min="3" x_max="5" y_min="3" y_max="5" />
      <group group_id="0" room_id="0" subroom_id="0" number="0" router_id="1" agent_parameter_id="1" />
    </agents_distribution>

    <agents_sources>
      <source_ id="10" caption="new-source" time_min="5" time_max="30" frequency="5" N_create="10" agents_max="300"
      group_id="0"  x_min="0" x_max="3" y_min="0" y_max="3" percent="0.5" rate="2"  greedy="true"/>
      <file>sources.xml</file>
    </agents_sources>
  </agents>
*/

void InifileWidget::readAgents(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("agents"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("agents_distribution"))
            readAgentsDistribution(reader);
        else if (reader->name() == QLatin1String("agents_sources"))
            readAgentsSources(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readAgentsDistribution(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("agents_distribution"));

    while (reader->readNextStartElement())
    {
        if(reader->name() == QLatin1String("group"))
            readGroup(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readGroup(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("group"));

    int row = ui->tableWidget_groups_1->rowCount();

    QString group_id = reader->attributes().value("group_id").toString();
    QTableWidgetItem *id = new QTableWidgetItem(tr("%1").arg(group_id));
    ui->tableWidget_groups_1->setItem(row-1, 0, id);

    QString agent_parameter_id = reader->attributes().value("agent_parameter_id").toString();
    QTableWidgetItem *agent_parameter = new QTableWidgetItem(tr("%1").arg(agent_parameter_id));
    ui->tableWidget_groups_1->setItem(row-1, 1, agent_parameter);

    QString room_id = reader->attributes().value("room_id").toString();
    QTableWidgetItem *room = new QTableWidgetItem(tr("%1").arg(room_id));
    ui->tableWidget_groups_1->setItem(row-1, 2, room);

    QString subroom_id = reader->attributes().value("subroom_id").toString();
    QTableWidgetItem *subroom = new QTableWidgetItem(tr("%1").arg(subroom_id));
    ui->tableWidget_groups_1->setItem(row-1, 3, subroom);

    QString number = reader->attributes().value("number").toString();
    QTableWidgetItem *num = new QTableWidgetItem(tr("%1").arg(number));
    ui->tableWidget_groups_1->setItem(row-1, 4, num);

    QString router_id = reader->attributes().value("router_id").toString();
    QTableWidgetItem *router = new QTableWidgetItem(tr("%1").arg(router_id));
    ui->tableWidget_groups_1->setItem(row-1, 6, router);

    QString x_min = reader->attributes().value("x_min").toString();
    QTableWidgetItem *xmin = new QTableWidgetItem(tr("%1").arg(x_min));
    ui->tableWidget_groups_1->setItem(row-1, 7, xmin);

    QString x_max = reader->attributes().value("x_max").toString();
    QTableWidgetItem *xmax = new QTableWidgetItem(tr("%1").arg(x_max));
    ui->tableWidget_groups_1->setItem(row-1, 8, xmax);

    QString y_min = reader->attributes().value("y_min").toString();
    QTableWidgetItem *ymin = new QTableWidgetItem(tr("%1").arg(y_min));
    ui->tableWidget_groups_1->setItem(row-1, 9, ymin);

    QString y_max = reader->attributes().value("y_max").toString();
    QTableWidgetItem *ymax = new QTableWidgetItem(tr("%1").arg(x_max));
    ui->tableWidget_groups_1->setItem(row-1, 10, ymax);

    ui->tableWidget_groups_1->setRowCount(row+1); // add line for next group

    reader->readElementText();
}

void InifileWidget::readAgentsSources(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("agents_sources"));


    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("file"))
            readSourceFile(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readSourceFile(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("file"));
    QString file = reader->readElementText();
    ui->lineEdit_SourceFile->setText(file);
}

/*
    <operational_models>
        <model operational_model_id="3" description="Tordeux2015">
            <model_parameters>
                <solver>euler</solver>
                <stepsize>0.01</stepsize>
                <exit_crossing_strategy>2</exit_crossing_strategy>
                <linkedcells enabled="true" cell_size="2.2" />
                <force_ped a="5" D="0.1" />
                <force_wall a="5" D="0.02" />
            </model_parameters>
            <agent_parameters agent_parameter_id="1">
                <v0 mu="1.0" sigma="0.001" />
                <bmax mu="0.15" sigma="0.00000" />
                <bmin mu="0.15" sigma="0.00000" />
                <amin mu="0.15" sigma="0.00000" />
                <tau mu="0.5" sigma="0.001" />
                <atau mu="0.0" sigma="0.00000" />
                <T mu="1" sigma="0.001" />
            </agent_parameters>
        </model>
    </operational_models>
*/

void InifileWidget::readModels(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("operational_models"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("model"))
            readModel(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readModel(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("model"));

    int id = reader->attributes().value("operational_model_id").toInt();
    reader->readNext();//now is attribute element, move to end element

    // change tab widget to current model
    ui->comboBox_groups_1->setCurrentIndex(id-1);
    on_comboBox_groups_1_currentIndexChanged(id-1); //tabs changed

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("model_parameters"))
            readModelParameters(reader, id);
        else if (reader->name() == QLatin1String("agent_parameters"))
            readAgentParamaters(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readModelParameters(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("model_parameters"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("solver"))
            readSlover(reader, modelindex); // for all models
        else if (reader->name() == QLatin1String("stepsize"))
            readStepsize(reader, modelindex); // for all models
        else if (reader->name() == QLatin1String("exit_crossing_strategy"))
            readExit(reader, modelindex); // for all models
        else if (reader->name() == QLatin1String("linkedcells"))
            readLinkedcells(reader, modelindex); // for all models
        else if (reader->name() == QLatin1String("periodic"))
            readPeriodic(reader, modelindex); // for all models
        else if (reader->name() == QLatin1String("force_ped"))
            readForcePed(reader, modelindex); // for gcfm, gompertz, Tordeux2015, gradnav
        else if (reader->name() == QLatin1String("force_wall"))
            readForceWall(reader, modelindex); // for gcfm, gompertz, Tordeux2015, gradnav
        else if (reader->name() == QLatin1String("anti_clipping"))
            readAntiClipping(reader); // for gradnav
        else if (reader->name() == QLatin1String("floorfield"))
            readFloorfield(reader); // for in gradnav
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readSlover(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("solver"));
    QString solver = reader->readElementText();

    switch(modelindex)
    {
        case 1:
            ui->lineEdit_model_gcfm_01->setText(solver);
            break;
        case 2:
            ui->lineEdit_model_gompertz_01->setText(solver);
            break;
        case 3:
            ui->lineEdit_model_tordeux_01->setText(solver);
            break;
        case 4:
            ui->lineEdit_model_gradnav_01->setText(solver);
            break;
        case 5:
            ui->lineEdit_model_krausz_01->setText(solver);
            break;
        default:
            break;
    }
}

void InifileWidget::readStepsize(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("stepsize"));
    QString stepsize = reader->readElementText();

    switch(modelindex)
    {
        case 1:
            ui->lineEdit_model_gcfm_02->setText(stepsize);
            break;
        case 2:
            ui->lineEdit_model_gompertz_02->setText(stepsize);
            break;
        case 3:
            ui->lineEdit_model_tordeux_02->setText(stepsize);
            break;
        case 4:
            ui->lineEdit_model_gradnav_02->setText(stepsize);
            break;
        case 5:
            ui->lineEdit_model_krausz_02->setText(stepsize);
            break;
        default:
            break;
    }
}

void InifileWidget::readExit(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("exit_crossing_strategy"));
    QString exit = reader->readElementText();

    switch(modelindex)
    {
        case 1:
            ui->lineEdit_model_gcfm_03->setText(exit);
            break;
        case 2:
            ui->lineEdit_model_gompertz_03->setText(exit);
            break;
        case 3:
            ui->lineEdit_model_tordeux_03->setText(exit);
            break;
        case 4:
            ui->lineEdit_model_gradnav_03->setText(exit);
            break;
        case 5:
            ui->lineEdit_model_krausz_03->setText(exit);
            break;
        default:
            break;
    }
}

void InifileWidget::readPeriodic(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("periodic"));
    QString periodic = reader->readElementText();

    switch(modelindex)
    {
        case 1:
            ui->lineEdit_model_gcfm_14->setText(periodic);
            break;
        case 2:
            ui->lineEdit_model_gompertz_12->setText(periodic);
            break;
        case 3:
            ui->lineEdit_model_tordeux_10->setText(periodic);
            break;
        case 4:
            ui->lineEdit_model_gradnav_16->setText(periodic);
            break;
        case 5:
            ui->lineEdit_model_krausz_06->setText(periodic);
            break;
        default:
            break;
    }
}

void InifileWidget::readLinkedcells(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("linkedcells"));

    QString enabled = reader->attributes().value("enabled").toString();
    QString cell_size = reader->attributes().value("cell_size").toString();

    switch(modelindex)
    {
        case 1:
        {
            ui->lineEdit_model_gcfm_04->setText(enabled);
            ui->lineEdit_model_gcfm_05->setText(cell_size);
            break;
        }
        case 2:
        {
            ui->lineEdit_model_gompertz_04->setText(enabled);
            ui->lineEdit_model_gompertz_05->setText(cell_size);
            break;
        }
        case 3:
        {
            ui->lineEdit_model_tordeux_04->setText(enabled);
            ui->lineEdit_model_tordeux_05->setText(cell_size);
            break;
        }
        case 4:
        {
            ui->lineEdit_model_gradnav_04->setText(enabled);
            ui->lineEdit_model_gradnav_05->setText(cell_size);
            break;
        }
        case 5:
        {
            ui->lineEdit_model_krausz_04->setText(enabled);
            ui->lineEdit_model_krausz_05->setText(cell_size);
            break;
        }
        default:
        {
            break;
        }
    }

    reader->readNext();
}

void InifileWidget::readForcePed(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("force_ped"));

    switch(modelindex)
    {
        case 1:
        {
            QString nu_ped = reader->attributes().value("nu").toString();
            QString distance_ped = reader->attributes().value("dist_max").toString();
            QString distance_eff_ped = reader->attributes().value("disteff_max").toString();
            QString width_ped = reader->attributes().value("interpolation_width").toString();

            ui->lineEdit_model_gcfm_06->setText(nu_ped);
            ui->lineEdit_model_gcfm_07->setText(distance_ped);
            ui->lineEdit_model_gcfm_08->setText(distance_eff_ped);
            ui->lineEdit_model_gcfm_09->setText(width_ped);

            break;
        }
        case 2:
        {
            QString nu_ped = reader->attributes().value("nu").toString();
            QString b_ped = reader->attributes().value("b").toString();
            QString c_ped = reader->attributes().value("c").toString();

            ui->lineEdit_model_gompertz_06->setText(nu_ped);
            ui->lineEdit_model_gompertz_07->setText(b_ped);
            ui->lineEdit_model_gompertz_08->setText(c_ped);

            break;
        }
        case 3:
        {
            QString a_ped = reader->attributes().value("a").toString();
            QString D_ped = reader->attributes().value("D").toString();

            ui->lineEdit_model_tordeux_06->setText(a_ped);
            ui->lineEdit_model_tordeux_07->setText(D_ped);

            break;
        }
        case 4:
        {
            QString nu_ped = reader->attributes().value("nu").toString();
            QString b_ped = reader->attributes().value("b").toString();
            QString c_ped = reader->attributes().value("c").toString();

            ui->lineEdit_model_gradnav_10->setText(nu_ped);
            ui->lineEdit_model_gradnav_11->setText(b_ped);
            ui->lineEdit_model_gradnav_12->setText(c_ped);

            break;
        }
        case 5:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    reader->readNext();
}

void InifileWidget::readForceWall(QXmlStreamReader *reader, int modelindex)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("force_wall"));

    switch(modelindex)
    {
        case 1:
        {
            QString nu_wall = reader->attributes().value("nu").toString();
            QString distance_wall = reader->attributes().value("dist_max").toString();
            QString distance_eff_wall = reader->attributes().value("disteff_max").toString();
            QString width_wall = reader->attributes().value("interpolation_width").toString();

            ui->lineEdit_model_gcfm_10->setText(nu_wall);
            ui->lineEdit_model_gcfm_11->setText(distance_wall);
            ui->lineEdit_model_gcfm_12->setText(distance_eff_wall);
            ui->lineEdit_model_gcfm_13->setText(width_wall);

            break;
        }
        case 2:
        {
            QString nu_wall = reader->attributes().value("nu").toString();
            QString b_wall = reader->attributes().value("b").toString();
            QString c_wall = reader->attributes().value("c").toString();

            ui->lineEdit_model_gompertz_09->setText(nu_wall);
            ui->lineEdit_model_gompertz_10->setText(b_wall);
            ui->lineEdit_model_gompertz_11->setText(c_wall);

            break;
        }
        case 3:
        {
            QString a_wall = reader->attributes().value("a").toString();
            QString D_wall = reader->attributes().value("D").toString();

            ui->lineEdit_model_tordeux_08->setText(a_wall);
            ui->lineEdit_model_tordeux_09->setText(D_wall);

            break;
        }
        case 4:
        {
            QString nu_wall = reader->attributes().value("nu").toString();
            QString b_wall = reader->attributes().value("b").toString();
            QString c_wall = reader->attributes().value("c").toString();

            ui->lineEdit_model_gradnav_13->setText(nu_wall);
            ui->lineEdit_model_gradnav_14->setText(b_wall);
            ui->lineEdit_model_gradnav_15->setText(c_wall);

            break;
        }
        case 5:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    reader->readNext();
}

void InifileWidget::readAntiClipping(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("anti_clipping"));

    QString distance = reader->attributes().value("slow_down_distance").toString();
    ui->lineEdit_model_gradnav_09->setText(distance);

    reader->readNext();
}

void InifileWidget::readFloorfield(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("floorfield"));

    QString delta = reader->attributes().value("delta_h").toString();
    QString distance = reader->attributes().value("wall_avoid_distance").toString();
    QString avoidance = reader->attributes().value("use_wall_avoidance").toString();

    ui->lineEdit_model_gradnav_06->setText(delta);
    ui->lineEdit_model_gradnav_07->setText(distance);
    ui->lineEdit_model_gradnav_08->setText(avoidance);

    reader->readNext();
}

void InifileWidget::readAgentParamaters(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("agent_parameters"));

    QString id = reader->attributes().value("agent_parameter_id").toString();
    QTableWidgetItem *agentid = new QTableWidgetItem(tr("%1").arg(id));

    int row = ui->tableWidget_agents->rowCount();
    int row_index = row-1;

    ui->tableWidget_agents->setItem(row_index,0,agentid);

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("v0"))
            readV0(reader); // for all models
        else if (reader->name() == QLatin1String("v0_upstairs"))
            readV0Upstairs(reader); // for all models
        else if (reader->name() == QLatin1String("v0_downstairs"))
            readV0Downstars(reader); // for all models
        else if (reader->name() == QLatin1String("v0_idle_escalator_upstairs"))
            readV0IdleUpstairs(reader); // for all models
        else if (reader->name() == QLatin1String("v0_idle_escalator_downstairs"))
            readV0Idledownstairs(reader); // for all models
        else if (reader->name() == QLatin1String("bmax"))
            readBmax(reader); // for all models
        else if (reader->name() == QLatin1String("bmin"))
            readBmin(reader); // for all models
        else if (reader->name() == QLatin1String("amin"))
            readAmin(reader); // for all models
        else if (reader->name() == QLatin1String("tau"))
            readTau(reader);  // for all models
        else if (reader->name() == QLatin1String("atau"))
            readAtou(reader); // for all models
        else if (reader->name() == QLatin1String("T"))
            readT(reader); // for all models
        else if (reader->name() == QLatin1String("sway"))
            readSway(reader); // for krausz
        else
            reader->skipCurrentElement();
    }

    // add new row for next agent parameters
    ui->tableWidget_agents->setRowCount(row+1);

    reader->readNext();
}

void InifileWidget::readV0(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("v0"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *v0_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *v0_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,1, v0_mu);
    ui->tableWidget_agents->setItem(row_index,2, v0_sigma);

    reader->readNext();
}

void InifileWidget::readV0Upstairs(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("v0_upstairs"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *v0_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *v0_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,3, v0_mu);
    ui->tableWidget_agents->setItem(row_index,4, v0_sigma);

    reader->readNext();
}

void InifileWidget::readV0Downstars(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("v0_downstairs"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *v0_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *v0_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,5, v0_mu);
    ui->tableWidget_agents->setItem(row_index,6, v0_sigma);

    reader->readNext();
}

void InifileWidget::readV0IdleUpstairs(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("v0_idle_escalator_upstairs"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *v0_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *v0_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,7, v0_mu);
    ui->tableWidget_agents->setItem(row_index,8, v0_sigma);

    reader->readNext();
}

void InifileWidget::readV0Idledownstairs(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("v0_idle_escalator_downstairs"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *v0_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *v0_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,9, v0_mu);
    ui->tableWidget_agents->setItem(row_index,10, v0_sigma);

    reader->readNext();
}

void InifileWidget::readBmax(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("bmax"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *bmax_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *bmax_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,11, bmax_mu);
    ui->tableWidget_agents->setItem(row_index,12, bmax_sigma);

    reader->readNext();
}

void InifileWidget::readBmin(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("bmin"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *bmin_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *bmin_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,13, bmin_mu);
    ui->tableWidget_agents->setItem(row_index,14, bmin_sigma);

    reader->readNext();

}

void InifileWidget::readAmin(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("amin"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *amin_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *amin_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,15, amin_mu);
    ui->tableWidget_agents->setItem(row_index,16, amin_sigma);

    reader->readNext();

}

void InifileWidget::readTau(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("tau"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *tau_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *tau_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,17, tau_mu);
    ui->tableWidget_agents->setItem(row_index,18, tau_sigma);

    reader->readNext();

}

void InifileWidget::readAtou(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("atau"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *atau_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *atau_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,19, atau_mu);
    ui->tableWidget_agents->setItem(row_index,20, atau_sigma);


    reader->readNext();

}

void InifileWidget::readT(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("T"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString mu = reader->attributes().value("mu").toString();
    QString sigma = reader->attributes().value("sigma").toString();

    QTableWidgetItem *t_mu = new QTableWidgetItem(tr("%1").arg(mu));
    QTableWidgetItem *t_sigma = new QTableWidgetItem(tr("%1").arg(sigma));

    ui->tableWidget_agents->setItem(row_index,21, t_mu);
    ui->tableWidget_agents->setItem(row_index,22, t_sigma);

    reader->readNext();
}

void InifileWidget::readSway(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("sway"));

    int row_index = ui->tableWidget_agents->rowCount()-1;

    QString ampA = reader->attributes().value("ampA").toString();
    QString ampB = reader->attributes().value("ampB").toString();
    QString freqA = reader->attributes().value("freqA").toString();
    QString freqB = reader->attributes().value("freqB").toString();

    QTableWidgetItem *sway_ampA = new QTableWidgetItem(tr("%1").arg(ampA));
    QTableWidgetItem *sway_ampB = new QTableWidgetItem(tr("%1").arg(ampB));
    QTableWidgetItem *sway_freqA = new QTableWidgetItem(tr("%1").arg(freqA));
    QTableWidgetItem *sway_freqB = new QTableWidgetItem(tr("%1").arg(freqB));

    ui->tableWidget_agents->setItem(row_index,24, sway_ampA);
    ui->tableWidget_agents->setItem(row_index,25, sway_ampB);
    ui->tableWidget_agents->setItem(row_index,26, sway_freqA);
    ui->tableWidget_agents->setItem(row_index,27, sway_freqB);

    reader->readNext();
}

void InifileWidget::readRouteChoiceModels(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("route_choice_models"));

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("router"))
            readRouter(reader);
        else
            reader->skipCurrentElement();
    }
}

void InifileWidget::readRouter(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("router"));

    QString router_id = reader->attributes().value("router_id").toString();
    QString description = reader->attributes().value("description").toString();

    if(description == "ff_global_shortest")
    {
        ui->checkBox_ff->setChecked(true);
        ui->lineEdit_ff_global_ID->setText(router_id);
    }
    else if(description == "global_shortest")
    {
        ui->checkBox_global->setChecked(true);
        ui->lineEdit_global_ID->setText(router_id);
    }
    else if(description == "ff_global_shortest_trips")
    {
        ui->checkBox_trips->setChecked(true);
        ui->lineEdit_trips_ID->setText(router_id);
    }

    while (reader->readNextStartElement())
    {
        if (reader->name() == QLatin1String("parameters"))
            readRouterParameters(reader);
        else
            reader->skipCurrentElement();
    }
}


void InifileWidget::readRouterParameters(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("parameters"));

    while (reader->readNextStartElement())
    {
        if (reader->name() == QLatin1String("navigation_lines"))
            readNavigationLines(reader);
        else if(reader->name() == QLatin1String("write_VTK_files"))
            readVTK(reader);
        else
            reader->skipCurrentElement();
    }

    reader->readElementText();
}

void InifileWidget::readNavigationLines(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("navigation_lines"));

    QString file = reader->attributes().value("file").toString();
    ui->lineEdit_global_navi->setText(file);

    reader->readNext();
}

void InifileWidget::readVTK(QXmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement() && reader->name() == QLatin1String("write_VTK_files"));

    QString status = reader->attributes().value("write_VTK_files").toString();
    ui->comboBox_ff_global->setCurrentText(status);

    reader->readNext();
}

/*
    Since v0.8.8
    Get geometry file name
 */

void InifileWidget::pushButton_GeomeryClicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose Geometry"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_general_07->setText(fileName);
}

void InifileWidget::pushButton_GoalClicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose goals"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_GoalFile->setText(fileName);
}

void InifileWidget::pushButton_SourceClicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose sources"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_SourceFile->setText(fileName);
}

void InifileWidget::pushButton_TrafficClicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose trffic"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_TrafficFile->setText(fileName);
}

void InifileWidget::pushButton_RoutingClicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Choose Routing"),""
            ,tr("XML-Files (*.xml)"));

    ui->lineEdit_global_navi->setText(fileName);
}

void InifileWidget::pushButton_chooseTemplateClicked() {
    qDebug("Enter InifileWidget::pushButton_chooseTemplateClicked");
    templateWidget = new TemplateWidget(this);
    templateWidget->show();
    qDebug("Leave InifileWidget::pushButton_chooseTemplateClicked");
}
