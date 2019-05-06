 # This file is part of JuPedSim.
 #
 # JuPedSim is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # any later version.
 #
 # JuPedSim is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public License
 # along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 #
 # copyright   <2009-2018> Forschungszentrum Juelich GmbH.

 # Compatibility with Qt4 and Qt5
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

TEMPLATE = app
TARGET = JPSeditor

CONFIG += qt xml gui;
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -static
#QMAKE_CXXFLAGS += -static-libgcc
#QMAKE_CXXFLAGS += -static-libstdc++

#QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter



FORMS += \
    forms/mainwindow.ui \
    forms/roomwidget.ui \
    forms/widgetlandmark.ui \
    forms/widgetsettings.ui \
    forms/inifilewidget.ui \
    forms/settingdialog.ui \
    forms/snappingoptions.ui \
    forms/sourcewidget.ui \
    forms/goalwidget.ui \


HEADERS += \
    src/dtrace.h \
    src/mainWindow.h \
    src/GraphicView.h \
    src/graphicscene.h \
    src/widgets/roomwidget.h \
    src/rooms.h \
    src/datamanager.h \
    src/jpscrossing.h \
    src/jpsLineItem.h \
    src/jpsexit.h \
    src/jpsobstacle.h \
    src/jpslandmark.h \
    src/widgets/widgetlandmark.h \
    src/widgets/widgetsettings.h \
    src/jpsconnection.h \
    src/jpsregion.h \
    src/dtrace.h \
    src/widgets/settingdialog.h \
    src/widgets/inifilewidget.h \
    src/widgets/snappingoptions.h \
        src/jpssource.h \
    src/widgets/sourcewidget.h \
    src/widgets/goalwidget.h \
    src/elementtype.h \
    src/global.h \
    src/models/jpselementlistmodel.h \
    src/models/jpssourcelistmodel.h \
    src/jpsgoal.h \
    src/models/jpsgoallistmodel.h \
    src/XML/goalreader.h \
    src/XML/sourcereader.h \
    dxflib/src/dl_writer_ascii.h \
    dxflib/src/dl_writer.h \
    dxflib/src/dl_global.h \
    dxflib/src/dl_extrusion.h \
    dxflib/src/dl_exception.h \
    dxflib/src/dl_entities.h \
    dxflib/src/dl_dxf.h \
    dxflib/src/dl_creationinterface.h \
    dxflib/src/dl_creationadapter.h \
    dxflib/src/dl_codes.h \
    dxflib/src/dl_attributes.h \
    src/UndoFramework/action.h \
    src/UndoFramework/actionstack.h \
    src/UndoFramework/lineaction.h \
    src/AutomaticRoomIdentification/roomID.h \
    src/AutomaticRoomIdentification/roomdefinition.h \
    src/AutomaticRoomIdentification/roomidentification.h \
    src/tinyxml/tinystr.h \
    src/tinyxml/tinyxml.h
SOURCES += \
    src/mainWindow.cpp \
    src/main.cpp \
    src/GraphicView.cpp \
    src/graphicscene.cpp \
    src/widgets/roomwidget.cpp \
    src/rooms.cpp \
    src/datamanager.cpp \
    src/jpscrossing.cpp \
    src/jpsLineItem.cpp \
    src/jpsexit.cpp \
    src/jpsobstacle.cpp \
    src/jpslandmark.cpp \
    src/widgets/widgetlandmark.cpp \
    src/widgets/widgetsettings.cpp \
    src/jpsconnection.cpp \
    src/jpsregion.cpp \
    src/widgets/settingdialog.cpp \
    src/widgets/inifilewidget.cpp \
    src/widgets/snappingoptions.cpp \
        src/jpssource.cpp \
    src/widgets/sourcewidget.cpp \
    src/jpsgoal.cpp \
    src/widgets/goalwidget.cpp \
    src/global.cpp \
    src/models/jpselementlistmodel.cpp \
    src/models/jpssourcelistmodel.cpp \
    src/models/jpsgoallistmodel.cpp \
    src/XML/goalreader.cpp \
    src/XML/sourcereader.cpp \
    dxflib/src/dl_writer_ascii.cpp \
    dxflib/src/dl_dxf.cpp \
    src/UndoFramework/action.cpp \
    src/UndoFramework/actionstack.cpp \
    src/UndoFramework/lineaction.cpp \
    src/AutomaticRoomIdentification/roomdefinition.cpp \
    src/AutomaticRoomIdentification/roomidentification.cpp \
    src/tinyxml/tinystr.cpp \
    src/tinyxml/tinyxml.cpp \
    src/tinyxml/tinyxmlerror.cpp \
    src/tinyxml/tinyxmlparser.cpp \
#INCLUDEPATH += D:/boost_1_59_0/boost_1_59_0
RESOURCES += \
    Resources/resources.qrc

RC_FILE = Resources/jpseditor.rc
#osx fix
ICON = Resources/jpseditor.icns
#INCLUDEPATH += $$(ROOTSYS)/include


#CONFIG(debug, debug|release) {
#    DESTDIR = build/debug
#}
#CONFIG(release, debug|release) {
#    DESTDIR = build/release
#}
#
#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.u


demos.path = /usr/local/bin/jpseditor
demos.files = examples/*

INSTALLS += demos

target.path = /usr/local/bin/jpseditor
INSTALLS += target
