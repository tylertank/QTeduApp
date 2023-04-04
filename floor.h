// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef FLOOR_H
#define FLOOR_H
//#################################################################
//                           Floor
// Unimplemented. Will be used for tiling the floor.
// Each Room instance will have its own Floor instance.
// The getImage method will assemble the floor's image and return it.
// (I am thinking some kind of tiling can be done here to create a unique
// and cool appearance)
//#################################################################
#include <QImage>
class Floor
{
public:
    Floor(int seed);
    QImage getImage();
    QRect getRect();
private:
    QImage image;
    QRect rect;
    static const int width=1040;
    static const int height=720;
    static const int X=80;
    static const int Y=80;
};
#endif // FLOOR_H
