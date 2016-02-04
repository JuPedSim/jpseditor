#include "action.h"

Action::Action()
{

}

Action::Action(const QString &name, const QString &type)
{
    _name=name;
    _type=type;


}

Action::~Action()
{

}

const QString &Action::GetName() const
{
    return _name;

}

const QString &Action::GetType() const
{
    return _type;
}
