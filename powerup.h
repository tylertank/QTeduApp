// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef POWERUP_H
#define POWERUP_H
#include <QMainWindow>


class powerUp
{
public:
    powerUp(int ID,int roomID, int x, int y, bool died, bool visable);
    powerUp();
    int pID;
    int roomID;
    int x;
    int y;
    bool died;
    bool visable;
    bool pickup(int x, int y);
    QRect getRect();
    QImage getImage();

private:
    QRect rect;
    QImage image;
};

#endif // POWERUP_H
