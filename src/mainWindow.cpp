/**
 * \file        mainWindow.cpp
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
 * This class is setting up the main window incl. all buttons and bars. It is the parent widget of all other widgets
 * (GraphicView, roomWidget, widgetLandmark).
 *
 **/


//mainWindow.cpp

#include "mainWindow.h"
#include "GraphicView.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QSettings>

MWindow :: MWindow()
{
    setupUi(this);
    //Signal/Slot
    //VBox= new QVBoxLayout;

    mview = new jpsGraphicsView(this);
    dmanager = new jpsDatamanager(this,mview);
    mview->SetDatamanager(dmanager);

    //Roomwidget
    rwidget=nullptr;
    //Landmarkwidget
    lwidget=nullptr;
    //Snapping Options
    snappingOptions=nullptr;

    //StaturBar

    length_edit = new QLineEdit();
    length_edit->setMaximumWidth(75);

    x_edit = new QLineEdit();
    x_edit->setMaximumWidth(45);

    y_edit = new QLineEdit();
    y_edit->setMaximumWidth(45);

    infoLabel= new QLabel();
    infoLabel->setMinimumWidth(135);

    label_x = new QLabel();
    label_x->setMinimumWidth(10);
    label_x->setText("X :");

    label_y = new QLabel();
    label_y->setMinimumWidth(10);
    label_y->setText("Y :");

    label1 = new QLabel();
    label1->setMinimumWidth(90);
    label1->setText("Length of Line :");

    label2 = new QLabel();
    label2->setMinimumWidth(300);
    label2->setText("[m]");


    //filename of saved project
    _filename="";

    //WindowTitle
    this->setWindowTitle("JPSeditor");

    setCentralWidget(mview);
    //this->setMaximumSize(1920,1080);
    this->showMaximized();

    statusBar()->addPermanentWidget(infoLabel);
    statusBar()->addPermanentWidget(label_x);
    statusBar()->addPermanentWidget(x_edit);
    statusBar()->addPermanentWidget(label_y);
    statusBar()->addPermanentWidget(y_edit);
//    statusBar()->addPermanentWidget(label1);
//    statusBar()->addPermanentWidget(length_edit);
//    statusBar()->addPermanentWidget(label2);

    //Timer needed for autosaving function
    // timer will trigger autosave every 5th minute
    timer = new QTimer(this);
    timer->setInterval(60000);
    timer->start();

    _cMapTimer = new QTimer(this);
    //_cMapTimer=nullptr;

    _statScale=false;

    //Signals and Slots
    // Tab File
    connect(actionBeenden, SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(action_ffnen,SIGNAL(triggered(bool)),this,SLOT(openFileDXF()));
    connect(action_ffnen_xml,SIGNAL(triggered(bool)),this,SLOT(openFileXML()));
    connect(action_ffnen_cogmap,SIGNAL(triggered(bool)),this,SLOT(openFileCogMap()));
    connect(actionSpeichern,SIGNAL(triggered(bool)),this,SLOT(saveAsXML()));
    connect(actionSpeichern_dxf,SIGNAL(triggered(bool)),this,SLOT(saveAsDXF()));
    connect(actionSettings,SIGNAL(triggered(bool)),this,SLOT(Settings()));
    //connect(action_ffnen_CogMap,SIGNAL(triggered(bool)),this,SLOT(openFileCMap()));
    // Tab Help
    connect(action_ber,SIGNAL(triggered(bool)),this,SLOT(info()));
    // Tab Tools
    connect(actionanglesnap,SIGNAL(triggered(bool)),this,SLOT(anglesnap()));
    connect(actiongridmode,SIGNAL(triggered(bool)),this,SLOT(gridmode()));
    connect(actionWall,SIGNAL(triggered(bool)),this,SLOT(en_disableWall()));
    connect(actionDoor,SIGNAL(triggered(bool)),this,SLOT(en_disableDoor()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(en_disableExit()));
    connect(actionHLine,SIGNAL(triggered(bool)),this,SLOT(en_disableHLine()));
    connect(actionObjectsnap,SIGNAL(triggered(bool)),this,SLOT(objectsnap()));
    connect(actionDelete_lines,SIGNAL(triggered(bool)),this,SLOT(delete_lines()));
    connect(actionDelete_single_line,SIGNAL(triggered(bool)),this,SLOT(delete_marked_lines()));
    connect(actionRoom,SIGNAL(triggered(bool)),this,SLOT(define_room()));
    connect(actionAuto_Definition,SIGNAL(triggered(bool)),this,SLOT(autoDefine_room()));
    connect(actionSelect_Mode,SIGNAL(triggered(bool)),this,SLOT(en_selectMode()));
    connect(actionWall,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    connect(actionDoor,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    connect(actionScale,SIGNAL(triggered(bool)),this,SLOT(enableScale()));
    // Tab View
    connect(actionRotate_90_deg_clockwise,SIGNAL(triggered(bool)),this,SLOT(rotate()));
    connect(actionShow_Point_of_Origin,SIGNAL(triggered(bool)),this,SLOT(ShowOrigin()));


    // Length edit
//    connect(length_edit,SIGNAL(returnPressed()),this,SLOT(send_length()));
//    connect(length_edit,SIGNAL(returnPressed()),this,SLOT(ScaleLines()));
    // X Y edit
    connect(x_edit,SIGNAL(returnPressed()),this,SLOT(send_xy()));
    connect(y_edit,SIGNAL(returnPressed()),this,SLOT(send_xy()));

    // mview
    connect(mview,SIGNAL(no_drawing()),this,SLOT(en_selectMode()));
    connect(mview,SIGNAL(remove_marked_lines()),this,SLOT(lines_deleted()));
    connect(mview,SIGNAL(remove_all()),this,SLOT(remove_all_lines()));
    connect(mview,SIGNAL(set_focus_textedit()),length_edit,SLOT(setFocus()));
    connect(mview,SIGNAL(mouse_moved()),this,SLOT(show_coords()));
    connect(mview,SIGNAL(LineLengthChanged()),this,SLOT(ShowLineLength()));

//    QAction *str_escape = new QAction(this);
//    str_escape->setShortcut(Qt::Key_Escape);
//    connect(str_escape, SIGNAL(triggered(bool)), mview, SLOT(disableDrawing()));

    // Mark all lines
    QAction *str_a = new QAction(this);
    str_a->setShortcut(Qt::Key_A | Qt::CTRL);
    connect(str_a, SIGNAL(triggered(bool)), mview, SLOT(SelectAllLines()));
    this->addAction(str_a);
    QAction *str_del = new QAction(this);
    str_del->setShortcut(Qt::Key_D | Qt::CTRL);
    connect(str_del,SIGNAL(triggered(bool)),this,SLOT(remove_all_lines()));
    //connect(mview,SIGNAL(DoubleClick()),this,SLOT(en_selectMode()));
    // Autosave
    connect(timer, SIGNAL(timeout()), this, SLOT(AutoSave()));
    //Landmarks
    connect(actionLandmark,SIGNAL(triggered(bool)),this,SLOT(en_disableLandmark()));
    connect(actionLandmark,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    // Landmark specifications
    connect(actionLandmarkWidget,SIGNAL(triggered(bool)),this,SLOT(define_landmark()));
    //CMap
//    connect(actionRun_visualisation,SIGNAL(triggered(bool)),this,SLOT(RunCMap()));
//    connect(_cMapTimer,SIGNAL(timeout()),this,SLOT(UpdateCMap()));
//    connect(actionSpeichern_cogmap,SIGNAL(triggered()),this,SLOT(SaveCogMapXML()));
    //Undo Redo
    connect(actionUndo,SIGNAL(triggered(bool)),mview,SLOT(Undo()));
    connect(actionRedo,SIGNAL(triggered(bool)),mview,SLOT(Redo()));

    // room type data gathering
    connect(actionGather_data,SIGNAL(triggered(bool)),this, SLOT(GatherData()));
}

MWindow::~MWindow()
{
    delete mview;
    delete dmanager;
    delete length_edit;
    delete label1;
    delete label2;
    delete infoLabel;
    delete timer;
    delete _cMapTimer;
}

void MWindow::AutoSave()
{
    QMap<QString, QString> settingsmap = loadSettings();
    QString backupfolder = settingsmap["backupfolder"];

    QString filename = backupfolder + "/backup_untitled.xml";
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //QString coord_string=mview->build_coordString();

        dmanager->writeXML(file);
        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("Backup file generated!"), 10000);

        //routing (hlines)
        QString fileNameRouting = file.fileName();
        fileNameRouting = fileNameRouting.split(".").first() + "_routing.xml";
        QFile routingFile(fileNameRouting);
        if (routingFile.open(QIODevice::WriteOnly | QIODevice::Text))
            dmanager->writeRoutingXML(routingFile);
    }
}

void MWindow::GatherData()
{
    if (rwidget==nullptr)
    {
        rwidget = new roomWidget(this,this->dmanager,this->mview);
        //rwidget->setGeometry(QRect(QPoint(5,75), rwidget->size()));
        rwidget->setAttribute(Qt::WA_DeleteOnClose);
        rwidget->GatherRTData();
        rwidget->close();
        rwidget=nullptr;
        actionRoom->setChecked(false);
        //rwidget->show();

    }
    else
    {
        rwidget->GatherRTData();
    }

    statusBar()->showMessage(tr("Data gathered"),10000);
}


//void MWindow::RunCMap()
//{

//    double frameRate = dmanager->GetCMapFrameRate();
//    _cMapFrame=1;
//    if (frameRate==0)
//    {
//        statusBar()->showMessage(tr("No cognitive map has been loaded!"),10000);
//        return;
//    }
//    _cMapTimer->setInterval(1/frameRate*1000);
//    _cMapTimer->start();
//}

//void MWindow::UpdateCMap()
//{
//    _cMapFrame++;
//    if (_cMapFrame>dmanager->GetLastCMapFrame())
//    {
//        _cMapTimer->stop();
//        dmanager->ShowCMapFrame(1);
//        return;
//    }
//    dmanager->ShowCMapFrame(_cMapFrame);
//}


void MWindow::Settings()
{
    settingDialog = new SettingDialog;

//    QString backupfolder = settings.value("backupfolder").toString();
//    QString interval =  settings.value("interval").toString();
//
//    qDebug()<< settings.value("backupfolder");
//    qDebug()<< settings.value("interval");
//
//    QMap<QString, QString> defaultsetting;
//    defaultsetting["backupfolder"] = backupfolder;
//    defaultsetting["interval"] = interval;

    QMap<QString, QString> defaultsetting = loadSettings();
    connect(settingDialog,SIGNAL(sendSetting(QMap<QString, QString>)),
            this,SLOT(saveSettings(QMap<QString, QString>)));
    settingDialog->setCurrentSetting(defaultsetting);

    settingDialog->setModal(true);
    settingDialog->exec();
}

void MWindow::ShowOrigin()
{
    mview->ShowOrigin();
}

void MWindow::openFileDXF(){

    QString fileName=QFileDialog::getOpenFileName(this,tr("Open DXF"),"",tr("DXF-Drawings (*.dxf)"));
    //QFile file(fileName);
    std::string fName= fileName.toStdString();

    if (!dmanager->readDXF(fName))
    {
        statusBar()->showMessage("DXF-File could not be parsed!",10000);
    }
    //if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
      //  textEdit->setPlainText(QString::fromUtf8(file.readAll()));
     //   statusBar()->showMessage(tr("Datei erfolgreich geladen"),5000);
    //}
    else
    {

        statusBar()->showMessage("DXF-File successfully loaded!",10000);
    }
}

void MWindow::openFileXML()
{
    if(rwidget!=nullptr)
    {
        rwidget->close();
        rwidget=nullptr;
        actionRoom->setChecked(false);
    }

    QString fileName=QFileDialog::getOpenFileName(this,tr("Open XML"),"",tr("XML-Files (*.xml)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    //RoutingFile
    QString fileNameRouting= fileName.split(".").first()+"_routing.xml";
    QFile fileRouting(fileNameRouting);
    bool statusFileRouting=true;
    if (!fileRouting.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        statusFileRouting=false;

    }

    if (!dmanager->readXML(file))
    {
        QMessageBox::critical(this,
                              "OpenFileXML",
                              "Couldn't open xml-file",
                              QMessageBox::Ok);
        statusBar()->showMessage("XML-File could not be parsed!",10000);
    }



    else
    {
        //optional: load routing file
        if (statusFileRouting==true)
            dmanager->readRoutingXML(fileRouting);

        //AutoZoom to drawing
        mview->AutoZoom();
        statusBar()->showMessage("XML-File successfully loaded!",10000);
    }
    file.close();

}

void MWindow::openFileCogMap()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Open XML"),"",tr("XML-Files (*.xml)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,
                              "OpenFileXML",
                              "Couldn't open xml-file",
                              QMessageBox::Ok);
        return;
    }


    if (!dmanager->ParseCogMap(file))
    {
        statusBar()->showMessage("XML-File could not be parsed!",10000);
    }

    else
    {

        statusBar()->showMessage("Cognitive map successfully loaded!",10000);
    }
    file.close();
}

void MWindow::OpenLineFile()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Open Lines"),"",tr("txt-File (*.txt)"));
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        statusBar()->showMessage("Line-File could not be opened!",10000);
        return;

    }

    if (!dmanager->ReadLineFile(file))
    {
        statusBar()->showMessage("Line-File could not be parsed!",10000);
    }
    //if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
      //  textEdit->setPlainText(QString::fromUtf8(file.readAll()));
     //   statusBar()->showMessage(tr("Datei erfolgreich geladen"),5000);
    //}
    else
    {

        statusBar()->showMessage("Line-File successfully loaded!",10000);
    }
    file.close();
}

