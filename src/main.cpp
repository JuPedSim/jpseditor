/**
 * \file        main.cpp
 * \date        Oct-01-2018
 * \version     v0.8.4
 * \copyright   <2009-2018> Forschungszentrum Jülich GmbH. All rights reserved.
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
 * main.cpp is setting up the main window and is starting the application loop.
 *
 **/

//main.cpp

 //for compiling a standalone windows exe with VS
#ifdef _MSC_VER
#    ifdef NDEBUG
#        pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#    else
#        pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#    endif
#endif
#define LOG_FILE_KEEP_NUM 30


#include "mainWindow.h"
#include "GraphicView.h"
#include <QApplication>
#include <QDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Add log dir if there is no
    QDir dir("log");

    if (!dir.exists())
    {
        QDir dir;
        dir.mkdir("log");
    }

    // Create file
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    QString logName = "log_" + currentDate + ".txt";
    QString logFileName = "log/" + logName;

    if (!QFile::exists(logFileName))
    {
        QFileInfoList fileList = dir.entryInfoList(QStringList() << "*.txt", QDir::NoFilter, QDir::Time);

        // Check number of log files
        if (fileList.size() >= LOG_FILE_KEEP_NUM)
        {
            int i = 1;
            foreach(QFileInfo fileInfo , fileList)
            {
                if (i >= LOG_FILE_KEEP_NUM)
                {
                    QString fileName = fileInfo.absoluteFilePath();
                    QFile::remove(fileName);
                }
                i++;
            }
        }
    }

    // If application is still running, add log in one file
    if(dir.exists() && !dir.entryInfoList(QStringList() << "*.txt", QDir::NoFilter, QDir::Time).isEmpty())
    {
        QFile firstFile("log/" + dir.entryInfoList(QStringList() << "*.txt", QDir::NoFilter, QDir::Time).first().fileName());

        if(firstFile.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&firstFile);
            bool complete = false;

            while (!firstFile.atEnd())
            {
                QString line = firstFile.readLine();

                if(line.contains("Leave MWindow::~MWindow"))
                {
                    // True means the newest file is for the last run
                    complete = true;
                }
            }

            if(!complete)
            {
                logFileName = firstFile.fileName(); // fileName() return the relative path, rather not only the name
            }
        }

        firstFile.close();
    }

    QFile file(logFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.close();
        return ;
    }

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString logMsg;

    switch (type)
    {
        case QtDebugMsg:
            logMsg = QString("%1: [Debug]: %2 \n").arg(currentDateTime).arg(msg);
            break;
        case QtInfoMsg:
            logMsg = QString("%1: [Info]: %2  \n").arg(currentDateTime).arg(msg);
            break;
        case QtWarningMsg:
            logMsg = QString("%1: [Warning]: %2 \n").arg(currentDateTime).arg(msg);
            break;
        case QtCriticalMsg:
            logMsg = QString("%1: [Critical]: %2 \n").arg(currentDateTime).arg(msg);
            break;
        case QtFatalMsg:
            logMsg = QString("%1: [Fatal]: %2 \n").arg(currentDateTime).arg(msg);
            abort();
            break;
        default:
            break;
    }

    QTextStream textStream(&file);

    textStream << logMsg;

    file.close();
}

int main(int argc, char* argv[])
{

#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageOutput); // Logger qDebug Infomation
#endif

    QApplication app(argc,argv);
    MWindow mywindow;
    mywindow.show();

    return app.exec();
}