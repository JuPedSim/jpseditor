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

    dataManager = dmanager;

    // set InifileWidget as a Window
    setWindowFlag(Qt::Dialog);

    // Set model gompertz invisivle
    ui->tabWidget->removeTab(4);

    // Set agents gompertz invisivle
    ui->tabWidget->removeTab(4);

    // Set model tordeux invisivle
    ui->tabWidget->removeTab(4);

    // Set agents tordeux invisivle
    ui->tabWidget->removeTab(4);

    // Set model gradnav invisivle
    ui->tabWidget->removeTab(4);

    // Set agents gradnav invisivle
    ui->tabWidget->removeTab(4);

    // Set model krausz invisivle
    ui->tabWidget->removeTab(4);

    // Set agents krausz invisivle
    ui->tabWidget->removeTab(4);

    // Set contraints invisivle
    // ui->tabWidget->removeTab(4);

    // Set goals invisivle
    // ui->tabWidget->removeTab(4);

    //signals and slots connection
    ui->lineEdit_general_07->setReadOnly(true);
    ui->lineEdit_GoalFile->setReadOnly(true);
    ui->lineEdit_SourceFile->setReadOnly(true);
    ui->lineEdit_TrafficFile->setReadOnly(true);

    connect(ui->pushButton_Geometry, SIGNAL(clicked(bool)), this, SLOT(pushButton_GeomeryClicked()));
    connect(ui->pushButton_Goal, SIGNAL(clicked(bool)), this, SLOT(pushButton_GoalClicked()));
    connect(ui->pushButton_Source, SIGNAL(clicked(bool)), this, SLOT(pushButton_SourceClicked()));
    connect(ui->pushButton_Traffic, SIGNAL(clicked(bool)), this, SLOT(pushButton_TrafficClicked()));
    connect(ui->pushButton_Routing, SIGNAL(clicked(bool)), this, SLOT(pushButton_RoutingClicked()));
}

InifileWidget::~InifileWidget()
{
    delete ui;
}

// Add rows to tablewidgets by inserting a number into a spinbox

void InifileWidget::on_spinBox_groups_1_valueChanged(int)
{
    ui->tableWidget_groups_1->setRowCount(ui->spinBox_groups_1->value());
}

// Add rows to tablewidgets by inserting a number into a spinbox
void InifileWidget::on_spinBox_agents_gcfm_1_valueChanged(int)
{
    ui->tableWidget_agents_gcfm_1->setRowCount(ui->spinBox_agents_gcfm_1->value());
}

// Add rows to tablewidgets by inserting a number into a spinbox
void InifileWidget::on_spinBox_agents_gompertz_1_valueChanged(int)
{
    ui->tableWidget_agents_gompertz_1->setRowCount(ui->spinBox_agents_gompertz_1->value());
}

// Add rows to tablewidgets by inserting a number into a spinbox
void InifileWidget::on_spinBox_agents_tordeux_1_valueChanged(int)
{
    ui->tableWidget_agents_tordeux_1->setRowCount(ui->spinBox_agents_tordeux_1->value());
}

// Add rows to tablewidgets by inserting a number into a spinbox
void InifileWidget::on_spinBox_agents_gradnav_1_valueChanged(int)
{
    ui->tableWidget_agents_gradnav_1->setRowCount(ui->spinBox_agents_gradnav_1->value());
}

// Add rows to tablewidgets by inserting a number into a spinbox
void InifileWidget::on_spinBox_agents_krausz_1_valueChanged(int)
{
    ui->tableWidget_agents_krausz_1->setRowCount(ui->spinBox_agents_krausz_1->value());
}

// Set models and agents visible or invisible
void InifileWidget::on_comboBox_groups_1_currentIndexChanged(int index)
{
    // Set gcfm visible
    if (index+1 == 1)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gcfm, "Model Gcfm");
        ui->tabWidget->insertTab(3, ui->tab_agents_gcfm, "Agents Gcfm");
    }

    // Set gompertz visible
    if (index+1 == 2)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gompertz, "Model Gompertz");
        ui->tabWidget->insertTab(3, ui->tab_agents_gompertz, "Agents Gompertz");
    }

    // Set tordeux visible
    if (index+1 == 3)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_tordeux, "Model Tordeux");
        ui->tabWidget->insertTab(3, ui->tab_agents_tordeux, "Agents Tordeux");
    }

    // Set gradnav visible
    if (index+1 == 4)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_gradnav, "Model Gradnav");
        ui->tabWidget->insertTab(3, ui->tab_agents_gradnav, "Agents Gradnav");
    }

    // Set krausz visible
    if (index+1 == 5)
    {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->insertTab(2, ui->tab_model_krausz, "Model Krausz");
        ui->tabWidget->insertTab(3, ui->tab_agents_krausz, "Agents Krausz");
    }
}

