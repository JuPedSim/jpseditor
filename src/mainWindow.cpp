//mainWindow.cpp

#include "mainWindow.h"
#include "GraphicView.h"
#include "roomwidget.h"


#include <QFileDialog>
#include <QMessageBox>

MWindow :: MWindow() {

    setupUi(this);
    //Signal/Slot
    //VBox= new QVBoxLayout;

    dmanager = new jpsDatamanager(this);
    mview = new jpsGraphicsView(this,dmanager);

    rwidget=0L;

    length_edit = new QLineEdit();
    length_edit->setMaximumWidth(55);
    label1 = new QLabel();
    label1->setMinimumWidth(90);
    label1->setText("Length of Line :");
    label2 = new QLabel();
    label2->setMinimumWidth(300);

    setCentralWidget(mview);
    //this->setMaximumSize(1920,1080);
    this->showMaximized();
    statusBar()->addPermanentWidget(label1);
    statusBar()->addPermanentWidget(length_edit);
    statusBar()->addPermanentWidget(label2);


    //Signals and Slots
    connect(actionBeenden, SIGNAL(triggered(bool)),qApp,SLOT(quit()));
    connect(action_ffnen,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(actionSpeichern,SIGNAL(triggered(bool)),this,SLOT(saveFile()));
    connect(action_ber,SIGNAL(triggered(bool)),this,SLOT(info()));
    connect(actiongridmode,SIGNAL(triggered(bool)),this,SLOT(gridmode()));
    connect(actionWall,SIGNAL(triggered(bool)),this,SLOT(en_disableWall()));
    connect(actionDoor,SIGNAL(triggered(bool)),this,SLOT(en_disableDoor()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(en_disableExit()));
    connect(actionObjectsnap,SIGNAL(triggered(bool)),this,SLOT(objectsnap()));
    connect(mview,SIGNAL(mouse_moved()),this,SLOT(show_coords()));
    connect(actionDelete_lines,SIGNAL(triggered(bool)),this,SLOT(delete_lines()));
    connect(actionDelete_single_line,SIGNAL(triggered(bool)),this,SLOT(delete_marked_lines()));
    connect(mview,SIGNAL(set_focus_textedit()),length_edit,SLOT(setFocus()));
    connect(length_edit,SIGNAL(returnPressed()),this,SLOT(send_length()));
    connect(actionRoom,SIGNAL(triggered(bool)),this,SLOT(define_room()));


}

void MWindow::openFile(){

    QString fileName=QFileDialog::getOpenFileName(this);
    QFile file(fileName);
    /*if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        textEdit->setPlainText(QString::fromUtf8(file.readAll()));
        statusBar()->showMessage(tr("Datei erfolgreich geladen"),5000);
    }*/
}

void MWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //QString coord_string=mview->build_coordString();
        const qreal gl_x=mview->get_gl_min_x();
        const qreal gl_y=mview->get_gl_min_y();
        dmanager->writeXML(file,gl_x,gl_y);
        //file.write(coord_string.toUtf8());//textEdit->toPlainText().toUtf8());
        statusBar()->showMessage(tr("Datei erfolgreich gespeichert"),5000);
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
    mview->en_disableWall();
    this->actionDoor->setChecked(false);
    this->actionExit->setChecked(false);
}

void MWindow::en_disableDoor()
{
    mview->en_disableDoor();
    this->actionWall->setChecked(false);
    this->actionExit->setChecked(false);

}

void MWindow::en_disableExit()
{
    mview->en_disableExit();
    this->actionWall->setChecked(false);
    this->actionDoor->setChecked(false);
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
    statusBar()->showMessage(string);
}

void MWindow::delete_lines()
{
    mview->delete_all();
}

void MWindow::delete_marked_lines()
{
    mview->delete_marked_lines();
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













