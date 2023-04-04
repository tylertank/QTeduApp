// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef DOORTYPE_H
#define DOORTYPE_H
//#################################################################
//                          DoorType
//
// The doorType enum is created as a convenient way to distinguish
// the visual appearance of a door in a room for drawing.
//
// Empty: There is no door to be drawn at this position.
//
// Flush: This door should be drawn as a Flush door (this door leads from this room to another room)
//
// FlushLocked: This door leads from this room to another room and is locked.
//
// NotFlush: This door leads from another room into this room.
//#################################################################
enum doorType {Empty, Flush, FlushLocked, NotFlush};
#endif // DOORTYPE_H
