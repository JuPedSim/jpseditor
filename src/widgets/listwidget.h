#ifndef JPSEDITOR_LISTWIDGET_H
#define JPSEDITOR_LISTWIDGET_H

#include <QtGui>
#include <QtWidgets>

#include "src/datamanager.h"
#include "src/global.h"
#include "src/GraphicView.h"

namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr, jpsDatamanager *dmanager = nullptr, jpsGraphicsView *gview = nullptr);

    ~ListWidget();


public slots:
    void updateList();

    void addButtonClicked();
    void deleteButtonClicked();

    void select(QListWidgetItem *item);
    JPSZone *getCurrent(QListWidgetItem *item);
    const QList<JPSZone *> &getCurrentList(ZoneType type) const;

    void rename(QListWidgetItem *item);
    bool isRepeatedName(QString name);

    void highlight(QListWidgetItem *item);

    ZoneType getZoneType() const;
    void setZoneType(ZoneType zoneType);

signals:
    void zoneSelected(JPSZone *zone);
    void zoneDeleted();


private:
    Ui::ListWidget *ui;
    jpsDatamanager *data;
    jpsGraphicsView *view;

    ZoneType zoneType;
};

#endif //JPSEDITOR_LISTWIDGET_H
