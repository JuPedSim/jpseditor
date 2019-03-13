//
// Created by sainho93 on 2019-03-13.
//

#ifndef JPSEDITOR_JPSSOURCE_H
#define JPSEDITOR_JPSSOURCE_H

#include <QGraphicsView>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <QGraphicsRectItem>

class JPSSource
{
public:
    JPSSource(QGraphicsRectItem* source);

    JPSSource(int id);

    ~JPSSource();

    int getId() const;

    void setId(int id);

    QGraphicsRectItem *getSourceRect() const;

    void setSourceRect(QGraphicsRectItem *sourceRect);

    const QString &getFrequency() const;

    void setFrequency(const QString &frequency);

    const QString &getN_create() const;

    void setN_create(const QString &N_create);

    const QString &getPercent() const;

    void setPercent(const QString &percent);

    const QString &getRate() const;

    void setRate(const QString &rate);

    const QString &getTime_min() const;

    void setTime_min(const QString &time_min);

    const QString &getTime_max() const;

    void setTime_max(const QString &time_max);

    const QString &getAgents_max() const;

    void setAgents_max(const QString &agents_max);

    const QString &getGroup_id() const;

    void setGroup_id(const QString &group_id);

    const QString &getCaption() const;

    void setCaption(const QString &caption);

    const QString &getGreedy() const;

    void setGreedy(const QString &greedy);

    const QString &getTime() const;

    void setTime(const QString &time);

    const QString &getStartX() const;

    void setStartX(const QString &startX);

    const QString &getStartY() const;

    void setStartY(const QString &startY);

    qreal getX_min() const;

    void setX_min(qreal x_min);

    qreal getX_max() const;

    void setX_max(qreal x_max);

    qreal getY_min() const;

    void setY_min(qreal y_min);

    qreal getY_max() const;

    void setY_max(qreal y_max);

    bool isBeSaved() const;

    void setBeSaved(bool beSaved);

private:
    int id;
    QGraphicsRectItem *sourceRect;

    QString frequency;
    QString N_create;
    QString percent;
    QString rate;
    QString time_min;
    QString time_max;
    QString agents_max;
    QString group_id;
    QString caption;
    QString greedy;
    QString time;
    QString startX;
    QString startY;
    qreal x_min;
    qreal x_max;
    qreal y_min;
    qreal y_max;

    bool beSaved;
};


#endif //JPSEDITOR_JPSSOURCE_H
