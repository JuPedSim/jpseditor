//
// Created by Tao Zhong on 2019-04-02.
//

#ifndef JPSEDITOR_SOURCEREADER_H
#define JPSEDITOR_SOURCEREADER_H

#include "src/GraphicView.h"
#include <QtWidgets>

class SourceReader
{
public:
    SourceReader(jpsGraphicsView *view);

    bool read(QIODevice *device);

private:
    void readJPScore();
    void readSources();
    void readSource();

    QXmlStreamReader xml;
    jpsGraphicsView *currentView;
};


#endif //JPSEDITOR_SOURCEREADER_H
