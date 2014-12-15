#ifndef JPSOBSTACLE_H
#define JPSOBSTACLE_H

#include "rooms.h"

class jpsObstacle: public jpsRoom
{
public:
    jpsObstacle(int cId);
    void set_room(jpsRoom* croom);
    jpsRoom* get_room();
private:
    jpsRoom* _room;
};

#endif // JPSOBSTACLE_H
