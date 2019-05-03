//
// Created by Tao Zhong on 2019-04-01.
// See more infomation in goalreader.cpp
//

#ifndef JPSEDITOR_GOALREADER_H
#define JPSEDITOR_GOALREADER_H

#include <QtWidgets>
#include <src/GraphicView.h>
#include "src/jpsgoal.h"
#include "src/GraphicView.h"

class GoalReader
{
public:
    GoalReader(jpsGraphicsView *view);

    bool read(QIODevice *device);

private:
    void readJPScore();
    void readGoals();
    void readGoal();

    QXmlStreamReader xml;
    jpsGraphicsView *currentView;
};



#endif //JPSEDITOR_GOALREADER_H
