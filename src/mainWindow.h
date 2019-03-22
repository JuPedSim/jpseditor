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
 * This class is setting up the main window incl. all buttons and bars. It is the parent widget of all other widgets
 * (GraphicView, roomWidget, widgetLandmark).
 *
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QDockWidget>
#include "ui_mainwindow.h"
#include "src/widgets/roomwidget.h"
#include "src/widgets/widgetlandmark.h"
#include "src/widgets/widgetsettings.h"
#include "GraphicView.h"
#include "datamanager.h"
#include "rooms.h"
#include "src/widgets/settingdialog.h"
#include "src/widgets/inifilewidget.h"
#include "src/widgets/snappingoptions.h"
#include "src/widgets/sourcewidget.h"

class MWindow : public QMainWindow, private Ui::MainWindow {

    Q_OBJECT
public:
    MWindow();
    ~MWindow();


private:
    roomWidget* rwidget;
    widgetLandmark* lwidget;
    InifileWidget * inifileWidget;
    QDockWidget *sourceDockWidget;
    SourceWidget *sourceWidget;
    SnappingOptions* snappingOptions;
//    WidgetSettings* _settings;
    jpsDatamanager* dmanager;
    jpsGraphicsView* mview;
    QGraphicsScene *mscene;
    //QVBoxLayout* VBox;
    QLineEdit* length_edit;
    QLineEdit* x_edit;
    QLineEdit* y_edit;
    QLabel* label1;
    QLabel* label2;
    QLabel* label_x;
    QLabel* label_y;
    QLabel* infoLabel;
    QString _filename;

    //CMap
    QTimer *_cMapTimer;
    int _cMapFrame;

    //default setting
    SettingDialog *settingDialog;
    QTimer *timer;
    QSettings settings;

    bool _statScale;

    QActionGroup *drawingActionGroup;

protected slots:
    
    //parseFiles
    void openFileDXF();
    void openFileXML();
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
    void en_disableDoor();
    void en_disableExit();
    void en_disableLandmark();
    void en_disableHLine();
    void disableDrawing();
    void objectsnap();
    void sourceButtonClicked();
    void editModeButtonClicked();


    //Line operations
    void show_coords();
    void delete_lines();
    void delete_marked_lines();
    void send_length();
    void en_selectMode();
    void dis_selectMode();
    void lines_deleted();
    void remove_all_lines();
    void ShowLineLength();
    void ScaleLines();
    void enableScale();

    void send_xy();
    
    //Room and landmark def
    void define_room();
    void autoDefine_room();
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
    void closeEvent(QCloseEvent *event);

    //ESCAPE
    void keyPressEvent(QKeyEvent *event);



private slots:
    void on_actionCopy_triggered();
    void on_actionOnline_Help_triggered();
    void on_actionClear_all_Rooms_and_Doors_triggered();

    //default setting
    void saveSettings(QMap<QString, QString> settingsmap);
    QMap<QString, QString> loadSettings();
    void on_actionNew_Inifile_triggered();
    void on_actionBack_to_Origin_triggered();
    void on_actionZoom_Windows_triggered();
    void on_actionZoom_Extents_triggered();
};

#endif // MAINWINDOW_H
