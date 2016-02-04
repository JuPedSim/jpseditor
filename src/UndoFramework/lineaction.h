#ifndef LINEACTION_H
#define LINEACTION_H

#include "action.h"
#include <QLine>

class LineAction : public Action
{
public:
    LineAction();
    LineAction(const QString& name, const QString& type, const int& itemID, const QLineF& oldLine);
    ~LineAction();

    QLineF GetOldLine() const;
    int GetItemID() const;
private:
    QLineF _oldLine;
    int _itemID;
};

#endif // PAINTACTION_H
