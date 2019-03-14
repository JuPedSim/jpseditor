#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include "datamanager.h"
#include "graphicscene.h"


namespace Ui {
class SourceWidget;
}

class SourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SourceWidget(QWidget *parent = nullptr, QGraphicsScene *scene = nullptr,
                          jpsDatamanager *dmanager = nullptr);
    ~SourceWidget();

protected slots:
    void showSource();
    void showSourceInformation();
    void applySourceInformation();

private:
    Ui::SourceWidget *ui;
    jpsDatamanager *datamanager;
    QGraphicsScene *currentScene;
};

#endif // SOURCEWIDGET_H