void MWindow::saveAsXML(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save XML"),"",tr("XML-Files (*.xml)"));
    _filename=fileName;
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    //QString fileNameLines=fileName.split(".").first()+"_lines.xml";

    //QFile LinesFile(fileNameLines);
    //if (LinesFile.open(QIODevice::WriteOnly|QIODevice::Text))
    //    dmanager->writeLineItems(LinesFile);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //QString coord_string=mview->build_coordString();

//        QString message = dmanager->check_printAbility();

//        if (message!="")
//        {
//            statusBar()->showMessage(message,10000);
//            QMessageBox::warning(this,"Warning!", message,
//                                 QMessageBox::Ok);
//            return;
//        }

        dmanager->writeXML(file);

        //routing (hlines)
        QString fileNameRouting=fileName.split(".").first()+"_routing.xml";

        QFile routingFile(fileNameRouting);
        if (routingFile.open(QIODevice::WriteOnly|QIODevice::Text))
            dmanager->writeRoutingXML(routingFile);



        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("XML-File successfully saved!"),10000);
    }
}


void MWindow::saveAsDXF()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save DXF"),"",tr("DXF-Drawings (*.dxf)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //QString coord_string=mview->build_coordString();
        std::string fName= fileName.toStdString();
        dmanager->writeDXF(fName);
        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("DXF-File successfully saved!"),10000);
    }
}

