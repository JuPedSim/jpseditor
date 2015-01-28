#include "jpsobstacle.h"

jpsObstacle::jpsObstacle(int id):jpsRoom(id)
{
    QString string="Obstacle ";
    string.append(QString::number(id));
    jpsRoom::change_name(string);
    _room=0L;
}

void jpsObstacle::set_room(jpsRoom *room)
{
    _room=room;
}

jpsRoom* jpsObstacle::get_room()
{
    return _room;
}
