#ifndef JPSEDITOR_RUNNINGWIDGET_H
#define JPSEDITOR_RUNNINGWIDGET_H

#include <QWidget>

namespace Ui {
    class RunningWidget;
}

class RunningWidget : public QWidget{
    Q_OBJECT

public:
    explicit RunningWidget(QWidget *parent = nullptr);
    ~RunningWidget();

private:
    Ui::RunningWidget *ui;
};


#endif //JPSEDITOR_RUNNINGWIDGET_H
