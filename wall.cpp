// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "wall.h"

///
/// \brief Wall::Wall - the constructor for the wall class
/// \param pos - this is the position of the wall side
/// \param seed - a random seed used for generating the walls, since C++'s random generator is sudo-random.
///
Wall::Wall(doorPosition pos, int seed)
{
    position = pos;
    if (pos == LeftDoor) {
        image = QImage(":/walls/Wall_Vertical.png");
        image = image.scaledToHeight(canvasHeight);
        image = image.scaledToWidth(wallWidth);
        rect = image.rect();
        rect.moveTo(0,0);
    }
    else if (pos == RightDoor) {
        image = QImage(":/walls/Wall_Vertical.png");
        image = image.scaledToHeight(canvasHeight);
        image = image.scaledToWidth(wallWidth);
        rect = image.rect();
        rect.moveTo(canvasWidth-wallWidth, 0);
    }
    else if (pos == TopDoor) {
        image = QImage(":/walls/Wall_Horizontal.png");
        image = image.scaledToHeight(wallWidth);
        image = image.scaledToWidth(canvasWidth - 2 * wallWidth);
        rect = image.rect();
        rect.moveTo(wallWidth, 11);
    }
    else if (pos == BottomDoor) {
        image = QImage(":/walls/Wall_Horizontal.png");
        image = image.scaledToHeight(wallWidth);
        image = image.scaledToWidth(canvasWidth - 2 * wallWidth);
        rect = image.rect();
        rect.moveTo(wallWidth, canvasHeight - wallWidth);
    }
    srand(time(nullptr)-seed);
    for (int i=0;i<21;i++) {
        srand(rand());
    }
}

QImage Wall::getImage() {
    return image;
}

QRect Wall::getRect() {
    return rect;
}

///
/// \brief Wall::getPos - returns the side of the wall in the room
/// \return - the wall side
///
doorPosition Wall::getPos() {
    return position;
}