bool InifileWidget::CheckHeaderData()
{
    if(ui->lineEdit_general_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_07->text() == "")
    {
        QMessageBox::critical(this,
                              "Saving Inifile",
                              "Please chooses geometry file",
                              QMessageBox::Ok);
        return 0;
    }

    if(ui->lineEdit_general_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_10->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    if(ui->lineEdit_general_12->text() == "")
    {
        ui->label_warning->setText("<font color='red'>General incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckTrafficData()
{
    if(ui->lineEdit_TrafficFile->text().isEmpty())
    {
        QMessageBox::critical(this,
                              "Save Inifile",
                              "Please choose traffic file!",
                              QMessageBox::Ok);
        return false;
    }
    return true;
}

bool InifileWidget::CheckRoutingData()
{
    if(ui->lineEdit_GoalFile->text().isEmpty())
    {
        QMessageBox::critical(this,
                              "Save Inifile",
                              "Please choose goal file!",
                              QMessageBox::Ok);
        return false;
    }
    return true;
}

bool InifileWidget::CheckSourceData()
{
    if(ui->lineEdit_SourceFile->text().isEmpty())
    {
        QMessageBox::critical(this,
                              "Save Inifile",
                              "Please choose source file!",
                              QMessageBox::Ok);
        return false;
    }
    return true;
}

bool InifileWidget::CheckAgentData()
{
    for(int i = 0; i < ui->spinBox_groups_1->value(); i++)
    {
        for(int j = 0; j < 15; j++)
        {
            if (!ui->tableWidget_groups_1->item(i,j) || ui->tableWidget_groups_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Groups incomplete.</font>");
                return 0;
            }
        }
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckModelGcfmData()
{
    if(ui->lineEdit_model_gcfm_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_04->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_07->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_09->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_10->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_11->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_12->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gcfm_13->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gcfm incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckAgentGcfmData()
{
    for(int i = 0; i < ui->spinBox_agents_gcfm_1->value(); i++)
    {
        for(int j = 0; j < 21; j++)
        {
            if (!ui->tableWidget_agents_gcfm_1->item(i,j) || ui->tableWidget_agents_gcfm_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Agents Gcfm incomplete.</font>");
                return 0;
            }
        }
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckModelGompData()
{
    if(ui->lineEdit_model_gompertz_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_04->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_07->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_09->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_10->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gompertz_11->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gompertz incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckAgentGompData()
{
    for(int i = 0; i < ui->spinBox_agents_gompertz_1->value(); i++)
    {
        for(int j = 0; j < 21; j++)
        {
            if (!ui->tableWidget_agents_gompertz_1->item(i,j) || ui->tableWidget_agents_gompertz_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Agents Gompertz incomplete.</font>");
                return 0;
            }
        }
    }
    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckModelTordData()
{
    if(ui->lineEdit_model_tordeux_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_04->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_07->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_tordeux_09->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Tordeux incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckAgentTordData()
{
    for(int i = 0; i < ui->spinBox_agents_tordeux_1->value(); i++)
    {
        for(int j = 0; j < 23; j++)
        {
            if (!ui->tableWidget_agents_tordeux_1->item(i,j) || ui->tableWidget_agents_tordeux_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Agents Tordeux incomplete.</font>");
                return 0;
            }
        }
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckModelGradData()
{
    if(ui->lineEdit_model_gradnav_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_04->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_07->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_09->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_10->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_11->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_12->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_13->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_14->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_gradnav_15->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Gradnav incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckAgentGradData()
{
    for(int i = 0; i < ui->spinBox_agents_gradnav_1->value(); i++)
    {
        for(int j = 0; j < 21; j++)
        {
            if (!ui->tableWidget_agents_gradnav_1->item(i,j) || ui->tableWidget_agents_gradnav_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Agents Gradnav incomplete.</font>");
                return 0;
            }
        }
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckModelKrauData()
{
    if(ui->lineEdit_model_krausz_01->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_02->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_03->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_04->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_05->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_06->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_07->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_08->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_09->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_10->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_11->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_12->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }
    if(ui->lineEdit_model_krausz_13->text() == "")
    {
        ui->label_warning->setText("<font color='red'>Model Krausz incomplete.</font>");
        return 0;
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckAgentKrauData()
{
    for(int i = 0; i < ui->spinBox_agents_krausz_1->value(); i++)
    {
        for(int j = 0; j < 25; j++)
        {
            if (!ui->tableWidget_agents_krausz_1->item(i,j) || ui->tableWidget_agents_krausz_1->item(i,j)->text().isEmpty())
            {
                ui->label_warning->setText("<font color='red'>Agents Krausz incomplete.</font>");
                return 0;
            }
        }
    }

    ui->label_warning->setText("");
    return 1;
}

bool InifileWidget::CheckRouteChoiceData()
{
//    if(ui->lineEdit_route_choice_01->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_02->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_03->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_04->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_05->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_06->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_07->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_08->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_09->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_10->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_11->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_12->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//    if(ui->lineEdit_route_choice_13->text() == "")
//    {
//        ui->label_warning->setText("<font color='red'>Route choice incomplete.</font>");
//        return 0;
//    }
//
//    ui->label_warning->setText("");
    return 1;
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
    stream->writeCharacters(ui->lineEdit_general_03->text());
    stream->writeEndElement();

    stream->writeComment("seed used for initialising random generator");
    stream->writeStartElement("seed");
    stream->writeCharacters(ui->lineEdit_general_05->text());
    stream->writeEndElement();

    stream->writeComment("geometry file");
    stream->writeStartElement("geometry");
    stream->writeCharacters(ui->lineEdit_general_07->text().split("/").last());
    stream->writeEndElement();

    stream->writeComment("simulationtime");
    stream->writeStartElement("max_sim_time");
    stream->writeCharacters(ui->lineEdit_general_06->text());
    stream->writeEndElement();

    stream->writeComment("trajectories file and format");
    stream->writeStartElement("trajectories");
    stream->writeAttribute("format",ui->comboBox_general_02->currentText());
    stream->writeAttribute("fps", ui->lineEdit_general_10->text());
    stream->writeAttribute("color_mode",ui->comboBox_general_03->currentText());
    stream->writeStartElement("file");
    stream->writeAttribute("location", ui->lineEdit_general_12->text());
    stream->writeEndElement(); //end file
    stream->writeEndElement(); //end trajectories

    stream->writeComment("where to store the logs");
    stream->writeStartElement("logfile");
    stream->writeCharacters(ui->lineEdit_general_08->text());
    stream->writeEndElement();

    QString head_line_19 = "\t<!-- statistics -->\n";

    QString head_line_20 = "\t<show_statistics>" +
            ui->comboBox_general_01->currentText() +
            "</show_statistics>\n";

    stream->writeComment("statistics");
    stream->writeStartElement("show_statistics");
    stream->writeCharacters(ui->comboBox_general_01->currentText());
    stream->writeEndElement();

    return;
}

void InifileWidget::writeAgentData(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeComment("persons information and distribution");
    stream->writeStartElement("agents");
    stream->writeAttribute("operational_model_id", QString::number(ui->comboBox_groups_1->currentIndex() + 1));
    stream->writeStartElement("agents_distribution");

    for(int i = 0; i < ui->spinBox_groups_1->value(); i++)
    {
        stream->writeStartElement("group");
        stream->writeAttribute("group_id", ui->tableWidget_groups_1->item(i,0)->text());
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_groups_1->item(i,1)->text());
        stream->writeAttribute("room_id",ui->tableWidget_groups_1->item(i,2)->text());
        stream->writeAttribute("subroom_id",ui->tableWidget_groups_1->item(i,3)->text());
        stream->writeAttribute("number",ui->tableWidget_groups_1->item(i,4)->text());
        stream->writeAttribute("goal_id",ui->tableWidget_groups_1->item(i,5)->text());
        stream->writeAttribute("router_id",ui->tableWidget_groups_1->item(i,6)->text());
        stream->writeAttribute("x_min",ui->tableWidget_groups_1->item(i,7)->text());
        stream->writeAttribute("x_max",ui->tableWidget_groups_1->item(i,8)->text());
        stream->writeAttribute("y_min",ui->tableWidget_groups_1->item(i,9)->text());
        stream->writeAttribute("y_max",ui->tableWidget_groups_1->item(i,10)->text());
        stream->writeAttribute("pre_movement_mean",ui->tableWidget_groups_1->item(i,11)->text());
        stream->writeAttribute("pre_movement_sigma",ui->tableWidget_groups_1->item(i,12)->text());
        stream->writeAttribute("risk_tolerance_mean",ui->tableWidget_groups_1->item(i,13)->text());
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
    stream->writeCharacters(source_FileName);
    stream->writeEndElement(); //end files
    stream->writeEndElement(); //end agents_sources
    stream->writeEndElement(); //end agents

    return;
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
    stream->writeTextElement("solver", ui->lineEdit_model_gcfm_01->text());
    stream->writeTextElement("stepsize", ui->lineEdit_model_gcfm_02->text());
    stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gcfm_03->text());

    stream->writeStartElement("linkedcells");
    stream->writeAttribute("enabled", ui->lineEdit_model_gcfm_04->text());
    stream->writeAttribute("cell_size", ui->lineEdit_model_gcfm_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    stream->writeAttribute("nu", ui->lineEdit_model_gcfm_06->text());
    stream->writeAttribute("dist_max", ui->lineEdit_model_gcfm_07->text());
    stream->writeAttribute("desteff_max", ui->lineEdit_model_gcfm_08->text());
    stream->writeAttribute("interpolation_width", ui->lineEdit_model_gcfm_09->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    stream->writeAttribute("nu",ui->lineEdit_model_gcfm_10->text());
    stream->writeAttribute("dist_max", ui->lineEdit_model_gcfm_11->text());
    stream->writeAttribute("desteff_max", ui->lineEdit_model_gcfm_12->text());
    stream->writeAttribute("interpolation_width", ui->lineEdit_model_gcfm_13->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    for(int i = 0; i < ui->spinBox_agents_gcfm_1->value(); i++)
    {
        stream->writeStartElement("agent_parameters");
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents_gcfm_1->item(i,0)->text());

        stream->writeStartElement("v0");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,1)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,3)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,4)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,5)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,6)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,7)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,8)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,9)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,10)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmax");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,11)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,12)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,13)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,14)->text());
        stream->writeEndElement();

        stream->writeStartElement("amin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,15)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,16)->text());
        stream->writeEndElement();

        stream->writeStartElement("tau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,17)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gcfm_1->item(i,19)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gcfm_1->item(i,20)->text());
        stream->writeEndElement();

        stream->writeEndElement(); // end agent_parameters

    }

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model

    return ;
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
    stream->writeTextElement("solver", ui->lineEdit_model_gompertz_01->text());
    stream->writeTextElement("stepsize", ui->lineEdit_model_gompertz_02->text());
    stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gompertz_03->text());

    stream->writeStartElement("linkedcells");
    stream->writeAttribute("enabled", ui->lineEdit_model_gompertz_04->text());
    stream->writeAttribute("cell_size", ui->lineEdit_model_gompertz_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    stream->writeAttribute("nu", ui->lineEdit_model_gompertz_06->text());
    stream->writeAttribute("b", ui->lineEdit_model_gompertz_07->text());
    stream->writeAttribute("c", ui->lineEdit_model_gompertz_08->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    stream->writeAttribute("nu",ui->lineEdit_model_gompertz_09->text());
    stream->writeAttribute("b", ui->lineEdit_model_gompertz_10->text());
    stream->writeAttribute("c", ui->lineEdit_model_gompertz_11->text());
    stream->writeEndElement(); //end force_wall

    stream->writeEndElement(); // end model_parameters

    for(int i = 0; i < ui->spinBox_agents_gompertz_1->value(); i++)
    {
        stream->writeStartElement("agent_parameters");
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents_gompertz_1->item(i,0)->text());

        stream->writeStartElement("v0");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,1)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,3)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,4)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,5)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,6)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,7)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,8)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,9)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,10)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmax");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,11)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,12)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,13)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,14)->text());
        stream->writeEndElement();

        stream->writeStartElement("amin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,15)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,16)->text());
        stream->writeEndElement();

        stream->writeStartElement("tau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,17)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gompertz_1->item(i,19)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gompertz_1->item(i,20)->text());
        stream->writeEndElement();

        stream->writeEndElement(); // end agent_parameters

    }

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model

    return ;
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
    stream->writeTextElement("solver", ui->lineEdit_model_tordeux_01->text());
    stream->writeTextElement("stepsize", ui->lineEdit_model_tordeux_02->text());
    stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_tordeux_03->text());

    stream->writeStartElement("linkedcells");
    stream->writeAttribute("enabled", ui->lineEdit_model_tordeux_04->text());
    stream->writeAttribute("cell_size", ui->lineEdit_model_tordeux_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    stream->writeAttribute("a", ui->lineEdit_model_tordeux_06->text());
    stream->writeAttribute("D", ui->lineEdit_model_tordeux_07->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    stream->writeAttribute("a",ui->lineEdit_model_tordeux_08->text());
    stream->writeAttribute("D", ui->lineEdit_model_tordeux_09->text());
    stream->writeEndElement(); //end force_wall
    stream->writeEndElement(); // end model_parameters

    for(int i = 0; i < ui->spinBox_agents_tordeux_1->value(); i++)
    {
        stream->writeStartElement("agent_parameters");
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents_tordeux_1->item(i,0)->text());

        stream->writeStartElement("v0");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,1)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,3)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,4)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,5)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,6)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,7)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,8)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,9)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,10)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmax");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,11)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,12)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmin");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,13)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,14)->text());
        stream->writeEndElement();

        stream->writeStartElement("amin");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,15)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,16)->text());
        stream->writeEndElement();

        stream->writeStartElement("tau");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,17)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,19)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,20)->text());
        stream->writeEndElement();

        stream->writeStartElement("T");
        stream->writeAttribute("mu", ui->tableWidget_agents_tordeux_1->item(i,21)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_tordeux_1->item(i,22)->text());
        stream->writeEndElement();

        stream->writeEndElement(); // end agent_parameters
    }

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model

    return ;
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
    stream->writeTextElement("solver", ui->lineEdit_model_gradnav_01->text());
    stream->writeTextElement("stepsize", ui->lineEdit_model_gradnav_02->text());
    stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_gradnav_03->text());

    stream->writeStartElement("linkedcells");
    stream->writeAttribute("enabled", ui->lineEdit_model_gradnav_04->text());
    stream->writeAttribute("cell_size", ui->lineEdit_model_gradnav_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("floorfield");
    stream->writeAttribute("delta_h", ui->lineEdit_model_gradnav_06->text());
    stream->writeAttribute("wall_avoid_distance", ui->lineEdit_model_gradnav_07->text());
    stream->writeAttribute("use_wall_avoidance", ui->lineEdit_model_gradnav_08->text());
    stream->writeEndElement(); //end floorfield

    stream->writeStartElement("anti_clipping");
    stream->writeAttribute("slow_down_distance", ui->lineEdit_model_gradnav_09->text());
    stream->writeEndElement(); //end anti_clipping

    stream->writeStartElement("force_ped");
    stream->writeAttribute("nu", ui->lineEdit_model_gradnav_10->text());
    stream->writeAttribute("b", ui->lineEdit_model_gradnav_11->text());
    stream->writeAttribute("c", ui->lineEdit_model_gradnav_12->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    stream->writeAttribute("nu", ui->lineEdit_model_gradnav_13->text());
    stream->writeAttribute("b", ui->lineEdit_model_gradnav_14->text());
    stream->writeAttribute("c", ui->lineEdit_model_gradnav_15->text());
    stream->writeEndElement(); //end force_wall
    stream->writeEndElement(); // end model_parameters

    for(int i = 0; i < ui->spinBox_agents_gradnav_1->value(); i++)
    {
        stream->writeStartElement("agent_parameters");
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents_gradnav_1->item(i,0)->text());

        stream->writeStartElement("v0");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,1)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,3)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,4)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,5)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,6)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,7)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,8)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,9)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,10)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmax");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,11)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,12)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,13)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,14)->text());
        stream->writeEndElement();

        stream->writeStartElement("amin");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,15)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,16)->text());
        stream->writeEndElement();

        stream->writeStartElement("tau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,17)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");
        stream->writeAttribute("mu", ui->tableWidget_agents_gradnav_1->item(i,19)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_gradnav_1->item(i,20)->text());
        stream->writeEndElement();

        stream->writeEndElement(); // end agent_parameters

    }

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model

    return ;
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
    stream->writeTextElement("solver", ui->lineEdit_model_krausz_01->text());
    stream->writeTextElement("stepsize", ui->lineEdit_model_krausz_02->text());
    stream->writeTextElement("exit_crossing_strategy", ui->lineEdit_model_krausz_03->text());

    stream->writeStartElement("linkedcells");
    stream->writeAttribute("enabled", ui->lineEdit_model_krausz_04->text());
    stream->writeAttribute("cell_size", ui->lineEdit_model_krausz_05->text());
    stream->writeEndElement(); //end linkedcells

    stream->writeStartElement("force_ped");
    stream->writeAttribute("nu", ui->lineEdit_model_krausz_06->text());
    stream->writeAttribute("dist_max", ui->lineEdit_model_krausz_07->text());
    stream->writeAttribute("desteff_max", ui->lineEdit_model_krausz_08->text());
    stream->writeAttribute("interpolation_width", ui->lineEdit_model_krausz_09->text());
    stream->writeEndElement(); //end force_ped

    stream->writeStartElement("force_wall");
    stream->writeAttribute("nu", ui->lineEdit_model_krausz_10->text());
    stream->writeAttribute("dist_max", ui->lineEdit_model_krausz_11->text());
    stream->writeAttribute("desteff_max", ui->lineEdit_model_krausz_12->text());
    stream->writeAttribute("interpolation_width", ui->lineEdit_model_krausz_13->text());
    stream->writeEndElement(); //end force_wall
    stream->writeEndElement(); // end model_parameters

    for(int i = 0; i < ui->spinBox_agents_krausz_1->value(); i++)
    {
        stream->writeStartElement("agent_parameters");
        stream->writeAttribute("agent_parameter_id", ui->tableWidget_agents_krausz_1->item(i,0)->text());

        stream->writeStartElement("v0");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,1)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,2)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,3)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,4)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,5)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,6)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_upstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,7)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,8)->text());
        stream->writeEndElement();

        stream->writeStartElement("v0_idle_escalator_downstairs");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,9)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,10)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmax");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,11)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,12)->text());
        stream->writeEndElement();

        stream->writeStartElement("bmin");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,13)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,14)->text());
        stream->writeEndElement();

        stream->writeStartElement("amin");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,15)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,16)->text());
        stream->writeEndElement();

        stream->writeStartElement("tau");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,17)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,18)->text());
        stream->writeEndElement();

        stream->writeStartElement("atau");
        stream->writeAttribute("mu", ui->tableWidget_agents_krausz_1->item(i,19)->text());
        stream->writeAttribute("sigma", ui->tableWidget_agents_krausz_1->item(i,20)->text());
        stream->writeEndElement();

        stream->writeStartElement("sway");
        stream->writeAttribute("ampA", ui->tableWidget_agents_krausz_1->item(i,21)->text());
        stream->writeAttribute("ampB", ui->tableWidget_agents_krausz_1->item(i,22)->text());
        stream->writeAttribute("freqA", ui->tableWidget_agents_krausz_1->item(i,23)->text());
        stream->writeAttribute("freqB", ui->tableWidget_agents_krausz_1->item(i,24)->text());
        stream->writeEndElement();

        stream->writeEndElement(); // end agent_parameters
    }

    stream->writeEndElement(); //end model
    stream->writeEndElement(); //end operational model

    return ;
}

