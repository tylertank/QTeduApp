// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "room.h"

///
/// \brief Room::Room - this is the room constructor
/// \param id - the id of the room, which will then also be the id of the floor in the room
///
Room::Room(int id) : floor(id)
{
    _ID = id;
    walls.push_back(new Wall(LeftDoor, 4*id+1));
    walls.push_back(new Wall(RightDoor, 4*id+2));
    walls.push_back(new Wall(TopDoor, 4*id+3));
    walls.push_back(new Wall(BottomDoor, 4*id+4));
    xOffset = 0;
    yOffset = 0;
    visited = false;
}

void Room::addChildDoor(int child, doorPosition pos, bool locked) {
    srand(time(nullptr) + 100*child+_ID);
    doors.push_back(new Door(_ID, child, pos, locked, rand(), true));
}

void Room::addParentDoor(int parent, doorPosition pos) {
    srand(time(nullptr) + 100*_ID + parent);
    doors.push_back(new Door(parent, _ID, pos, false, rand(), true));
}

///
/// \brief Room::unlock - this method unlocks a door when a puzzle has been solved
/// \param pos - the position of the door to be unlocked
///
void Room::unlock(doorPosition pos) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            d->locked=false;
        }
    }
}

///
/// \brief Room::isLocked - this returns whether or not a door is locked
/// \param pos - the door being checked
/// \return - returns the status of the door lock. True for locked, false if not
///
bool Room::isLocked(doorPosition pos) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            return d->locked;
        }
    }
    return false;
}

///
/// \brief Room::getDoorsToDraw - this returns the state of the door to be drawn as either flushed to the wall or not
/// \return - returns a vector of doors to be drawn
///
std::vector<doorType> Room::getDoorsToDraw() {
    std::vector<doorType> returned;
    for(int i=0;i<4;i++) {
        returned.push_back(Empty);
    }
    for (Door* d : doors) {
        int idx = -1;
        switch (d->position) {
        case LeftDoor:
            idx = d->parentRoom==_ID ? 0:2;
            break;
        case RightDoor:
            idx = d->parentRoom==_ID ? 2:0;
            break;
        case TopDoor:
            idx = d->parentRoom==_ID ? 1:3;
            break;
        case BottomDoor:
            idx = d->parentRoom==_ID ? 3:1;
            break;
        case None:
            break;
        }
        if (d->parentRoom == _ID)
            returned[idx] = d->locked ? FlushLocked : Flush;
        else {
            returned[idx] = NotFlush;
        }
    }
    return returned;
}

std::vector<Wall*> Room::getWalls() {
    return walls;
}

int Room::getID() {
    return _ID;
}

std::vector<Door*> Room::getAllDoors(){
    return doors;
}

///
/// \brief Room::getAdjRoomID - this gets the room id that you are trying to enter
/// \param pos - the position of the door you are entering
/// \return - returns the id of the room, or -1 if there is no room id to be returned
///
int Room::getAdjRoomID(doorPosition pos) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            return d->childRoom;
        }
        if (d->parentPosition()==pos&&d->childRoom==_ID) {
            return d->parentRoom;
        }
    }
    return -1;
}

QRect Room::getRect(){
    return floor.getRect();
}

QImage Room::getImage(){
    return floor.getImage();
}

///
/// \brief Room::getPuzzleSize - this gets the number of nodes from the puzzle
/// \param pos - this is the door that you are trying to enter
/// \return - returns the number of nodes, or -1 if there is no puzzle nodes to be returned
///
int Room::getPuzzleSize(doorPosition pos) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            return d->puzzle.getSize();
        }
    }
    return -1;
}

///
/// \brief Room::getPuzzleMinValue - returns the weight of the minimum spanning tree through that door
/// \param pos - the position of the door you are trying to enter
/// \return - returns the minimum weight of the tree, or -1 if there is no puzzle there
///
int Room::getPuzzleMinValue(doorPosition pos) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            return d->puzzle.getMinTreeValue();
        }
    }
    return -1;
}

///
/// \brief Room::getNodeWeight - gets the weight of the edge between nodes x and y
/// \param pos - the door you are trying to enter (tied to the puzzle)
/// \param x - the first nodes
/// \param y - the second node
/// \return - the weight of the edge, or -1 if there is no edge
///
int Room::getNodeWeight(doorPosition pos, int x, int y) {
    for (Door* d : doors) {
        if(d->position==pos && d->parentRoom == _ID) {
            return d->puzzle.weightAt(x, y);
        }
    }
    return -1;
}

///
/// \brief Room::getLastAddedDoor - gets the last door that was added to the doors vector. Presumable the last door you entered
/// \return - this is the door you entered
///
Door* Room::getLastAddedDoor(){
    return doors.at(doors.size() - 1);
}
