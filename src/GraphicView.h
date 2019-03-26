/**
 * \file        GraphicView.h
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
 * This class is setting up the drawing area. It enables the user to draw walls and doors.
 *
 **/

#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H


#include <QGraphicsView>
//#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include "jpsLineItem.h"
#include "jpslandmark.h"
#include "jpsconnection.h"
#include "jpssource.h"
#include "jpsgoal.h"
#include "./UndoFramework/actionstack.h"

using ptrConnection = std::shared_ptr<jpsConnection>;
using ptrLandmark = std::shared_ptr<jpsLandmark>;

class jpsDatamanager;

class jpsGraphicsView: public QGraphicsView {

    Q_OBJECT

public:
    //Constructor
    explicit jpsGraphicsView(QWidget* parent = nullptr, jpsDatamanager* datamanager=nullptr);

    //Destructor
    ~jpsGraphicsView() override;

    QGraphicsScene *GetScene();
    const qreal& GetTranslationX() const;
    const qreal& GetTranslationY() const;
    const qreal& GetScaleF() const;

    //Pos
    const QPointF& return_Pos() const;

    //datamanger
    void SetDatamanager(jpsDatamanager* datamanager);

    //Change modes
    enum DrawingMode {Selecting, Editing, Wall, Door, Exit, HLine, Landmark, Source, Goal};
    void setDrawingMode(DrawingMode mode);

    void change_stat_anglesnap();
    bool get_stat_anglesnap();
    void change_objectsnap();
    bool get_objectsnap();
    void change_gridmode();
    bool statusWall();
    void en_disableWall();
    bool statusDoor();
    void en_disableDoor();
    bool statusExit();
    void en_disableExit();
    bool statusHLine();
    void en_disableHLine();
    bool statusLandmark();
    void en_disableLandmark();
    void enableSourceMode();
    void drawSource();
    void drawGoal();
    void enableEditMode();
    void enableGoalMode();


    // global functions
    qreal get_scale_f();
    void take_l_from_lineEdit(const qreal &length);
    void take_endpoint_from_xyEdit(const QPointF &endpoint);
    void disable_drawing();
    bool use_anglesnap(QGraphicsLineItem *currentline, int accuracy);
    void use_gridmode();

    //View
    void zoom(int delta);
    void translations(QPointF old_pos);
    void AutoZoom();
    qreal CalcGridSize();
    void ShowOrigin();
    void selectedWindows();


    //Catch lines, points
    void catch_points();
    void catch_start_endpoints();
    void catch_intersections_point();
    void catch_center_point();
    void catch_line_point();

    void locate_intersection(jpsLineItem* item1, jpsLineItem* item2);
    void line_collision();
    void catch_lines();

    //Delete all
    void delete_all(bool final=false);

    //Line operations
    void drawLine();
    qreal calc_d_point(const QLineF &line, const qreal &x, const qreal &y);
    void delete_marked_lines();
    void RemoveLineItem(jpsLineItem *mline);
    void RemoveLineItem(const QLineF &line);
    void RemoveIntersections(jpsLineItem* lineItem);
    // lines read from dxf-file
    jpsLineItem *addLineItem(const qreal &x1, const qreal &y1, const qreal &x2, const qreal &y2, const QString &type="");
    jpsLineItem *addLineItem(const QLineF &line, const QString &type="");
    QList<jpsLineItem *> get_markedLines();
    QList<jpsLineItem *> get_line_vector();
    void unmark_all_lines();
    void select_line(jpsLineItem *mline);
    void SetVLine();
    void EditLine(QPointF* point);
    qreal ReturnLineLength();
    void start_Copy_function();
    void Copy_lines(const QPointF& delta);
    void ScaleLines(const double& factor);
    QPointF getNearstPointOnLine(jpsLineItem* selected_line);

    // Landmark
    void delete_landmark();
    void catch_landmark();
    void select_landmark(jpsLandmark *landmark);
    void addLandmark();
    void addLandmark(const QPointF& pos);
    // unmark Landmarks see slots

    //Connections and YAHPointer
    QGraphicsRectItem* GetCurrentSelectRect();
    void SetStatDefConnections(const int& stat);

    void ShowYAHPointer(const QPointF& pos, const qreal& dir);
    void ShowConnections(QList<ptrConnection> cons);
    void ClearConnections();

    //RoomCaption
    bool show_hide_roomCaption(QString name, qreal x, qreal y);
    // return true if caption is hided
    bool is_hide_roomCaption(QString name);

