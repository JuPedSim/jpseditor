//
// Created by sainho93 on 2019-05-01.
//

#ifndef JPSEDITOR_INIREADER_H
#define JPSEDITOR_INIREADER_H

#include <QXmlStreamWriter>
#include "src/widgets/inifilewidget.h"

class IniReader
{
public:
    IniReader(InifileWidget *inifileWidget);

    bool read(QIODevice *device);

    QString errorString() const;

    void readJuPedSim(InifileWidget *widget);

    void readSeed(InifileWidget *widget);

private:
    QXmlStreamReader xml;
    InifileWidget *widget;
};


#endif //JPSEDITOR_INIREADER_H
