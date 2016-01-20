#include "actionstack.h"

ActionStack::ActionStack()
{

}

ActionStack::~ActionStack()
{

}

LineAction ActionStack::GetRecentAction()
{
   return _actionstack.pop();
}

void ActionStack::PushNewAction(const LineAction &action)
{
    _actionstack.push_back(action);

}