void InifileWidget::writeRouteChoiceData(QXmlStreamWriter *stream, QFile &file)
{

    stream->writeComment("route chice model");
    stream->writeStartElement("route_choice_models");

    int router = ui->comboBox_Router->currentIndex();

    switch(router){
        case 0:
            writeFFGlobalShortestModel(stream, file);
            break;
        case 1:
            writeGlobalShortestModel(stream, file);
            break;
        case 2:
            writeCognitiveMap(stream, file);
            break;
        default:
            break;
    }

    stream->writeEndElement();

    return ;
}

void InifileWidget::writeFFGlobalShortestModel(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_ID->text());
    stream->writeAttribute("description","ff_global_shortest");

    if(ui->radioButton_VTK->isChecked())
        stream->writeTextElement("write_VTK_files", "true");
    else
        stream->writeTextElement("write_VTK_files", "false");

    stream->writeEndElement();
}

void InifileWidget::writeGlobalShortestModel(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_ID->text());
    stream->writeAttribute("description","global_shortest");

    stream->writeStartElement("parameters");
    stream->writeStartElement("navigation_lines");
    stream->writeAttribute("file", ui->lineEdit_route_choice_03->text());
    stream->writeEndElement(); //end navigation lines
    stream->writeEndElement(); //end patameters
    stream->writeEndElement(); //end router
}

