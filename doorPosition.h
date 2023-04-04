// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef DOORPOSITION_H
#define DOORPOSITION_H
//#################################################################
//                      DoorPosition
//
// The purpose of the doorPosition enum is create a convenient way
// to store a door's position in the parent room. The 'None' value is
// provided so that the Player class can specify that no door was entered
// from a move action.
//
//                           TopDoor
//            +---------------[===]--------------+
//            |                                  |
//            |                                  |
//            |                                  |
//            -                                  -
//  LeftDoor |||                                |||  RightDoor
//            -                                  -
//            |                                  |
//            |                                  |
//            |                                  |
//            +---------------[===]--------------+
//                          BottomDoor
//#################################################################

enum doorPosition{LeftDoor, RightDoor, TopDoor, BottomDoor, None};

#endif // DOORPOSITION_H
