#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H


#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include "jpsLineItem.h"



class jpsGraphicsView: public QGraphicsView {

    Q_OBJECT

public:
    jpsGraphicsView(QWidget* parent = 0L);
    ~jpsGraphicsView(){}
    void change_gridmode();
    bool get_gridmode();
    void use_gridmode();
    void catch_points();
    void change_objectsnap();
    bool get_objectsnap();
    bool statusWall();
    void en_disableWall();
    bool statusDoor();
    void en_disableDoor();
    bool statusExit();
    void en_disableExit();
    QPointF return_Pos();
    void delete_all();
    void catch_lines();
    qreal calc_d_point(const QLineF &line, const qreal &x, const qreal &y);
    void delete_marked_lines();
    void take_l_from_lineEdit(const qreal &length);
    QList<jpsLineItem *> get_markedLines();
    QList<jpsLineItem *> get_line_vector();
    qreal get_scale_f();
    void unmark_all_lines();
    void select_line(jpsLineItem *mline);
    void disable_drawing();
    void addLineItem(qreal x1,qreal y1, qreal x2, qreal y2);
    void locate_intersection(jpsLineItem* item1, jpsLineItem* item2);
    bool show_hide_roomCaption(QString name, qreal x, qreal y);


protected:
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    //void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsLineItem* current_line;
    //std::vector<jpsLineItem> line_vector;
    QList<QPointF *> intersect_point_vector;
    QList<jpsLineItem *> line_vector;
    //QList<QList<jpsLineItem*> *> mainlist;
    QPointF pos;
    //QPointF* intersection_point;
    QGraphicsScene* Scene;
    bool midbutton_hold;
    qreal translation_x;
    qreal translation_y;
    QPointF translated_pos;
    bool gridmode;
    bool statWall;
    bool statDoor;
    bool statExit;
    qreal catch_radius;
    qreal gl_scale_f;
    bool point_tracked;
    QGraphicsItem* current_rect;
    bool objectsnap;
    QPointF* intersection_point;
    int line_tracked;
    QPen currentPen;
    qreal catch_line_distance;
    QList<jpsLineItem *> marked_lines;
    QGraphicsTextItem* current_caption;
    QList<QGraphicsTextItem* > caption_list;
    //qreal gl_min_x;
    //qreal gl_min_y;

signals:
    void mouse_moved();
    void set_focus_textedit();
    void lines_deleted();
    void no_drawing();
    void remove_marked_lines();
    void remove_all();




};



#endif // GRAPHICVIEW_H
