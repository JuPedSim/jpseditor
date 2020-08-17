#ifndef JPSEDITOR_RUNNINGWIDGET_H
#define JPSEDITOR_RUNNINGWIDGET_H

#include <QWidget>
#include <QProcess>

namespace Ui {
    class RunningWidget;
}

class RunningWidget : public QWidget{
    Q_OBJECT

public:
    explicit RunningWidget(QWidget *parent = nullptr);
    ~RunningWidget();

private slots:
    void on_readoutput();
    void on_readerror();
    void inputCommand();

    void chooseInifileButtonClicked();
    void chooseJPScoreButtonClicked();
    void chooseJPSvisButtonClicked();
    void chooseGeometryButtonClicked();
    void chooseTrajetoryButtonClicked();

    void runButtonClicked();
    void viewGeometryButtonClicked();
    void viewResultButtonClicked();

private:
    Ui::RunningWidget *ui;
    QProcess *cmd;
};


#endif //JPSEDITOR_RUNNINGWIDGET_H
