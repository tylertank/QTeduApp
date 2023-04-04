// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef DOOR_H
#define DOOR_H

#include <vector>
#include "doorPosition.h"
#include "doorType.h"
#include "puzzle.h"
#include <QImage>

//#####################################################################
//                   DOOR
//
// Doors store a parent room id, a child room id, a door position
// (an enum representing the position of the door in the parent room),
// and a boolean isLocked storing whether or not the door has a lock
// on it (that a puzzle needs to be solved to unlock)
//
// Door also has a static getImage function to return a QImage based on the passed in doorType.
//#####################################################################
class Door
{
public:
    Door(int pRoom, int cRoom, doorPosition pos, bool isLocked, int seed, bool visible);
    doorPosition parentPosition();
    static QImage getImage(doorType);
    void setVisible(bool);

    int parentRoom;
    int childRoom;
    doorPosition position; // This is the position of the door in the Parent Room
    Puzzle puzzle;
    bool visible;
    bool locked;
};

#endif // DOOR_H
