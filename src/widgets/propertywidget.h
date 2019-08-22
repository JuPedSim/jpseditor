#ifndef JPSEDITOR_PROPERTYWIDGET_H
#define JPSEDITOR_PROPERTYWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include "src/datamanager.h"

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
    void updateWallListWidget();
    void updateCrossingListWidget();

    // Crossing tab
    void addCrossingButtonClicked();
    void removeCrossingButtonClicked();

    // Wall tab
    void addWallButtonClicked();
    void removeWallButtonClicked();
    void highlightWall(QListWidgetItem *item);

    // Inspector Tab
    void applyElevationButtonClicked();

    // Track tab
    void updateTrackListWidget();

private:
    Ui::PropertyWidget *ui;
    jpsGraphicsView *view;
    jpsDatamanager *data;

    JPSZone *current_zone;
};

#endif //JPSEDITOR_PROPERTYWIDGET_H
