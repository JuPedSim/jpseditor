#include "propertywidget.h"
#include "ui_propertywidget.h"

PropertyWidget::PropertyWidget(QWidget *parent, jpsDatamanager *dmanager,
                               JPSZone *zone, jpsGraphicsView *gview)
    : QWidget(parent), ui(new Ui::PropertyWidget)
{
    qDebug("Enter PropertyWidget::PropertyWidget");
    ui->setupUi(this);
    view = gview;
    data = dmanager;

    current_zone = zone;

    // Change layout depends on type
    if(zone != nullptr)
    {
        zoneType = zone->getType();
        updateWidget();
    }

    // Update list widget if line deleted
    connect(view, SIGNAL(markedLineDeleted()), this, SLOT(updateListwidget()));
    connect(view, SIGNAL(lines_deleted()),this, SLOT(updateListwidget()));

    // Set-up elevation
    ui->lineEdit_elevation->setText(QString::number(current_zone->get_elevation()));

    // For wall tab
    connect(ui->pushButton_addWall, SIGNAL(clicked()), this, SLOT(addWallButtonClicked()));
    connect(ui->pushButton_removeWall, SIGNAL(clicked()), this, SLOT(removeWallButtonClicked()));
    connect(ui->listWidget_walls, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem *)));

    // For track tab
    connect(ui->pushButton_addTrack, SIGNAL(clicked()), this, SLOT(addTrackButtonClicked()));
    connect(ui->pushButton_removeTrack, SIGNAL(clicked()), this, SLOT(removeTrackButtonClicked()));
    connect(ui->listWidget_track, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(highlightWall(QListWidgetItem*)));
    connect(ui->listWidget_track, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateType(QListWidgetItem*)));
    connect(ui->pushButton_applyType, SIGNAL(clicked()), this, SLOT(applyTypeButtonClicked()));

    // For property tab
    connect(ui->pushButton_applyProperty, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));

    qDebug("Leave PropertyWidget::PropertyWidget");
}

PropertyWidget::~PropertyWidget()
{
    qDebug("Enter PropertyWidget::~PropertyWidget");
    delete ui;
    qDebug("Leave PropertyWidget::~PropertyWidget");
}

void PropertyWidget::updateWidget()
{
    qDebug("Enter PropertyWidget::updateWidget");
    switch(zoneType)
    {
        case Room:
            ui->tabWidget->removeTab(1); // Remove track tab
            break;

        case Platform:
            ui->tabWidget->removeTab(0); // Remove wall tab
            break;

        case Stair:
            ui->tabWidget->removeTab(1); // Remove track tab
            ui->tabWidget->removeTab(1); // Remove property tab
            break;
        default:
            qDebug("Leave PropertyWidget::updateWidget");
            return;
    }

    updateListwidget();
    updatePropertyWidget();
    qDebug("Leave PropertyWidget::updateWidget");
}

void PropertyWidget::updateListwidget()
{
    qDebug("Enter PropertyWidget::updateListwidget");

    updateWallListWidget();
    updateTrackListWidget();

}

void PropertyWidget::updateWallListWidget()
{
    qDebug("Enter PropertyWidget::updateWallListWidget");
    ui->listWidget_walls->clear();

    if(current_zone == nullptr)
        return;

    QList<jpsLineItem *> walllist = current_zone->get_listWalls();

    if(walllist.isEmpty())
    {
        qDebug("Leave PropertyWidget::updateWallListWidget");
        return;
    }

    for (int i = 0; i < walllist.size(); i++)
    {
        if(walllist[i]->get_line() != nullptr)
        {
            QString string = "";
            string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                           walllist[i]->get_line()->line().x1(),
                           walllist[i]->get_line()->line().x2(),
                           walllist[i]->get_line()->line().y1(),
                           walllist[i]->get_line()->line().y2());

            ui->listWidget_walls->addItem(string);
        }
    }

    qDebug("Leave PropertyWidget::updateWallListWidget");
}

void PropertyWidget::addWallButtonClicked()
{
    qDebug("Enter PropertyWidget::addWallButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "wall")
                current_zone->addWall(line);
        }

        updateWallListWidget();
    }
    qDebug("Leave PropertyWidget::addWallButtonClicked");
}

void PropertyWidget::removeWallButtonClicked()
{
    qDebug("Enter PropertyWidget::removeWallButtonClicked");
    int row = ui->listWidget_walls->currentRow();

    if(row == -1) // There is no rows in list
        return;

    current_zone->removeWall(current_zone->get_listWalls()[row]);

    ui->listWidget_walls->setCurrentRow(-1); // Set no focus

    updateWallListWidget();
    qDebug("Leave PropertyWidget::removeWallButtonClicked");
}

void PropertyWidget::highlightWall(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::highlightWall");
    int wRow = ui->listWidget_walls->currentRow();
    int tRow = ui->listWidget_track->currentRow();
    int index = ui->tabWidget->currentIndex();

    if(ui->tabWidget->tabText(index) == "Wall")
    {
        auto *line= current_zone->get_listWalls()[wRow];
        view->unmark_all_lines();
        view->markLine(line);
    }
    else if(ui->tabWidget->tabText(index) == "Track")
    {
        auto *line = current_zone->getTrackList()[tRow];
        view->unmark_all_lines();
        view->markLine(line->getLine());
    }
    qDebug("Leave PropertyWidget::highlightWall");
}

