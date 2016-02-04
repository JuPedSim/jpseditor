#include "lineaction.h"

LineAction::LineAction()
{

}

LineAction::LineAction(const QString &name, const QString &type, const int &itemID, const QLineF &oldLine) : Action(name,type)
{
    _oldLine=oldLine;
    _itemID=itemID;
}

LineAction::~LineAction()
{

}

QLineF LineAction::GetOldLine() const
{
    return _oldLine;
}

int LineAction::GetItemID() const
{
    return _itemID;
}

