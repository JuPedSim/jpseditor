#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <QGraphicsScene>
#include "datamanager.h"
#include "graphicscene.h"
#include "jpssource.h"


namespace Ui {
class SourceWidget;
}

class SourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SourceWidget(QWidget *parent , jpsGraphicsView *view ,
                          jpsDatamanager *dmanager);
    ~SourceWidget();

protected slots:
    void showSource();
    void showSourceInformation();
    void applySourceInformation();

private:
    Ui::SourceWidget *ui;
    jpsGraphicsView *currentView;
};

#endif // SOURCEWIDGET_H