void PropertyWidget::updateTrackListWidget()
{
    qDebug("Enter PropertyWidget::updateTrackListWidget");
    ui->listWidget_track->clear();

    if(current_zone == nullptr)
        return;

    QList<JPSTrack *> track_list = current_zone->getTrackList();

    if(track_list.isEmpty())
    {
        qDebug("Leave PropertyWidget::updateTrackListWidget");
        return;
    }

    for (int i = 0; i < track_list.size(); i++)
    {
        QString string = "";
        string.sprintf("[%+06.3f, %+06.3f] - [%+06.3f, %+06.3f]",
                       track_list[i]->getLine()->get_line()->line().x1(),
                       track_list[i]->getLine()->get_line()->line().x2(),
                       track_list[i]->getLine()->get_line()->line().y1(),
                       track_list[i]->getLine()->get_line()->line().y2());

        ui->listWidget_track->addItem(string);
    }
    qDebug("Leave PropertyWidget::updateTrackListWidget");
}

void PropertyWidget::addTrackButtonClicked()
{
    qDebug("Enter PropertyWidget::addTrackButtonClicked");
    if(!view->get_markedLines().isEmpty())
    {
        foreach(jpsLineItem *line, view->get_markedLines())
        {
            if(line->getType() == "track")
                current_zone->addTrack(line, ui->lineEdit_Type->text());
        }
    }

    updateTrackListWidget();
    qDebug("Leave PropertyWidget::addTrackButtonClicked");
}

void PropertyWidget::removeTrackButtonClicked()
{
    qDebug("Enter PropertyWidget::removeTrackButtonClicked");
    if(ui->listWidget_track->currentItem() == nullptr)
    {
        return;
    }
    else
    {
        int row = ui->listWidget_track->currentRow();
        auto *track = current_zone->getTrackList()[row];
        current_zone->removeTrack(track);

        ui->listWidget_track->setCurrentRow(-1);

        updateTrackListWidget();
    }
    qDebug("Leave PropertyWidget::removeTrackButtonClicked");
}

void PropertyWidget::updateType(QListWidgetItem *item)
{
    qDebug("Enter PropertyWidget::updateType");
    ui->lineEdit_Type->clear();

    if(current_zone == nullptr || item == nullptr)
        return;

    auto *track = current_zone->getTrackList()[ui->listWidget_track->currentRow()];
    ui->lineEdit_Type->setText(QString::number(track->getType()));
    qDebug("Leave PropertyWidget::updateType");
}

void PropertyWidget::applyTypeButtonClicked()
{
    qDebug("Enter PropertyWidget::applyTypeButtonClicked");
    int type = ui->lineEdit_Type->text().toInt();

    if(current_zone != nullptr)
        current_zone->getTrackList()[ui->listWidget_track->currentRow()]->setType(type);
    qDebug("Leave PropertyWidget::applyTypeButtonClicked");
}

void PropertyWidget::updatePropertyWidget()
{   
    qDebug("Enter PropertyWidget::updatePropertyWidget");
    if(current_zone == nullptr)
        return;

    switch(zoneType)
    {
        case Room:
            ui->comboBox_locateIn->setEnabled(false);
            ui->comboBox_roomType->setCurrentIndex(getRoomType(current_zone));
            ui->lineEdit_elevation->setText(QString::number(current_zone->get_elevation()));
            break;

        case Platform:
            ui->comboBox_locateIn->setEnabled(false);
            ui->comboBox_roomType->setEnabled(false);
            ui->lineEdit_elevation->setText(QString::number(current_zone->get_elevation()));
            break;
        default:
            qDebug("Leave PropertyWidget::updateWidget");
            return;
    }

    qDebug("Leave PropertyWidget::updatePropertyWidget");
}

int PropertyWidget::getRoomType(JPSZone *zone)
{
    qDebug("Enter PropertyWidget::getRoomType");
    switch(zone->getRoomType())
    {
        case Corridor:
            return 0;
        case Office:
            return 1;
        case Lobby:
            return 2;
        case Entrance:
            return 3;
    }
    qDebug("Leave PropertyWidget::getRoomType");
    return 0;
}

void PropertyWidget::applyButtonClicked()
{
    qDebug("Enter PropertyWidget::applyButtonClicked");
    if(current_zone == nullptr)
        return;

    switch(zoneType)
    {
        case Room:
            current_zone->set_elevation(ui->lineEdit_elevation->text().toFloat());
            current_zone->setRoomType(getRoomTypeFromString(ui->comboBox_roomType->currentText()));
            break;
        case Platform:
            current_zone->set_elevation(ui->lineEdit_elevation->text().toFloat());
            break;
        case Stair:
            break;
        default:
            qDebug("Leave PropertyWidget::updateWidget");
            return;
    }
    qDebug("Leave PropertyWidget::applyButtonClicked");
}

RoomType PropertyWidget::getRoomTypeFromString(QString type)
{
    qDebug("Enter getRoomTypeFromString");
    if(type == "Corridor")
    {
        return Corridor;
    }
    else if(type == "Office")
    {
        return Office;
    }
    else if(type == "Lobby")
    {
        return Lobby;
    }
    else if(type == "Entrance")
    {
        return Entrance;
    }
    else
    {
        return Corridor;
    }
    qDebug("Leave getRoomTypeFromString");
}
