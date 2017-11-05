#ifndef ROOMDEFINITION_H
#define ROOMDEFINITION_H


#include <QLineF>
#include <QList>
#include <vector>
class jpsDatamanager;
class jpsLineItem;

class RoomDefinition
{
public:
    RoomDefinition();
    RoomDefinition(const QList<jpsLineItem*>& lineItems, jpsDatamanager* dManager);

    void SetUpRoomsAndDoors();
    void RemoveOutside();
    void RemoveRoomsWithoutDoors();

private:
    std::vector<jpsLineItem*> _lineItems;
    std::vector<QLineF> _lines; // all walls, doors, transitions

    //roomDefinition
    jpsLineItem* FindLineItem(const QLineF& line) const;
    jpsDatamanager* _dManager;
};


#endif // ROOMDEFINITION_H
