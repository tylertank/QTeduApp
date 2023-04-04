// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef WALL_H
#define WALL_H
#include "doorPosition.h"
#include <QImage>


//###########################################################################################
//                                         Wall
//
// The Wall class represents a single wall in a Room. There are getImage() and getRoom() methods
// for the wall so it can be drawn, as well as a getPos() method to get the wall's position.
//###########################################################################################

class Wall
{
public:
    Wall(doorPosition pos, int seed);
    QImage getImage();
    QRect getRect();
    doorPosition getPos();
    static const int wallWidth = 80;
    static const int canvasWidth = 1200;
    static const int canvasHeight = 880;
private:
    doorPosition position;
    QImage image;
    QRect rect;
};

#endif // WALL_H
