#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include <QWidget>
#include "GraphicView.h"
#include "datamanager.h"
#include "GraphicView.h"


namespace Ui {
class SourceWidget;
}

class SourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SourceWidget(QWidget *parent = nullptr, jpsGraphicsView *gview = nullptr, jpsDatamanager *dmanager = nullptr);
    ~SourceWidget();

protected slots:
    void showSource();
    void showSourceInformation();
    void applySourceInformation();

private:
    Ui::SourceWidget *ui;
    jpsDatamanager *datamanager;
    jpsGraphicsView *graphview;
};

#endif // SOURCEWIDGET_H
