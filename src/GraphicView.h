/**
 * \file        GraphicView.h
 * \date        Jun 26, 2015
 * \version     v0.8.1
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
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
#include "graphicscene.h"
#include <QGraphicsLineItem>
#include "jpsLineItem.h"
#include "jpslandmark.h"
#include "jpsconnection.h"
#include "./UndoFramework/actionstack.h"

using ptrConnection = std::shared_ptr<jpsConnection>;
using ptrLandmark = std::shared_ptr<jpsLandmark>;

class jpsDatamanager;

class jpsGraphicsView: public QGraphicsView {

    Q_OBJECT

public:
    //Constructor
    jpsGraphicsView(QWidget* parent = nullptr, jpsDatamanager* datamanager=nullptr);

    //Destructor
    ~jpsGraphicsView();

    QGraphicsScene* GetScene();
    const qreal& GetTranslationX() const;
    const qreal& GetTranslationY() const;
    const qreal& GetScaleF() const;

    //Pos
    const QPointF& return_Pos() const;


    void SetDatamanager(jpsDatamanager* datamanager);

    //Change modes
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

    // global functions
    qreal get_scale_f();
    void take_l_from_lineEdit(const qreal &length);
    void disable_drawing();
    bool use_anglesnap(QGraphicsLineItem *currentline, int accuracy);
    void use_gridmode();

    //View
    void zoom(int delta);
    void translations(QPointF old_pos);
    void AutoZoom();
    qreal CalcGridSize();
    void ShowOrigin();


    //Catch lines, points and intersections
    void catch_points();
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

protected:
    //Mouse events
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    //void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

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
    GraphicScene* Scene;
    bool midbutton_hold;
    bool leftbutton_hold;
    qreal translation_x;
    qreal translation_y;
    QPointF translated_pos;
    bool anglesnap;
    bool statWall;
    bool statDoor;
    bool statExit;
    bool statLandmark;
    bool _statHLine;
    int _statCopy;
    QPointF _copyOrigin;
    qreal catch_radius;
    qreal _scaleFactor;
    qreal gl_scale_f;
    qreal _gridSize;
    bool point_tracked;
    QGraphicsItem* current_rect;
    QGraphicsRectItem* currentSelectRect;
    bool objectsnap;
    QPointF* intersection_point;
    int line_tracked;
    QPen currentPen;
    qreal catch_line_distance;
    QList<jpsLineItem *> marked_lines;
    QGraphicsTextItem* current_caption;
    QList<QGraphicsTextItem* > caption_list;
    int id_counter;

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
    bool _gridmode;

    //Undo/Redo
    ActionStack _undoStack;
    ActionStack _redoStack;


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
    //void DoubleClick();

};



#endif // GRAPHICVIEW_H
