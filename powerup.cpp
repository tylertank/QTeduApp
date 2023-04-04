// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "powerup.h"

///
/// \brief powerUp::powerUp - the constructor for the powerup
/// \param ID - the powerup ID
/// \param roomID - the id of the room the powerup resides in
/// \param x - the x coord of the powerup
/// \param y - the y coord of the powerup
/// \param died - a variable used to see if powerup has been picked up or not
/// \param visable - a variable used to see if powerup should be visible in the current room
///
powerUp::powerUp(int ID,int roomID, int x, int y, bool died, bool visable)
{
    this->pID = ID;
    this->roomID = roomID;
    this->x = x;
    this->y = y;
    this->died = died;
    this->visable = visable;

    image = QImage(":/powerups/Powerup_Clock.png");
    rect.setX( x*80);
    rect.setY( y*80);
}

QRect powerUp::getRect(){
    return rect;
}

QImage powerUp::getImage(){
    return image;
}