void MWindow::SaveCogMapXML()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save CognitiveMap XML"),"",tr("XML-Files (*.xml)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        dmanager->WriteCognitiveMapXML(file);
        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("XML-File successfully saved!"),10000);
    }
}


void MWindow::info(){
    /*
     * JPSeditor version information
     * */
    QString info = "\
    JPSeditor (version 0.8.4) is a tool\n\
    to create and process geometries for\n\
    JuPedSim.\n\
    2018. All rights reserved.";

    QMessageBox messageBox;
    messageBox.information(nullptr,tr("About..."),info);
}

void MWindow::anglesnap()
{
   mview->change_stat_anglesnap();
}

void MWindow::en_disableWall()
{
    this->disableDrawing();
    actionWall->setChecked(true);
    mview->en_disableWall();

}

void MWindow::en_disableDoor()
{
    this->disableDrawing();
    actionDoor->setChecked(true);
    mview->en_disableDoor();

}

void MWindow::en_disableExit()
{
    this->disableDrawing();
    actionExit->setChecked(true);
    mview->en_disableExit();
}

void MWindow::en_disableLandmark()
{
    this->disableDrawing();
    actionLandmark->setChecked(true);
    mview->en_disableLandmark();
}

void MWindow::en_disableHLine()
{
    this->disableDrawing();
    actionHLine->setChecked(true);
    mview->en_disableHLine();
}

