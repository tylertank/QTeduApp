// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef PLAYER_H
#define PLAYER_H
#include <QImage>
#include <QRect>
#include "moveType.h"
#include "doorPosition.h"

//###########################################################################################
//                                        Player
//
// The Player class completely represents a player in the game. An instance of Player
// is stored by the model and updated correctly when keys are pressed and new rooms are entered.
// Player can return a QImage for the View to draw, as well as a QRect. It also returns a doorPosition
// of the new Door to move into (if any) when the move() method is called.
//###########################################################################################

class Player
{
public:
    Player();
    doorPosition move(moveType); // returns position of door to move into, None if no change
    QRect getRect();
    QImage & getImage();
    void enterNewRoom(moveType);
    void resetPlayerPos();
    std::tuple<int,int> getPlayerPos();
private:
    int x;
    int y;
    static const int width = 80;
    static const int height = 80;
    static const int roomWidth = 13;
    static const int roomHeight = 9;
    static const bool requireTurn = true; // True if the player must turn before moving
    moveType facing;
    QImage image;
    QRect rect;

};

#endif // PLAYER_H
