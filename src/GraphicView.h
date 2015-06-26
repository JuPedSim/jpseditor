/**
 * \file        GraphicView.h
 * \date        Jun 26, 2015
 * \version     v0.7
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
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include "jpsLineItem.h"
#include "jpslandmark.h"


class jpsGraphicsView: public QGraphicsView {

    Q_OBJECT

public:
    ///Constructor
    jpsGraphicsView(QWidget* parent = nullptr);

    ///Destructor
    ~jpsGraphicsView();

    ///Change modes
    void change_gridmode();
    bool get_gridmode();
    bool use_gridmode(QGraphicsLineItem *currentline, int accuracy);
    void change_objectsnap();
    bool get_objectsnap();
    bool statusWall();
    void en_disableWall();
    bool statusDoor();
    void en_disableDoor();
    bool statusExit();
    void en_disableExit();
    bool statusLandmark();
    void en_disableLandmark();

    ///Pos
    QPointF return_Pos();

    ///Catch points
    void catch_points();

    ///Delete all
    void delete_all(bool final=false);

    ///Line operations
    void catch_lines();
    void drawLine();
    qreal calc_d_point(const QLineF &line, const qreal &x, const qreal &y);
    void delete_marked_lines();

    /// Landmark
    void delete_landmark();
    void catch_landmark();
    void select_landmark(jpsLandmark *landmark);
    void addLandmark();
    void unmarkLandmark();
    QList<jpsLandmark *> get_landmarks();

    ///Waypoints
    QGraphicsRectItem* GetCurrentSelectRect();
    void ShowWaypoints(QList<jpsWaypoint* > waypoints);


    /// Lines
    void take_l_from_lineEdit(const qreal &length);
    QList<jpsLineItem *> get_markedLines();
    QList<jpsLineItem *> get_line_vector();
    qreal get_scale_f();
    void unmark_all_lines();
    void select_line(jpsLineItem *mline);
    void disable_drawing();
    jpsLineItem *addLineItem(const qreal &x1, const qreal &y1, const qreal &x2, const qreal &y2, const QString &type="");
    void locate_intersection(jpsLineItem* item1, jpsLineItem* item2);
    void SetVLine();
    void EditLine(QPointF* point);

    ///RoomCaption
    bool show_hide_roomCaption(QString name, qreal x, qreal y);

    void line_collision();
    void create_grid();

    ///View
    void zoom(int delta);
    void translations(QPointF old_pos);
    void AutoZoom();

public slots:
    ///Waypoints
    void StatAssoDef();
    void ClearWaypoints();


protected:
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    //void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsLineItem* current_line;
    //std::vector<jpsLineItem> line_vector;
    QList<QPointF *> intersect_point_vector;
    QList<QPointF> grid_point_vector;
    QList<jpsLineItem *> line_vector;
    //QList<QList<jpsLineItem*> *> mainlist;
    QPointF pos;
    //QPointF* intersection_point;
    QGraphicsScene* Scene;
    bool midbutton_hold;
    bool leftbutton_hold;
    qreal translation_x;
    qreal translation_y;
    QPointF translated_pos;
    bool gridmode;
    bool statWall;
    bool statDoor;
    bool statExit;
    bool statLandmark;
    qreal catch_radius;
    qreal gl_scale_f;
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
    QList<jpsLandmark* > LLandmarks;
    jpsLandmark* markedLandmark;
    QGraphicsRectItem* currentLandmarkRect;
    QList<QGraphicsEllipseItem* > _waypoints;
    QGraphicsLineItem* _currentVLine;
    QPointF* _currentTrackedPoint;

    bool _statLineEdit;

    bool lines_collided;
    bool _assoDef;

    //qreal gl_min_x;
    //qreal gl_min_y;

signals:
    void mouse_moved();
    void set_focus_textedit();
    void lines_deleted();
    void no_drawing();
    void remove_marked_lines();
    void remove_all();
    void landmark_added();
    void AssoDefCompleted();
    //void DoubleClick();




};



#endif // GRAPHICVIEW_H
