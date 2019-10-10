#ifndef JPSEDITOR_PROPERTYWIDGET_H
#define JPSEDITOR_PROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"
#include "src/jpstransition.h"

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    Q_OBJECT;

public:
    explicit PropertyWidget(QWidget *parent, jpsDatamanager *dmanager,
                            JPSZone *zone, jpsGraphicsView *gview);

    ~PropertyWidget();

protected slots:
    void updateWidget(ZoneType type);
    void updateListwidget();

    // Crossing tab
    void updateCrossingListWidget();
    void addCrossingButtonClicked();
    void removeCrossingButtonClicked();
    void applyOutflowButtonClicked();
    void applyMaxagentsButtonClicked();
    void updateCrossingInfo(QListWidgetItem* item);

    // Wall tab
    void updateWallListWidget();
    void addWallButtonClicked();
    void removeWallButtonClicked();
    void highlightWall(QListWidgetItem *item);

    // Inspector Tab
    void applyElevationButtonClicked();

    // Track tab
    void updateTrackListWidget();
    void addTrackButtonClicked();
    void removeTrackButtonClicked();
    void updateType(QListWidgetItem* item);
    void applyTypeButtonClicked();

private:
    Ui::PropertyWidget *ui;
    jpsGraphicsView *view;
    jpsDatamanager *data;

    JPSZone *current_zone;
};

#endif //JPSEDITOR_PROPERTYWIDGET_H
