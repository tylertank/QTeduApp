// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "floor.h"

///
/// \brief Floor::Floor - this is the floor constructor
/// \param seed - this is a random seed to be used to generate different color floors
///
Floor::Floor(int seed)
{
    srand (time(nullptr)+seed);
    for (int i=0;i<21;i++) {
        srand(rand());
    }
    int index = rand() % 7;
    switch (index){
        case 0:
            image = QImage(":/floors/Floor_Blue.png");
            break;
        case 1:
            image = QImage(":/floors/Floor_Green.png");
            break;
        case 2:
            image = QImage(":/floors/Floor_Orange.png");
            break;
        case 3:
            image = QImage(":/floors/Floor_Pink.png");
            break;
        case 4:
            image = QImage(":/floors/Floor_Purple.png");
            break;
        case 5:
            image = QImage(":/floors/Floor_Red.png");
            break;
        case 6:
            image = QImage(":/floors/Floor_Yellow.png");
            break;
    }

    rect = image.rect();
    this->rect.moveTo(X, Y);
}

QRect Floor::getRect() {
    return rect;
}

QImage Floor::getImage() {
    return image;
}
