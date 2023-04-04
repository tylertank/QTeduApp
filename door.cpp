// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "door.h"

///
/// \brief Door::Door - this is the Door constructor
/// \param pRoom - parent room id
/// \param cRoom - child room id
/// \param pos - the position of the door being top, right, left, or bottom
/// \param isLocked - this is whether or not the room is locked
/// \param seed - this is a random seed used for making random puzzles "between" locked rooms
/// \param visible - this is a bool used for the minimap to state if it is visible or not
///
Door::Door(int pRoom, int cRoom, doorPosition pos, bool isLocked, int seed, bool visible): puzzle(seed % 5+3, seed)
{
    parentRoom = pRoom;
    childRoom = cRoom;
    position = pos;
    locked = isLocked;
    this->visible = visible;
}

///
/// \brief Door::setVisible - this method sets the visible method based on the given parameter
/// \param visible - a bool representing true if the room has been visited
///
void Door::setVisible(bool visible){
    this->visible = visible;
}

///
/// \brief Door::parentPosition - this method returns the doorPosition of a parent class
/// \return - returns the state of the position variable
///
doorPosition Door::parentPosition() {
    switch (position) {
    case LeftDoor:
        return RightDoor;
        break;
    case RightDoor:
        return LeftDoor;
        break;
    case TopDoor:
        return BottomDoor;
        break;
    case BottomDoor:
        return TopDoor;
        break;
    case None:
        return None;
    }
}

///
/// \brief Door::getImage - this method gets the image for the door used for drawing
/// \param dType - this is the kind of doorType image you are trying to get
/// \return - returns the door to be drawn
///
QImage Door::getImage(doorType dType) {
    if (dType==Flush)
        return QImage(":/doors/Door_Unlocked");
    else if (dType==FlushLocked)
        return QImage(":/doors/Door_Locked");
    else if (dType==NotFlush)
        return QImage(":/doors/Door_Unlocked");
    else {
        QImage empty = QImage(80, 80, QImage::Format_ARGB32);
        empty.fill(0x00000000);
        return empty;
    }
}
