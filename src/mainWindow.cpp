//mainWindow.cpp

#include "mainWindow.h"
#include "GraphicView.h"
#include <iostream>



#include <QFileDialog>
#include <QMessageBox>

MWindow :: MWindow() {

    setupUi(this);
    //Signal/Slot
    //VBox= new QVBoxLayout;


    mview = new jpsGraphicsView(this);
    dmanager = new jpsDatamanager(this,mview);

    ///Roomwidget
    rwidget=nullptr;
    ///Landmarkwidget
    lwidget=nullptr;

    length_edit = new QLineEdit();
    length_edit->setMaximumWidth(55);
    label1 = new QLabel();
    label1->setMinimumWidth(90);
    label1->setText("Length of Line :");
    label2 = new QLabel();
    label2->setMinimumWidth(300);
    label2->setText("[m]");
    infoLabel= new QLabel();
    infoLabel->setMinimumWidth(135);

    ///filename of saved project
    _filename="";


    setCentralWidget(mview);
    //this->setMaximumSize(1920,1080);
    this->showMaximized();
    statusBar()->addPermanentWidget(infoLabel);
    statusBar()->addPermanentWidget(label1);
    statusBar()->addPermanentWidget(length_edit);
    statusBar()->addPermanentWidget(label2);

    ///Timer needed for autosave function
    /// timer will trigger autosave every 5th minute
    timer = new QTimer(this);
    timer->setInterval(300000);
    timer->start();


    ///Signals and Slots
    /// Tab File
    connect(actionBeenden, SIGNAL(triggered(bool)),qApp,SLOT(quit()));
    connect(action_ffnen,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(action_ffnen_xml,SIGNAL(triggered(bool)),this,SLOT(openFileXML()));
    connect(actionSpeichern,SIGNAL(triggered(bool)),this,SLOT(saveFile()));
    connect(actionSpeichern_dxf,SIGNAL(triggered(bool)),this,SLOT(saveAsDXF()));
    /// Tab Help
    connect(action_ber,SIGNAL(triggered(bool)),this,SLOT(info()));
    /// Tab Tools
    connect(actiongridmode,SIGNAL(triggered(bool)),this,SLOT(gridmode()));
    connect(actionWall,SIGNAL(triggered(bool)),this,SLOT(en_disableWall()));
    connect(actionDoor,SIGNAL(triggered(bool)),this,SLOT(en_disableDoor()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(en_disableExit()));
    connect(actionObjectsnap,SIGNAL(triggered(bool)),this,SLOT(objectsnap()));
    connect(actionDelete_lines,SIGNAL(triggered(bool)),this,SLOT(delete_lines()));
    connect(actionDelete_single_line,SIGNAL(triggered(bool)),this,SLOT(delete_marked_lines()));
    connect(actionRoom,SIGNAL(triggered(bool)),this,SLOT(define_room()));
    connect(actionSelect_Mode,SIGNAL(triggered(bool)),this,SLOT(en_selectMode()));
    connect(actionWall,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    connect(actionDoor,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    /// Tab View
    connect(actionRotate_90_deg_clockwise,SIGNAL(triggered(bool)),this,SLOT(rotate()));
    /// Length edit
    connect(length_edit,SIGNAL(returnPressed()),this,SLOT(send_length()));
    /// mview
    connect(mview,SIGNAL(no_drawing()),this,SLOT(en_selectMode()));
    connect(mview,SIGNAL(remove_marked_lines()),this,SLOT(lines_deleted()));
    connect(mview,SIGNAL(remove_all()),this,SLOT(remove_all_lines()));
    connect(mview,SIGNAL(set_focus_textedit()),length_edit,SLOT(setFocus()));
    connect(mview,SIGNAL(mouse_moved()),this,SLOT(show_coords()));
    connect(mview,SIGNAL(landmark_added()),this,SLOT(add_landmark()));
    connect(mview,SIGNAL(DoubleClick()),this,SLOT(en_selectMode()));
    ///Autosave
    connect(timer, SIGNAL(timeout()), this, SLOT(AutoSave()));
    ///Landmarks
    connect(actionLandmark,SIGNAL(triggered(bool)),this,SLOT(en_disableLandmark()));
    connect(actionLandmark,SIGNAL(triggered(bool)),this,SLOT(dis_selectMode()));
    ///Landmark specifications
    connect(actionLandmarkWidget,SIGNAL(triggered(bool)),this,SLOT(define_landmark()));

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
}

void MWindow::AutoSave()
{
    QFile file("Name");
    if (_filename!="")
    {
        QString fN="backup_"+_filename+".xml";
        file.setFileName(fN);
    }
    else
    {
        QString fN="backup_untitled.xml";
        file.setFileName(fN);
    }


    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //QString coord_string=mview->build_coordString();

        dmanager->writeXML(file);
        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("Backup file generated!"),10000);
    }
}

void MWindow::add_landmark()
{
    jpsLandmark* landmark = mview->get_landmarks().last();
    dmanager->new_landmark(landmark);
}

void MWindow::openFile(){

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


    if (!dmanager->readXML(file))
    {
        statusBar()->showMessage("XML-File could not be parsed!",10000);
    }

    else
    {

        statusBar()->showMessage("XML-File successfully loaded!",10000);
    }

}

void MWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save XML"),"",tr("XML-Files (*.xml)"));
    _filename=fileName;
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //QString coord_string=mview->build_coordString();

        QString message = dmanager->check_printAbility();

        if (message!="")
        {
            statusBar()->showMessage(message,10000);
            QMessageBox::warning(this,"Warning!", message,
                                 QMessageBox::Ok);
            return;
        }
        dmanager->writeXML(file);
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


void MWindow::info(){

    QMessageBox::information(
                0,tr("About..."),tr("jpsGui; Interface for generating and editing geometry(XML)-files necessary for using JuPedSim; Manual and tutorial coming soon!"));
}

void MWindow::gridmode()
{
   mview->change_gridmode();
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

void MWindow::disableDrawing()
{
    this->actionWall->setChecked(false);
    this->actionDoor->setChecked(false);
    this->actionExit->setChecked(false);
    this->actionLandmark->setChecked(false);
}

void MWindow::objectsnap()
{
    mview->change_objectsnap();
}

void MWindow::show_coords()
{
    QPointF point = mview->return_Pos();
    QString string = "x: ";
    string.append(QString::number(point.x()));
    string.append(" y: ");
    string.append(QString::number(point.y()));
    string.append(" [m]");
    infoLabel->setText(string);
}

void MWindow::delete_lines()
{
    mview->delete_all();
}

void MWindow::delete_marked_lines()
{
    mview->delete_marked_lines();
    mview->delete_landmark();
}

void MWindow::send_length()
{
    qreal length = length_edit->text().toFloat();
    mview->take_l_from_lineEdit(length);
    length_edit->clear();

}

void MWindow::define_room()
{
    if (rwidget==0L)
    {
        rwidget = new roomWidget(this,this->dmanager,this->mview);
        rwidget->setGeometry(QRect(QPoint(5,75), rwidget->size()));
        rwidget->show();
    }
    else
    {
        rwidget->close();
        rwidget=0L;
        actionRoom->setChecked(false);
    }
}

void MWindow::define_landmark()
{
    if (lwidget==0L)
    {
        lwidget = new widgetLandmark(this,this->dmanager,this->mview);
        lwidget->setGeometry(QRect(QPoint(5,75), lwidget->size()));
        lwidget->show();
    }
    else
    {
        lwidget->close();
        mview->ClearWaypoints();
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
    actionLandmark->setChecked(false);
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

void MWindow::rotate()
{
    mview->rotate(-90);
}













