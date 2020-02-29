/**
 * \file        mainWindow.h
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
 * Controller for main window, connect data manager(model) and mainwinow.ui(view)
 *
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QDockWidget>

#include "ui_mainwindow.h"
#include "src/widgets/widgetlandmark.h"
#include "src/widgets/listwidget.h"
#include "GraphicView.h"
#include "datamanager.h"
#include "jpszone.h"
#include "src/XML/goalreader.h"
#include "src/XML/sourcereader.h"
#include "src/widgets/settingdialog.h"
#include "src/widgets/inifilewidget.h"
#include "src/widgets/snappingoptions.h"
#include "src/widgets/sourcewidget.h"
#include "src/widgets/goalwidget.h"
#include "src/widgets/propertywidget.h"
#include "src/widgets/transitionwidget.h"
#include "src/widgets/layerwidget.h"
#include "src/widgets/runningwidget.h"

class MWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
public:
    MWindow();
    ~MWindow();

private:
    // qwidget pointers, will be deleted by QtWidgets itself
    widgetLandmark *lwidget;
    InifileWidget *inifileWidget;

    QDockWidget *bottomDockWidget;
    QDockWidget *propertyDockWidget;
    QDockWidget *listDockWidget;
    ListWidget *listWidget;
    PropertyWidget *propertyWidget;

    SettingDialog *settingDialog;

    QToolBar *drawing_toolbar_;
    QActionGroup *drawingActionGroup;

    QToolBar *zone_toolbar_;
    QActionGroup *zoneActionGroup;

    SnappingOptions* snappingOptions;

    QLineEdit* length_edit;
    QLineEdit* x_edit;
    QLineEdit* y_edit;
    QLabel* label1;
    QLabel* label2;
    QLabel* label_x;
    QLabel* label_y;
    QLabel* infoLabel;
    QString _filename;

    /// Model
    QGraphicsScene *mscene;
    jpsDatamanager* dmanager;

    // CMap
//    QTimer *_cMapTimer;

    jpsGraphicsView* mview;

    /// Variabes
    int _cMapFrame;

    QTimer *timer;

    bool _statScale;

    QList<bool> objectsnapping;

    // Zone type in list widget
    ZoneType curentTypeListwidget;

public slots:
    
    //parseFiles
    void openFileDXF();

    //For "Load XML" menu button
    void openFileXML();
    QString openGeometry(QString fileName);
    QString openRouting(QString fileName);
    QString openSource(QString fileName);
    QString openGoal(QString fileName);
    QString openTraffic(QString fileName);

    void openFileCogMap();
    void OpenLineFile();

    void saveAsXML();
    void saveAsDXF();

    //cogmap
    void SaveCogMapXML();
    
    //about
    void info();
    
    //drawing options
    void anglesnap();
    void en_disableWall();
    void en_disableLandmark();
    void en_disableHLine();
    void objectsnap();
    void sourceButtonClicked();
    void goalButtionClicked();
    void transitionButtonClicked();
    void trackButtonClicked();

    //Line operations
    void show_coords();
    void deleteAllContents();
    void delete_marked_lines();

    void en_selectMode();
    void dis_selectMode();
    void ShowLineLength();
    void ScaleLines();
    void enableScale();

    void send_xy();
    
    void define_landmark();
    
    //view options
    void gridmode();
    void rotate();
    void Settings();
    void ShowOrigin();

    //autosave
    void AutoSave();

    //gather data
    void GatherData();

    //CMap
    //void RunCMap();
    //void UpdateCMap();

    //quit
    void closeEvent(QCloseEvent *event) override;

    // ESCAPE
    void keyPressEvent(QKeyEvent *event) override;

    void showStatusBarMessage(QString msg, int duration);

    // MeasureMode
    void measureLengthButtonClicked();
    void msgReceived(QString Msg);

    // Background
    void importBackground();

    // Layer
    void layerButtonClicked();

    // Running widget
    void runSimulationButtonClicked();

    // Time
    void setTimer(int interval);


private slots:
    //TODO: Redesign with signal and slot
    void on_actionCopy_triggered();
    void on_actionOnline_Help_triggered();
    void on_actionClear_all_Rooms_and_Doors_triggered();

    void on_actionNew_Inifile_triggered();
    void on_actionBack_to_Origin_triggered();
    void on_actionZoom_Windows_triggered();
    void on_actionZoom_Extents_triggered();

    //ToolBar
    void closeLeftToolBarArea();

    //Drawing ToolBar
    void setupDrawingToolBar();

    //Zone ToolBar
    void setupZoneToolBar();

    // Property widget
    void roomButtonClicked();
    void platformButtonClicked();
    void stairButtonClicked();
    void transitionWidgetButtonClicked();
    void sourceWidgetButtonClicked();
    void goalWidgetButtonClicked();
    void addPropertyDockWidget(JPSZone *zone); // used in room list widget
    void closePropertyDockWidget();


    // ListDockWidget
    void addListDockWidget(const QString &type);
    void closeListDockWidget();

    // BottemDockWidget
    void closeBottomDockWidget();

};

#endif // MAINWINDOW_H