void InifileWidget::writeCognitiveMap(QXmlStreamWriter *stream, QFile &file)
{
    stream->writeStartElement("router");
    stream->writeAttribute("router_id", ui->lineEdit_ID->text());
    stream->writeAttribute("description","cognitive_map");
    stream->writeStartElement("sensors");
    //TODO: finish sensors
    stream->writeEndElement();//end sensors

    stream->writeStartElement("cognitive_map");
    stream->writeAttribute("status", "complete");
    stream->writeEndElement(); //end cognitive map
    stream->writeEndElement(); //router
}

// Create ini.xml on button push
void InifileWidget::on_pushButton_write_clicked()
{
//    //check header (geometry)
//    if (CheckHeaderData() == 0)
//    {
//        return;
//    }
//
//    //goal
//    if (!CheckRoutingData())
//        return;
//
//    //source
//    if (!CheckSourceData())
//        return;
//
//    //traffic(door)
//    if (!CheckTrafficData())
//        return;
//
//    //check agents information and distribution
//    if (CheckAgentData() == 0)
//    {
//        return;
//    }
//
//    //check operational model - gcfm
//    if (CheckModelGcfmData() == 0)
//    {
//        return;
//    }
//
//    //check agent parameters - gcfm
//    if (CheckAgentGcfmData() == 0)
//    {
//        return;
//    }
//
//    //check operational model - gompertz
//    if (CheckModelGompData() == 0)
//    {
//        return;
//    }
//
//    //check agent parameters - gompertz
//    if (CheckAgentGompData() == 0)
//    {
//        return;
//    }
//
//    //check operational model - tordeux
//    if (CheckModelTordData() == 0)
//    {
//        return;
//    }
//
//    //check agent parameters - tordeux
//    if (CheckAgentTordData() == 0)
//    {
//        return;
//    }
//
//    //check operational model - gradnav
//    if (CheckModelGradData() == 0)
//    {
//        return;
//    }
//
//    //check agent parameters - gradnav
//    if (CheckAgentGradData() == 0)
//    {
//        return;
//    }
//
//    //check operational model - krausz
//    if (CheckModelKrauData() == 0)
//    {
//        return;
//    }
//
//    //check agent parameters - krausz
//    if (CheckAgentKrauData() == 0)
//    {
//        return;
//    }
//
//    //check route_choice_models
//    if (CheckRouteChoiceData() == 0)
//    {
//        return;
//    }
//
//    //header
//    QString head_lines = writeHeaderData();
//
//    //agents information and distribution
//    QString agen_lines = writeAgentData();
//
//    //operational model and agent parameters - gcfm
//    QString gcfm_lines = writeModelGcfmData();
//
//    //operational model and agent parameters - gompertz
//    QString gomp_lines = writeModelGompData();
//
//    //operational model and agent parameters - tordeux
//    QString tord_lines = writeModelTordData();
//
//    //operational model and agent parameters - gradnav
//    QString grad_lines = writeModelGradData();
//
//    //operational model and agent parameters - krausz
//    QString krau_lines = writeModelKrauData();
//
//    //route_choice_models
//    QString choi_lines = writeRouteChoiceData();
//
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

void InifileWidget::ReadJuPedSimData(TiXmlElement* JuPedSim)
{
    //JuPedSim
    if (JuPedSim->Attribute("project"))
    {
        QString value = JuPedSim->Attribute("project");
        ui->lineEdit_general_01->setText(value);
    }

    if (JuPedSim->Attribute("version"))
    {
        QString value = JuPedSim->Attribute("version");
        ui->lineEdit_general_02->setText(value);
    }
}

void InifileWidget::ReadHeaderData(TiXmlElement *JuPedSim)
{
    if (JuPedSim->FirstChild("num_threads") && !JuPedSim->FirstChild("num_threads")->NoChildren())
    {
        QString value = JuPedSim->FirstChild("num_threads")->FirstChild()->Value();
        ui->lineEdit_general_03->setText(value);
    }

    if (JuPedSim->FirstChild("seed") && !JuPedSim->FirstChild("seed")->NoChildren())
    {
        QString value = JuPedSim->FirstChild("seed")->FirstChild()->Value();
        ui->lineEdit_general_05->setText(value);
    }

    if (JuPedSim->FirstChild("max_sim_time") && !JuPedSim->FirstChild("max_sim_time")->NoChildren())
    {
        QString value = JuPedSim->FirstChild("max_sim_time")->FirstChild()->Value();
        ui->lineEdit_general_06->setText(value);
    }

    if (JuPedSim->FirstChild("geometry") && !JuPedSim->FirstChild("geometry")->NoChildren())
    {

        QString value = JuPedSim->FirstChild("geometry")->FirstChild()->Value();
        ui->lineEdit_general_07->setText(value);
    }

    if (JuPedSim->FirstChildElement("trajectories"))
    {
        if (JuPedSim->FirstChildElement("trajectories")->Attribute("format"))
        {
            QString value = JuPedSim->FirstChildElement("trajectories")->Attribute("format");
            if (value == "xml-plain")
            {
                ui->comboBox_general_02->setCurrentIndex(0);
            }
            if (value == "plain")
            {
                ui->comboBox_general_02->setCurrentIndex(1);
            }
        }

        if (JuPedSim->FirstChildElement("trajectories")->Attribute("fps"))
        {
            QString value = JuPedSim->FirstChildElement("trajectories")->Attribute("fps");
            ui->lineEdit_general_10->setText(value);
        }

        if (JuPedSim->FirstChildElement("trajectories")->Attribute("color_mode"))
        {
            QString value = JuPedSim->FirstChildElement("trajectories")->Attribute("color_mode");
            if (value == "velocity")
            {
                ui->comboBox_general_03->setCurrentIndex(0);
            }
            if (value == "spotlight")
            {
                ui->comboBox_general_03->setCurrentIndex(1);
            }
            if (value == "group")
            {
                ui->comboBox_general_03->setCurrentIndex(2);
            }
            if (value == "knowledge")
            {
                ui->comboBox_general_03->setCurrentIndex(3);
            }
            if (value == "router")
            {
                ui->comboBox_general_03->setCurrentIndex(4);
            }
            if (value == "final_goal")
            {
                ui->comboBox_general_03->setCurrentIndex(5);
            }
            if (value == "intermediate_goal")
            {
                ui->comboBox_general_03->setCurrentIndex(6);
            }
        }

        if (JuPedSim->FirstChild("trajectories")->FirstChildElement("file"))
        {
            if (JuPedSim->FirstChild("trajectories")->FirstChildElement("file")->Attribute("location"))
            {
                QString value = JuPedSim->FirstChild("trajectories")->FirstChildElement("file")->Attribute("location");
                ui->lineEdit_general_12->setText(value);
            }
        }
    }

    if (JuPedSim->FirstChild("logfile") && !JuPedSim->FirstChild("logfile")->NoChildren())
    {
        QString value = JuPedSim->FirstChild("logfile")->FirstChild()->Value();
        ui->lineEdit_general_08->setText(value);
    }

    if (JuPedSim->FirstChild("show_statistics"))
    {
        QString value = JuPedSim->FirstChild("show_statistics")->FirstChild()->Value();
        if (value == "false")
        {
            ui->comboBox_general_01->setCurrentIndex(0);
        }
        if (value == "true")
        {
            ui->comboBox_general_01->setCurrentIndex(1);
        }
    }
}


void InifileWidget::ReadAgentData(TiXmlElement *JuPedSim)
{
    //agents information and distribution
    if (JuPedSim->FirstChildElement("agents"))
    {
        if (JuPedSim->FirstChildElement("agents")->Attribute("operational_model_id"))
        {
            QString value = JuPedSim->FirstChildElement("agents")->Attribute("operational_model_id");
            ui->comboBox_groups_1->setCurrentIndex(value.toInt()-1);
        }

        if (JuPedSim->FirstChild("agents")->FirstChild("agents_distribution"))
        {
            if (JuPedSim->FirstChild("agents")->FirstChild("agents_distribution")->FirstChildElement("group"))
            {
                int counter = 0;
                for (TiXmlElement* group = JuPedSim->FirstChild("agents")->FirstChild("agents_distribution")->FirstChildElement("group");
                     group; group = group->NextSiblingElement("group"))
                {
                    ui->spinBox_groups_1->setValue(counter+1);

                    QString count = QString::number(counter);
                    //default_values
                    ui->tableWidget_groups_1->setItem(counter, 0, new QTableWidgetItem(count));
                    ui->tableWidget_groups_1->setItem(counter, 1, new QTableWidgetItem("1"));
                    ui->tableWidget_groups_1->setItem(counter, 2, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 3, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 4, new QTableWidgetItem("1"));
                    ui->tableWidget_groups_1->setItem(counter, 5, new QTableWidgetItem("-1"));
                    ui->tableWidget_groups_1->setItem(counter, 6, new QTableWidgetItem("1"));
                    ui->tableWidget_groups_1->setItem(counter, 7, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 8, new QTableWidgetItem("1"));
                    ui->tableWidget_groups_1->setItem(counter, 9, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 10, new QTableWidgetItem("1"));
                    ui->tableWidget_groups_1->setItem(counter, 11, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 12, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 13, new QTableWidgetItem("0"));
                    ui->tableWidget_groups_1->setItem(counter, 14, new QTableWidgetItem("0"));


                    if (group->Attribute("group_id"))
                    {
                        QString value = group->Attribute("group_id");
                        ui->tableWidget_groups_1->setItem(counter, 0, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("agent_parameter_id"))
                    {
                        QString value = group->Attribute("agent_parameter_id");
                        ui->tableWidget_groups_1->setItem(counter, 1, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("room_id"))
                    {
                        QString value = group->Attribute("room_id");
                        ui->tableWidget_groups_1->setItem(counter, 2, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("subroom_id"))
                    {
                        QString value = group->Attribute("subroom_id");
                        ui->tableWidget_groups_1->setItem(counter, 3, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("number"))
                    {
                        QString value = group->Attribute("number");
                        ui->tableWidget_groups_1->setItem(counter, 4, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("goal_id"))
                    {
                        QString value = group->Attribute("goal_id");
                        ui->tableWidget_groups_1->setItem(counter, 5, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("router_id"))
                    {
                        QString value = group->Attribute("router_id");
                        ui->tableWidget_groups_1->setItem(counter, 6, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("x_min"))
                    {
                        QString value = group->Attribute("x_min");
                        ui->tableWidget_groups_1->setItem(counter, 7, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("x_max"))
                    {
                        QString value = group->Attribute("x_max");
                        ui->tableWidget_groups_1->setItem(counter, 8, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("y_min"))
                    {
                        QString value = group->Attribute("y_min");
                        ui->tableWidget_groups_1->setItem(counter, 9, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("y_max"))
                    {
                        QString value = group->Attribute("y_max");
                        ui->tableWidget_groups_1->setItem(counter, 10, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("pre_movement_mean"))
                    {
                        QString value = group->Attribute("pre_movement_mean");
                        ui->tableWidget_groups_1->setItem(counter, 11, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("pre_movement_sigma"))
                    {
                        QString value = group->Attribute("pre_movement_sigma");
                        ui->tableWidget_groups_1->setItem(counter, 12, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("risk_tolerance_mean"))
                    {
                        QString value = group->Attribute("risk_tolerance_mean");
                        ui->tableWidget_groups_1->setItem(counter, 13, new QTableWidgetItem(value));
                    }
                    if (group->Attribute("risk_tolerance_sigma"))
                    {
                        QString value = group->Attribute("risk_tolerance_sigma");
                        ui->tableWidget_groups_1->setItem(counter, 14, new QTableWidgetItem(value));
                    }
                    counter = counter + 1;
                }
            }
        }
    }
}

void InifileWidget::ReadModelData(TiXmlElement *JuPedSim)
{
    //operational model and agent parameters
    if (JuPedSim->FirstChild("operational_models"))
    {
        if (JuPedSim->FirstChild("operational_models")->FirstChildElement("model"))
        {
            for (TiXmlElement* model = JuPedSim->FirstChild("operational_models")->FirstChildElement("model");
                 model; model = model->NextSiblingElement("model"))
            {
                QString desc = model->Attribute("description");
                if (desc == "gcfm")
                {
                    int index = 0;
                    on_comboBox_groups_1_currentIndexChanged(index);

                    if (model->FirstChild("model_parameters"))
                    {
                        if (model->FirstChild("model_parameters")->FirstChild("solver"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("solver")->FirstChild()->Value();
                            ui->lineEdit_model_gcfm_01->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("stepsize"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("stepsize")->FirstChild()->Value();
                            ui->lineEdit_model_gcfm_02->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy")->FirstChild()->Value();
                            ui->lineEdit_model_gcfm_03->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled");
                                ui->lineEdit_model_gcfm_04->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size");
                                ui->lineEdit_model_gcfm_05->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_ped"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu");
                                ui->lineEdit_model_gcfm_06->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("dist_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("dist_max");
                                ui->lineEdit_model_gcfm_07->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("desteff_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("desteff_max");
                                ui->lineEdit_model_gcfm_08->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("interpolation_width"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("interpolation_width");
                                ui->lineEdit_model_gcfm_09->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_wall"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu");
                                ui->lineEdit_model_gcfm_10->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("dist_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("dist_max");
                                ui->lineEdit_model_gcfm_11->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("desteff_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("desteff_max");
                                ui->lineEdit_model_gcfm_12->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("interpolation_width"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("interpolation_width");
                                ui->lineEdit_model_gcfm_13->setText(value);
                            }
                        }

                        if (model->FirstChildElement("agent_parameters"))
                        {
                            int counter = 0;
                            for (TiXmlElement* agent = model->FirstChildElement("agent_parameters");
                                 agent; agent = agent->NextSiblingElement("agent_parameters"))
                            {
                                ui->spinBox_agents_gcfm_1->setValue(counter+1);

                                QString count = QString::number(counter+1);
                                //default_values
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 0, new QTableWidgetItem(count));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 1, new QTableWidgetItem("1.33"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 2, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 3, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 4, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 5, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 6, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 7, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 8, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 9, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 10, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 11, new QTableWidgetItem("0.25"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 12, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 13, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 14, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 15, new QTableWidgetItem("0.18"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 16, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 17, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 18, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 19, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gcfm_1->setItem(counter, 20, new QTableWidgetItem("0.001"));

                                if (agent->Attribute("agent_parameter_id"))
                                {
                                    QString value = agent->Attribute("agent_parameter_id");
                                    ui->tableWidget_agents_gcfm_1->setItem(counter, 0, new QTableWidgetItem(value));
                                }
                                if (agent->FirstChildElement("v0"))
                                {
                                    if (agent->FirstChildElement("v0")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 1, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 2, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 3, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 4, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 5, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 6, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 7, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 8, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 9, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 10, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmax"))
                                {
                                    if (agent->FirstChildElement("bmax")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 11, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmax")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 12, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmin"))
                                {
                                    if (agent->FirstChildElement("bmin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 13, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 14, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("amin"))
                                {
                                    if (agent->FirstChildElement("amin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 15, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("amin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 16, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("tau"))
                                {
                                    if (agent->FirstChildElement("tau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 17, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("tau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 18, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("atau"))
                                {
                                    if (agent->FirstChildElement("atau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("mu");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 19, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("atau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("sigma");
                                        ui->tableWidget_agents_gcfm_1->setItem(counter, 20, new QTableWidgetItem(value));
                                    }
                                }
                                counter = counter + 1;
                            }
                        }
                    }
                }

                if (desc == "gompertz")
                {
                    int index = 1;
                    on_comboBox_groups_1_currentIndexChanged(index);
                    if (model->FirstChild("model_parameters"))
                    {
                        if (model->FirstChild("model_parameters")->FirstChild("solver"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("solver")->FirstChild()->Value();
                            ui->lineEdit_model_gompertz_01->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("stepsize"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("stepsize")->FirstChild()->Value();
                            ui->lineEdit_model_gompertz_02->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy")->FirstChild()->Value();
                            ui->lineEdit_model_gompertz_03->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled");
                                ui->lineEdit_model_gompertz_04->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size");
                                ui->lineEdit_model_gompertz_05->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_ped"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu");
                                ui->lineEdit_model_gompertz_06->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("b"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("b");
                                ui->lineEdit_model_gompertz_07->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("c"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("c");
                                ui->lineEdit_model_gompertz_08->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_wall"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu");
                                ui->lineEdit_model_gompertz_09->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("b"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("b");
                                ui->lineEdit_model_gompertz_10->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("c"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("c");
                                ui->lineEdit_model_gompertz_11->setText(value);
                            }
                        }

                        if (model->FirstChildElement("agent_parameters"))
                        {
                            int counter = 0;
                            for (TiXmlElement* agent = model->FirstChildElement("agent_parameters");
                                 agent; agent = agent->NextSiblingElement("agent_parameters"))
                            {
                                ui->spinBox_agents_gompertz_1->setValue(counter+1);

                                QString count = QString::number(counter+1);
                                //default_values
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 0, new QTableWidgetItem(count));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 1, new QTableWidgetItem("1.33"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 2, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 3, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 4, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 5, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 6, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 7, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 8, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 9, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 10, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 11, new QTableWidgetItem("0.25"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 12, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 13, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 14, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 15, new QTableWidgetItem("0.18"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 16, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 17, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 18, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 19, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gompertz_1->setItem(counter, 20, new QTableWidgetItem("0.001"));

                                if (agent->Attribute("agent_parameter_id"))
                                {
                                    QString value = agent->Attribute("agent_parameter_id");
                                    ui->tableWidget_agents_gompertz_1->setItem(counter, 0, new QTableWidgetItem(value));
                                }
                                if (agent->FirstChildElement("v0"))
                                {
                                    if (agent->FirstChildElement("v0")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 1, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 2, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 3, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 4, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 5, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 6, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 7, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 8, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 9, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 10, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmax"))
                                {
                                    if (agent->FirstChildElement("bmax")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 11, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmax")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 12, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmin"))
                                {
                                    if (agent->FirstChildElement("bmin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 13, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 14, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("amin"))
                                {
                                    if (agent->FirstChildElement("amin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 15, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("amin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 16, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("tau"))
                                {
                                    if (agent->FirstChildElement("tau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 17, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("tau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 18, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("atau"))
                                {
                                    if (agent->FirstChildElement("atau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("mu");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 19, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("atau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("sigma");
                                        ui->tableWidget_agents_gompertz_1->setItem(counter, 20, new QTableWidgetItem(value));
                                    }
                                }
                                counter = counter + 1;
                            }
                        }
                    }
                }

                if (desc == "Tordeux2015")
                {
                    int index = 2;
                    on_comboBox_groups_1_currentIndexChanged(index);
                    if (model->FirstChild("model_parameters"))
                    {
                        if (model->FirstChild("model_parameters")->FirstChild("solver"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("solver")->FirstChild()->Value();
                            ui->lineEdit_model_tordeux_01->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("stepsize"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("stepsize")->FirstChild()->Value();
                            ui->lineEdit_model_tordeux_02->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy")->FirstChild()->Value();
                            ui->lineEdit_model_tordeux_03->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled");
                                ui->lineEdit_model_tordeux_04->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size");
                                ui->lineEdit_model_tordeux_05->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_ped"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("a"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("a");
                                ui->lineEdit_model_tordeux_06->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("D"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("D");
                                ui->lineEdit_model_tordeux_07->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_wall"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("a"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("a");
                                ui->lineEdit_model_tordeux_08->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("D"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("D");
                                ui->lineEdit_model_tordeux_09->setText(value);
                            }
                        }

                        if (model->FirstChildElement("agent_parameters"))
                        {
                            int counter = 0;
                            for (TiXmlElement* agent = model->FirstChildElement("agent_parameters");
                                 agent; agent = agent->NextSiblingElement("agent_parameters"))
                            {
                                ui->spinBox_agents_tordeux_1->setValue(counter+1);

                                QString count = QString::number(counter+1);
                                //default_values
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 0, new QTableWidgetItem(count));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 1, new QTableWidgetItem("1.33"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 2, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 3, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 4, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 5, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 6, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 7, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 8, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 9, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 10, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 11, new QTableWidgetItem("0.15"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 12, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 13, new QTableWidgetItem("0.15"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 14, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 15, new QTableWidgetItem("0.15"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 16, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 17, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 18, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 19, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 20, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 21, new QTableWidgetItem("1.0"));
                                ui->tableWidget_agents_tordeux_1->setItem(counter, 22, new QTableWidgetItem("0.001"));

                                if (agent->Attribute("agent_parameter_id"))
                                {
                                    QString value = agent->Attribute("agent_parameter_id");
                                    ui->tableWidget_agents_tordeux_1->setItem(counter, 0, new QTableWidgetItem(value));
                                }
                                if (agent->FirstChildElement("v0"))
                                {
                                    if (agent->FirstChildElement("v0")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 1, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 2, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 3, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 4, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 5, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 6, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 7, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 8, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 9, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 10, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmax"))
                                {
                                    if (agent->FirstChildElement("bmax")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 11, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmax")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 12, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmin"))
                                {
                                    if (agent->FirstChildElement("bmin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 13, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 14, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("amin"))
                                {
                                    if (agent->FirstChildElement("amin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 15, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("amin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 16, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("tau"))
                                {
                                    if (agent->FirstChildElement("tau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 17, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("tau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 18, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("atau"))
                                {
                                    if (agent->FirstChildElement("atau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 19, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("atau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 20, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("T"))
                                {
                                    if (agent->FirstChildElement("T")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("T")->Attribute("mu");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 21, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("T")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("T")->Attribute("sigma");
                                        ui->tableWidget_agents_tordeux_1->setItem(counter, 22, new QTableWidgetItem(value));
                                    }
                                }
                                counter = counter + 1;
                            }
                        }
                    }
                }

                if (desc == "gradnav")
                {
                    int index = 3;
                    on_comboBox_groups_1_currentIndexChanged(index);
                    if (model->FirstChild("model_parameters"))
                    {
                        if (model->FirstChild("model_parameters")->FirstChild("solver"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("solver")->FirstChild()->Value();
                            ui->lineEdit_model_gradnav_01->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("stepsize"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("stepsize")->FirstChild()->Value();
                            ui->lineEdit_model_gradnav_02->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy")->FirstChild()->Value();
                            ui->lineEdit_model_gradnav_03->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled");
                                ui->lineEdit_model_gradnav_04->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size");
                                ui->lineEdit_model_gradnav_05->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("floorfield"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("delta_h"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("delta_h");
                                ui->lineEdit_model_gradnav_06->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("wall_avoid_distance"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("wall_avoid_distance");
                                ui->lineEdit_model_gradnav_07->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("use_wall_avoidance"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("floorfield")->Attribute("use_wall_avoidance");
                                ui->lineEdit_model_gradnav_08->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("anti_clipping"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("anti_clipping")->Attribute("slow_down_distance"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("anti_clipping")->Attribute("slow_down_distance");
                                ui->lineEdit_model_gradnav_09->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_ped"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu");
                                ui->lineEdit_model_gradnav_10->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("b"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("b");
                                ui->lineEdit_model_gradnav_11->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("c"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("c");
                                ui->lineEdit_model_gradnav_12->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_wall"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu");
                                ui->lineEdit_model_gradnav_13->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("b"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("b");
                                ui->lineEdit_model_gradnav_14->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("c"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("c");
                                ui->lineEdit_model_gradnav_15->setText(value);
                            }
                        }

                        if (model->FirstChildElement("agent_parameters"))
                        {
                            int counter = 0;
                            for (TiXmlElement* agent = model->FirstChildElement("agent_parameters");
                                 agent; agent = agent->NextSiblingElement("agent_parameters"))
                            {
                                ui->spinBox_agents_gradnav_1->setValue(counter+1);

                                QString count = QString::number(counter+1);
                                //default_values
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 0, new QTableWidgetItem(count));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 1, new QTableWidgetItem("1.33"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 2, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 3, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 4, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 5, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 6, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 7, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 8, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 9, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 10, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 11, new QTableWidgetItem("0.25"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 12, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 13, new QTableWidgetItem("0.20"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 14, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 15, new QTableWidgetItem("0.18"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 16, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 17, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 18, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 19, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_gradnav_1->setItem(counter, 20, new QTableWidgetItem("0.001"));

                                if (agent->Attribute("agent_parameter_id"))
                                {
                                    QString value = agent->Attribute("agent_parameter_id");
                                    ui->tableWidget_agents_gradnav_1->setItem(counter, 0, new QTableWidgetItem(value));
                                }
                                if (agent->FirstChildElement("v0"))
                                {
                                    if (agent->FirstChildElement("v0")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 1, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 2, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 3, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 4, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 5, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 6, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 7, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 8, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 9, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 10, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmax"))
                                {
                                    if (agent->FirstChildElement("bmax")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 11, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmax")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 12, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmin"))
                                {
                                    if (agent->FirstChildElement("bmin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 13, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 14, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("amin"))
                                {
                                    if (agent->FirstChildElement("amin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 15, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("amin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 16, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("tau"))
                                {
                                    if (agent->FirstChildElement("tau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 17, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("tau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 18, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("atau"))
                                {
                                    if (agent->FirstChildElement("atau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("mu");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 19, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("atau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("sigma");
                                        ui->tableWidget_agents_gradnav_1->setItem(counter, 20, new QTableWidgetItem(value));
                                    }
                                }
                                counter = counter + 1;
                            }
                        }
                    }
                }

                if (desc == "krausz")
                {
                    int index = 4;
                    ui->comboBox_groups_1->setCurrentIndex(index);
                    on_comboBox_groups_1_currentIndexChanged(index);
                    if (model->FirstChild("model_parameters"))
                    {
                        if (model->FirstChild("model_parameters")->FirstChild("solver"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("solver")->FirstChild()->Value();
                            ui->lineEdit_model_krausz_01->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("stepsize"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("stepsize")->FirstChild()->Value();
                            ui->lineEdit_model_krausz_02->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy"))
                        {
                            QString value = model->FirstChild("model_parameters")->FirstChild("exit_crossing_strategy")->FirstChild()->Value();
                            ui->lineEdit_model_krausz_03->setText(value);
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("enabled");
                                ui->lineEdit_model_krausz_04->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("linkedcells")->Attribute("cell_size");
                                ui->lineEdit_model_krausz_05->setText(value);
                            }

                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_ped"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("nu");
                                ui->lineEdit_model_krausz_06->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("dist_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("dist_max");
                                ui->lineEdit_model_krausz_07->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("desteff_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("desteff_max");
                                ui->lineEdit_model_krausz_08->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("interpolation_width"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_ped")->Attribute("interpolation_width");
                                ui->lineEdit_model_krausz_09->setText(value);
                            }
                        }
                        if (model->FirstChild("model_parameters")->FirstChildElement("force_wall"))
                        {
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("nu");
                                ui->lineEdit_model_krausz_10->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("dist_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("dist_max");
                                ui->lineEdit_model_krausz_11->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("desteff_max"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("desteff_max");
                                ui->lineEdit_model_krausz_12->setText(value);
                            }
                            if (model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("interpolation_width"))
                            {
                                QString value = model->FirstChild("model_parameters")->FirstChildElement("force_wall")->Attribute("interpolation_width");
                                ui->lineEdit_model_krausz_13->setText(value);
                            }
                        }

                        if (model->FirstChildElement("agent_parameters"))
                        {
                            int counter = 0;
                            for (TiXmlElement* agent = model->FirstChildElement("agent_parameters");
                                 agent; agent = agent->NextSiblingElement("agent_parameters"))
                            {
                                ui->spinBox_agents_krausz_1->setValue(counter+1);

                                QString count = QString::number(counter+1);
                                //default_values
                                ui->tableWidget_agents_krausz_1->setItem(counter, 0, new QTableWidgetItem(count));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 1, new QTableWidgetItem("1.33"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 2, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 3, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 4, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 5, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 6, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 7, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 8, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 9, new QTableWidgetItem("0.6"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 10, new QTableWidgetItem("0.2"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 11, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 12, new QTableWidgetItem("0.0"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 13, new QTableWidgetItem("0.20"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 14, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 15, new QTableWidgetItem("0.18"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 16, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 17, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 18, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 19, new QTableWidgetItem("0.5"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 20, new QTableWidgetItem("0.001"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 21, new QTableWidgetItem("-0.14"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 22, new QTableWidgetItem("0.21"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 23, new QTableWidgetItem("0.44"));
                                ui->tableWidget_agents_krausz_1->setItem(counter, 24, new QTableWidgetItem("0.25"));

                                if (agent->Attribute("agent_parameter_id"))
                                {
                                    QString value = agent->Attribute("agent_parameter_id");
                                    ui->tableWidget_agents_krausz_1->setItem(counter, 0, new QTableWidgetItem(value));
                                }
                                if (agent->FirstChildElement("v0"))
                                {
                                    if (agent->FirstChildElement("v0")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 1, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 2, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 3, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 4, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 5, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 6, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_upstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 7, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_upstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 8, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("v0_idle_escalator_downstairs"))
                                {
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 9, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("v0_idle_escalator_downstairs")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 10, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmax"))
                                {
                                    if (agent->FirstChildElement("bmax")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 11, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmax")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmax")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 12, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("bmin"))
                                {
                                    if (agent->FirstChildElement("bmin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 13, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("bmin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("bmin")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 14, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("amin"))
                                {
                                    if (agent->FirstChildElement("amin")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 15, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("amin")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("amin")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 16, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("tau"))
                                {
                                    if (agent->FirstChildElement("tau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 17, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("tau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("tau")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 18, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("atau"))
                                {
                                    if (agent->FirstChildElement("atau")->Attribute("mu"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("mu");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 19, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("atau")->Attribute("sigma"))
                                    {
                                        QString value = agent->FirstChildElement("atau")->Attribute("sigma");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 20, new QTableWidgetItem(value));
                                    }
                                }
                                if (agent->FirstChildElement("sway"))
                                {
                                    if (agent->FirstChildElement("sway")->Attribute("ampA"))
                                    {
                                        QString value = agent->FirstChildElement("sway")->Attribute("ampA");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 21, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("sway")->Attribute("ampB"))
                                    {
                                        QString value = agent->FirstChildElement("sway")->Attribute("ampB");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 22, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("sway")->Attribute("freqA"))
                                    {
                                        QString value = agent->FirstChildElement("sway")->Attribute("freqA");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 23, new QTableWidgetItem(value));
                                    }
                                    if (agent->FirstChildElement("sway")->Attribute("freqB"))
                                    {
                                        QString value = agent->FirstChildElement("sway")->Attribute("freqB");
                                        ui->tableWidget_agents_krausz_1->setItem(counter, 24, new QTableWidgetItem(value));
                                    }
                                }
                                counter = counter + 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void InifileWidget::ReadRouteChoiceData(TiXmlElement* JuPedSim)
{
    //route_choice_models
    if (JuPedSim->FirstChild("route_choice_models"))
    {
        if (JuPedSim->FirstChild("route_choice_models")->FirstChildElement("router"))
        {
            for (TiXmlElement* rout = JuPedSim->FirstChild("route_choice_models")->FirstChildElement("router");
                 rout; rout = rout->NextSiblingElement("router"))
            {
                QString desc = rout->Attribute("description");

                if (desc == "ff_global_shortest")
                {
                    if (rout->Attribute("router_id"))
                    {
                        QString value = rout->Attribute("router_id");
//                        ui->lineEdit_route_choice_01->setText(value);
                    }
                }
                if (desc == "global_shortest")
                {
                    if (rout->Attribute("router_id"))
                    {
                        QString value = rout->Attribute("router_id");
//                        ui->lineEdit_route_choice_02->setText(value);
                    }
                    if (rout->FirstChild("parameters"))
                    {
                        if (rout->FirstChild("parameters")->FirstChildElement("navigation_lines"))
                        {
                            if (rout->FirstChild("parameters")->FirstChildElement("navigation_lines")->Attribute("file"))
                            {
                                QString value = rout->FirstChild("parameters")->FirstChildElement("navigation_lines")->Attribute("file");
                                ui->lineEdit_route_choice_03->setText(value);
                            }
                        }
                    }
                }
                if (desc == "cognitive_map")
                {
                    QString value = rout->Attribute("router_id");
//                    ui->lineEdit_route_choice_04->setText(value);

                    int counter = 0;
                    for (TiXmlElement* sens = rout->FirstChild("sensors")->FirstChildElement("sensor");
                         sens; sens = sens->NextSiblingElement("sensor"))
                    {
                        if (counter == 0)
                        {
                            if (sens->Attribute("sensor_id"))
                            {
                                QString sensor = sens->Attribute("sensor_id");
                                ui->lineEdit_route_choice_05->setText(sensor);
                            }
                            if (sens->Attribute("description"))
                            {
                                QString value = sens->Attribute("description");
                                ui->lineEdit_route_choice_06->setText(value);
                            }
                        }
                        if (counter == 1)
                        {
                            if (sens->Attribute("sensor_id"))
                            {
                                QString sensor = sens->Attribute("sensor_id");
                                ui->lineEdit_route_choice_07->setText(sensor);
                            }
                            if (sens->Attribute("description"))
                            {
                                QString value = sens->Attribute("description");
                                ui->lineEdit_route_choice_08->setText(value);
                            }
                            if (sens->Attribute("p_field_path"))
                            {
                                QString value = sens->Attribute("p_field_path");
                                ui->lineEdit_route_choice_09->setText(value);
                            }
                            if (sens->Attribute("update_time"))
                            {
                                QString value = sens->Attribute("update_time");
                                ui->lineEdit_route_choice_10->setText(value);
                            }
                            if (sens->Attribute("final_time"))
                            {
                                QString value = sens->Attribute("final_time");
                                ui->lineEdit_route_choice_11->setText(value);
                            }
                        }
                        if (counter == 2)
                        {
                            if (sens->Attribute("sensor_id"))
                            {
                                QString sensor = sens->Attribute("sensor_id");
                                ui->lineEdit_route_choice_12->setText(sensor);
                            }
                            if (sens->Attribute("description"))
                            {
                                QString value = sens->Attribute("description");
                                ui->lineEdit_route_choice_13->setText(value);
                            }
                        }
                        counter = counter + 1;
                    }
                }
            }
        }
    }
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
            reader->raiseError(QObject::tr("The file is not an JuPedSim version 0.8 file."));
        }
    }

    return reader->error();
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
    ui->lineEdit_general_03->setText(version);

    while (reader->readNextStartElement()) {
        if (reader->name() == QLatin1String("seed"))
            readSeed(reader);
        else if (reader->name() == QLatin1String("geometry"))
            readGeometry(reader);
        else if (reader->name() == QLatin1String("max_sim_time"))
            readMaxSimTime(reader);
        else if (reader->name() == QLatin1String("trajectories"))
            readTrajectories(reader);
        else if (reader->name() == QLatin1String("logfile"))
            readLogfile(reader);
        else if (reader->name() == QLatin1String("traffic_constraints"))
            readTrafficConstraints(reader);
        else if (reader->name() == QLatin1String("routing"))
            readRouting(reader);
        else if (reader->name() == QLatin1String("agents"))
            readAgents(reader);
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
    //TODO: work later from here!
}

void InifileWidget::readGroup(QXmlStreamReader *reader)
{

}

void InifileWidget::readAgentsSources(QXmlStreamReader *reader)
{

}

void InifileWidget::readSourceFile(QXmlStreamReader *reader)
{

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

    ui->lineEdit_route_choice_03->setText(fileName);
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
        if(crossing->IsExit())
            doorlist.append(crossing);
    }

    dataManager->writeTraffics(stream, doorlist);

    auto traffic_FileName = ui->lineEdit_TrafficFile->text().split("/").last();
    stream->writeStartElement("file");
    stream->writeCharacters(traffic_FileName);
    stream->writeEndElement(); //end files
    stream->writeEndElement(); //end doors
    stream->writeEndElement(); //end traffic_constraints
}