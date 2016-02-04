#include "jpsconnection.h"

jpsConnection::jpsConnection(ptrWaypoint waypoint1, ptrWaypoint waypoint2, const int &frameID)
{
    _waypoint1=waypoint1;
    _waypoint2=waypoint2;
    _firstFrame=frameID;
    _lastFrame=frameID;
}

jpsConnection::~jpsConnection()
{

}

std::pair<ptrWaypoint, ptrWaypoint> jpsConnection::GetWaypoints() const
{
    return std::make_pair(_waypoint1,_waypoint2);
}

bool jpsConnection::OccursInFrame(const int& frameID) const
{
    if (frameID>=_firstFrame && frameID<=_lastFrame)
    {
        return true;
    }
    else
        return false;
}

void jpsConnection::SetFirstFrame(const int &frameID)
{
    _firstFrame=frameID;
}

void jpsConnection::SetLastFrame(const int &frameID)
{
    _lastFrame=frameID;
}

const int &jpsConnection::GetFirstFrame() const
{
    return _firstFrame;
}

const int &jpsConnection::GetLastFrame() const
{
    return _lastFrame;
}

bool jpsConnection::operator == (const ptrConnection con2) const
{
    if ((con2->GetWaypoints().first==_waypoint1 && con2->GetWaypoints().second==_waypoint2)
            || (con2->GetWaypoints().second==_waypoint1 && con2->GetWaypoints().first==_waypoint2))
    {
        return true;
    }
    else
        return false;
}

