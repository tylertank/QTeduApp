// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "player.h"

///
/// \brief Player::Player - this is a constructor for a player
///
Player::Player()
{
    x=roomWidth/2;
    y=roomHeight/2;
    facing=Forward;

    image = QImage(":/player/Character.png");
    rect = image.rect();
    this->rect.moveTo(x, y);
}

///
/// \brief Player::getPlayerPos - returns the x,y cordinate of the player in the game in the form of a Tuple<int>
/// \return - the first element in the tuple is the x position, the second is the y position
///
std::tuple<int,int> Player::getPlayerPos(){
    return std::tuple (this->x, this->y);
}

///
/// \brief Player::resetPlayerPos - this method resets the player to position to its default state when starting the game
///
void Player::resetPlayerPos(){
    x=roomWidth/2;
    y=roomHeight/2;
    facing=Forward;
    this->rect.moveTo(x, y);
}

///
/// \brief Player::move - this method either moves the player, or changes the orientation (way the player is facing)
/// \param direction - this is the direction that was pressed on the keyboard (up, down, left, right)
/// \return - returns nothing if just moving the player around the current room, or returns the door the player is attempting to enter
///
doorPosition Player::move(moveType direction) {
    if (direction == facing || !requireTurn) {
        facing = direction;
        if (direction==Left && x > 0) {
            x--;
        }
        else if (direction==Left && y==roomHeight/2) {
            return LeftDoor;
        }
        if (direction==Right && x < roomWidth-1) {
            x++;
        }
        else if (direction==Right &&y==roomHeight/2) {
            return RightDoor;
        }
        if (direction==Forward && y > 0) {
            y--;
        }
        else if (direction==Forward&&x==roomWidth/2) {
            return TopDoor;
        }
        if (direction==Backward && y < roomHeight-1) {
            y++;
        }
        else if (direction==Backward&&x==roomWidth/2) {
            return BottomDoor;
        }
    }
    if (direction != facing) {
        facing = direction;
    }
    return None;
}

///
/// \brief Player::enterNewRoom - this method gets the player coordinates when entering a new room
/// \param move - this is the move type the player used to enter the room, thus giving a hint of where the player should be in the next room
///
void Player::enterNewRoom(moveType move) {
    if (move==Left) {
        x = roomWidth-1;
        y = roomHeight/2;
    }
    else if (move==Right){
        x = 0;
        y = roomHeight/2;
    }
    else if (move==Forward) {
        x = roomWidth/2;
        y = roomHeight-1;
    }
    else if (move==Backward){
        x = roomWidth/2;
        y = 0;
    }
}

QRect Player::getRect(){
    this->rect.moveTo(80+80*x,80+80*y);
    return rect;
}

QImage & Player::getImage(){
    if (facing==Left)
        image = image.transformed(QTransform().rotate(-90));
    else if (facing==Right)
        image = image.transformed(QTransform().rotate(90));
    else if (facing==Forward)
        image = image.transformed(QTransform().rotate(0));
    else if (facing==Backward)
        image = image.transformed(QTransform().rotate(180));
    return image;
}