void MWindow::disableDrawing()
{
    this->actionWall->setChecked(false);
    this->actionDoor->setChecked(false);
    this->actionExit->setChecked(false);
    this->actionLandmark->setChecked(false);
    this->actionHLine->setChecked(false);
    this->actionCopy->setChecked(false);
}

void MWindow::objectsnap()
{

    if(snappingOptions==nullptr)
    {
        snappingOptions = new SnappingOptions(this);
        snappingOptions->setGeometry(QRect(QPoint(5,75), snappingOptions->size()));
        snappingOptions->setAttribute(Qt::WA_DeleteOnClose);
        snappingOptions->show();

        connect(snappingOptions,SIGNAL(snapStart_endpoint(bool)),mview,SLOT(changeStart_endpoint(bool)));
        connect(snappingOptions,SIGNAL(snapIntersections_point(bool)),mview,SLOT(changeIntersections_point(bool)));
    }
    else
    {
        snappingOptions->close();
        snappingOptions=nullptr;
        actionObjectsnap->setChecked(false);
    }
    mview->change_objectsnap();
}

void MWindow::gridmode()
{
    mview->change_gridmode();
}

void MWindow::show_coords()
{
    QPointF point = mview->return_Pos();
    QString string = "";
    string.sprintf("(%.2f, %5.2f)", point.x(), point.y());
    infoLabel->setText(string);
}

