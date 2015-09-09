#ifndef JPSCONNECTION_H
#define JPSCONNECTION_H

#include <memory>
#include <utility>
#include "jpswaypoint.h"

using ptrWaypoint = std::shared_ptr<jpsWaypoint>;


class jpsConnection
{
using ptrConnection = std::shared_ptr<jpsConnection>;
public:
    jpsConnection(ptrWaypoint waypoint1, ptrWaypoint waypoint2, const int& frameID=-1);
    ~jpsConnection();
    std::pair<ptrWaypoint,ptrWaypoint> GetWaypoints() const;
    //Occurence
    bool OccursInFrame(const int &frameID) const;
    void SetFirstFrame(const int &frameID);
    void SetLastFrame(const int &frameID);
    const int& GetFirstFrame() const;
    const int& GetLastFrame() const;
    // Operator overload
    bool operator ==(const ptrConnection con2) const;
private:
    ptrWaypoint _waypoint1;
    ptrWaypoint _waypoint2;
    int _firstFrame;
    int _lastFrame;
};

#endif // JPSCONNECTION_H
