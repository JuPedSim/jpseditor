#ifndef JPSEDITOR_JPSTRACK_H
#define JPSEDITOR_JPSTRACK_H

#include "src/jpsLineItem.h"

class JPSTrack
{
public:
    JPSTrack(jpsLineItem *lineItem);

    ~JPSTrack();

    QString getNumber() const;

    void setNumber(QString number);

    jpsLineItem *getLine() const;

private:
    jpsLineItem *line;
    QString number;
};


#endif //JPSEDITOR_JPSTRACK_H