void MWindow::delete_lines()
{
    mview->delete_all();
    statusBar()->showMessage(tr("All lines are deleted!"),10000);
}

void MWindow::delete_marked_lines()
{
    mview->delete_marked_lines();
    mview->delete_landmark();
    statusBar()->showMessage(tr("Marked lines are deleted!"),10000);
}

void MWindow::send_length()
{

    qreal length = length_edit->text().toDouble();
    if(length != 0)
    {
         mview->take_l_from_lineEdit(length);
    }
    length_edit->clear();
}

void MWindow::send_xy()
{
    qreal x = x_edit->text().toDouble();
    qreal y = x_edit->text().toDouble();

    QPointF endpoint;
    endpoint.setX(x);
    endpoint.setY(y);

    mview->take_endpoint_from_xyEdit(endpoint);

    x_edit->clear();
}


void MWindow::define_room()
{

    if (rwidget==nullptr)
    {
        rwidget = new roomWidget(this,this->dmanager,this->mview);
        rwidget->setGeometry(QRect(QPoint(5,75), rwidget->size()));
        rwidget->setAttribute(Qt::WA_DeleteOnClose);
        rwidget->show();

    }
    else
    {
        rwidget->close();
        rwidget=nullptr;
        actionRoom->setChecked(false);
    }
}

void MWindow::autoDefine_room()
{
    if (rwidget==nullptr)
    {
        rwidget = new roomWidget(this,this->dmanager,this->mview);
        //rwidget->setGeometry(QRect(QPoint(5,75), rwidget->size()));
        rwidget->setAttribute(Qt::WA_DeleteOnClose);
        rwidget->StartAutoDef();
        rwidget->close();
        rwidget=nullptr;
        actionRoom->setChecked(false);
        //rwidget->show();

    }
    else
    {
        rwidget->StartAutoDef();
    }

    statusBar()->showMessage(tr("Rooms and doors are set."),10000);
}

void MWindow::define_landmark()
{
    if (lwidget==nullptr)
    {
        lwidget = new widgetLandmark(this,this->dmanager,this->mview);
        lwidget->setGeometry(QRect(QPoint(5,75), lwidget->size()));
        lwidget->setAttribute(Qt::WA_DeleteOnClose);
        lwidget->show();
    }
    else
    {
        lwidget->close();
        lwidget=nullptr;
        actionLandmarkWidget->setChecked(false);
    }
}