    //Undo Framework
    void RecordUndoLineAction(const QString &name, const QString &type, const int &itemID, const QLineF &oldLine);
    void RecordRedoLineAction(const QString &name, const QString &type, const int &itemID, const QLineF &oldLine);
    void UndoLineEdit(const int &lineID, const QLineF &old_line);
    void RedoLineEdit(const int &lineID, const QLineF &old_line);

    //Grid Mode
    void ChangeGridmode(const bool& stat);
    bool GetGridmode() const;
    void ChangeTranslation(qreal x, qreal y);
    void SetGrid(QString grid);
    void ChangeGridSize(const qreal& factor);

    QList<JPSSource *> getSources();

public slots:
    //Landmarks/Regions
    void StatPositionDef();
    void ChangeRegionStatDef();
    //GridSettings
    void ActivateLineGrid();
    void ActivatePointGrid();
    //Landmarks
    void unmarkLandmark();
    //Line operations
    void SelectAllLines();
    //Undo Redo
    void Undo();
    void Redo();
    void deleteSource(int index);
    void itemSeleted(const QModelIndex &index);
    void changeSource(int index);

protected:
    //Mouse events
    void mouseMoveEvent(QMouseEvent * mouseEvent) override;
    //void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //Grid Mode
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void DrawLineGrid(QPainter *painter, const QRectF &rect);
    void DrawPointGrid(QPainter *painter, const QRectF &rect);

protected slots:
    void changeStart_endpoint(bool state);
    void changeIntersections_point(bool state);
    void changeCenter_point(bool state);
    void changeLine_point(bool state);

private:
    jpsDatamanager* _datamanager;
    QGraphicsLineItem* current_line;
    QPolygonF polygon;
    //std::vector<jpsLineItem> line_vector;
    QList<QPointF *> intersect_point_vector;
    //QList<QPointF> grid_point_vector;
    QList<jpsLineItem *> line_vector;
    QList<QGraphicsLineItem *> _origin;
    //QList<QList<jpsLineItem*> *> mainlist;
    QPointF pos;
    //QPointF* intersection_point;
    bool midbutton_hold;
    bool leftbutton_hold;
    qreal translation_x;
    qreal translation_y;
    QPointF translated_pos;
    bool anglesnap;

    // Drawing Mode
    DrawingMode drawingMode;
//    bool statWall;
//    bool statDoor;
//    bool statExit;
//    bool statLandmark;
//    bool _statHLine;

    bool stat_break_;
    int _statCopy;
    QPointF _copyOrigin;
    qreal catch_radius;
    qreal _scaleFactor;
    qreal gl_scale_f;
    bool point_tracked;
    QGraphicsItem* current_rect;
    QGraphicsRectItem* currentSelectRect;
    bool objectsnap;
    bool start_endpoint_snap;
    bool intersectionspoint_snap;
    bool centerpoint_snap;
    bool linepoint_snap;
    QPointF* intersection_point;
    int line_tracked;
    QPen currentPen;
    qreal catch_line_distance;
    QList<jpsLineItem *> marked_lines;
    QGraphicsTextItem* current_caption;
    QList<QGraphicsTextItem* > caption_list;
    int id_counter;

    //Source
    QGraphicsRectItem *currentSource;

    //Goal
    QGraphicsRectItem *currentGoal;

//    QGraphicsItemGroup *sourceGroup;
//    QGraphicsItemGroup *getSourceGroup() const;

private:

    //Landmark and waypoints
    jpsLandmark* markedLandmark;
    QGraphicsRectItem* currentLandmarkRect;
    QList<QGraphicsLineItem* > _connections;
    QList<QGraphicsLineItem* > _yahPointer;
    QList<QGraphicsTextItem* > _waypointLabels;
    int _statDefConnections;
    bool _regionDef;
    bool _posDef;

    QGraphicsLineItem* _currentVLine;
    QPointF* _currentTrackedPoint;
    QGraphicsPixmapItem* gridmap;
    bool _statLineEdit;
    bool lines_collided;


    //Undo/Redo
    ActionStack _undoStack;
    ActionStack _redoStack;

    //View
    bool statzoomwindows;

    //Grid Mode
    qreal _gridSize;
    bool _gridmode;
    qreal _translationX;
    qreal _translationY;
    QString _statgrid;

signals:
    void mouse_moved();
    void set_focus_textedit();
    void lines_deleted();
    void no_drawing();
    void remove_marked_lines();
    void remove_all();
    void PositionDefCompleted();
    void LineLengthChanged();
    void DefConnection1Completed();
    void DefConnection2Completed();
    void RegionDefCompleted();
    void sourcesChanged();
    void goalsChanged();
    //void DoubleClick();

};



#endif // GRAPHICVIEW_H
