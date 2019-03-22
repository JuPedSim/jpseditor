#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <QGraphicsScene>
#include "src/datamanager.h"
#include "src/graphicscene.h"
#include "src/jpssource.h"
#include "src/models/jpssourcelistmodel.h"


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
    void deleteButtonClicked();

private:
    Ui::SourceWidget *ui;
    jpsGraphicsView *currentView;
    JPSSourceListModel *model;

signals:
    void deletedSource(int index);
};

#endif // SOURCEWIDGET_H