void MWindow::en_selectMode()
{
    mview->disable_drawing();

    actionSelect_Mode->setChecked(true);

    actionWall->setChecked(false);
    actionDoor->setChecked(false);
    actionExit->setChecked(false);
    actionHLine->setChecked(false);
    actionLandmark->setChecked(false);
    actionCopy->setChecked(false);
    length_edit->clearFocus();
}

void MWindow::dis_selectMode()
{
    if (actionWall->isChecked()==true || actionDoor->isChecked()==true || actionExit->isChecked()==true
            || actionLandmark->isChecked()==true)
    {
        actionSelect_Mode->setChecked(false);
    }
}

void MWindow::lines_deleted()
{
    dmanager->remove_marked_lines();
}

void MWindow::remove_all_lines()
{
    dmanager->remove_all();
}

void MWindow::ShowLineLength()
{
    length_edit->setText(QString::number(mview->ReturnLineLength()));
    length_edit->selectAll();
}

void MWindow::ScaleLines()
{
    if (_statScale)
    {
        qreal factor = length_edit->text().toDouble();
        mview->ScaleLines(factor);
        length_edit->clear();
        _statScale=false;
    }
}

void MWindow::enableScale()
{
    _statScale=true;
}

void MWindow::rotate()
{
    mview->rotate(-90);
}

void MWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(
                this, "Quit?",
                "Do you really want to quit?",
                QMessageBox::Yes | QMessageBox::No );

    if (ret == QMessageBox::Yes)
    {
        QMainWindow::closeEvent(event);
    }
    else
    {
        event->ignore();
    }
}


void MWindow::on_actionCopy_triggered()
{
    actionCopy->setChecked(true);
    mview->start_Copy_function();
}

void MWindow::on_actionOnline_Help_triggered()
{
    QString JPSeditor = "http://www.jupedsim.org/jpseditor/";
    QDesktopServices::openUrl(QUrl(JPSeditor));
}

void MWindow::on_actionClear_all_Rooms_and_Doors_triggered()
{
    dmanager->remove_all();

    if(rwidget!= nullptr){
        rwidget->show_rooms();
        rwidget->showLayersInfo();
        rwidget->show_crossings();
        rwidget->show_obstacles();
    }

    statusBar()->showMessage(tr("All rooms and doors are cleared!"),10000);
}

void MWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            mview->disable_drawing();
            en_selectMode();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

// Default settings
void MWindow::saveSettings(QMap<QString, QString> settingsmap)
{
    QSettings settings("FZJ","JPSeditor");
    settings.beginGroup("backup");
    settings.setValue("backupfolder", settingsmap["backupfolder"]);
    settings.setValue("interval", settingsmap["interval"]);

    timer->setInterval(settingsmap["interval"].toInt());

    settings.endGroup();
}

QMap<QString, QString> MWindow::loadSettings()
{
    QSettings settings("FZJ","JPSeditor");

    settings.beginGroup("backup");
    QString value = settings.value("backupfolder", "../").toString();
    QString interval = settings.value("interval", "60000").toString();
    settings.endGroup();

    QMap<QString, QString> settingsmap;
    settingsmap["backupfolder"] = value;
    settingsmap["interval"] = interval;

    return settingsmap;
}

void MWindow::on_actionNew_Inifile_triggered()
{
    inifileWidget = new InifileWidget(this);
    inifileWidget->show();
    qDebug()<< "MWindow::on_actionNew_Inifile_triggered(): inifile widget is showed!";
}

void MWindow::on_actionBack_to_Origin_triggered()
{
    mview->centerOn(QPointF(0.0,0.0)); //TODO: Ensure in any situation
}

void MWindow::on_actionZoom_Windows_triggered()
{
    en_selectMode();
    mview->selectedWindows();
}

void MWindow::on_actionZoom_Extents_triggered()
{
    mview->AutoZoom();
}
