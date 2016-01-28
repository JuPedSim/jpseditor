#include "lineaction.h"

LineAction::LineAction()
{

}

LineAction::LineAction(const QString &name, const QString &type, const QLineF &oldLine) : Action(name,type)
{
    _oldLine=oldLine;
}

LineAction::~LineAction()
{

}

QLineF LineAction::GetOldLine() const
{
    return _oldLine;
}

