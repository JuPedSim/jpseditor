#ifndef JPSEDITOR_JPSTRACK_H
#define JPSEDITOR_JPSTRACK_H

#include "src/jpsLineItem.h"

class JPSTrack
{
public:
    JPSTrack(jpsLineItem *lineItem);

    ~JPSTrack();

    int getType() const;

    void setType(int number);

    jpsLineItem *getLine() const;

private:
    jpsLineItem *line;
    int type ;
};


#endif //JPSEDITOR_JPSTRACK_H
