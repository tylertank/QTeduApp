// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#ifndef PUZZLE_H
#define PUZZLE_H

#include <list>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <tuple>
class Puzzle
{
public:
    Puzzle(int nodes, int seed);
    int getSize();
    int weightAt(int x, int y);
    int getMinTreeValue();
private:
    int nodes;
    int minSpanSum;
    std::vector<std::vector<int>> weights;
};

#endif // PUZZLE_H
