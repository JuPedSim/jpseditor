#ifndef WIDGETLANDMARK_H
#define WIDGETLANDMARK_H

#include <QTabWidget>
#include "datamanager.h"
#include "GraphicView.h"

namespace Ui {
class widgetLandmark;
}

class widgetLandmark : public QTabWidget
{
    Q_OBJECT

public:
    widgetLandmark(QWidget *parent = nullptr, jpsDatamanager *dmanager=nullptr, jpsGraphicsView *gview=nullptr);
    ~widgetLandmark();

protected slots:
    void show_landmarks();
    void add_room_to_landmark();
    void enable_room_selection();
    void disable_room_selection();
    void change_name();
    void AddAssociation();
    void ShowAssociations();
    void RemoveAssociation();

private:
    Ui::widgetLandmark *ui;
    jpsDatamanager* _dmanager;
    jpsGraphicsView* _gview;
};

#endif // WIDGETLANDMARK_H
