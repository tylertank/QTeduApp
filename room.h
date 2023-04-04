// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef ROOM_H
#define ROOM_H
#include "wall.h"
#include "floor.h"
#include "door.h"
#include <vector>
#include <QImage>
#include <QRect>
#include <time.h>
#include "doorType.h"

#include "puzzle.h"
#include <iostream>

//###########################################################################################
//                                          Room
// Room is one of the most important classes in the project. A room fully stores all
// components needed to correctly draw itself. It stores its floor, its walls, its doors,
// and its size. It has appropriate methods to return its Doors and Walls for drawing purposes.
// Rooms can be connected to other rooms using the addChildDoor and addParentDoor methods, which
// will create a new Door and add it to the door vector.
// The getDoorsToDraw() method returns a doorType vector of length 4, detailing the type of door
// to be drawn at each position in the following order: <LeftDoor, TopDoor, RightDoor, BottomDoor>.
//
// DONE: Moved the getRect() and getImage methods from this class to the Floor class.
// ##########################################################################################

class Room
{
public:
    Room(int id);
    void addChildDoor(int child, doorPosition pos, bool locked);
    void addParentDoor(int parent, doorPosition pos);
    void unlock(doorPosition pos);
    int getAdjRoomID(doorPosition pos);
    int getID();
    int getPuzzleSize(doorPosition pos);
    int getPuzzleMinValue(doorPosition pos);
    int getNodeWeight(doorPosition pos, int x, int y);
    std::vector<doorType> getDoorsToDraw(); // Left, Top, Right, Down
    std::vector<Wall*> getWalls();
    std::vector<Door*> getAllDoors();
    QRect getRect();
    QImage getImage();
    bool isLocked(doorPosition pos);
    Door* getLastAddedDoor();

    int xOffset;
    int yOffset;
    bool visited;
private:
    int _ID;
    std::vector<Door*> doors;
    std::vector<Wall*> walls;
    Floor floor;
    static const int width = 1040;
    static const int height = 720;
    static const int X = 80;
    static const int Y = 80;
    QImage image;
    QRect rect;
};

#endif // ROOM_H
