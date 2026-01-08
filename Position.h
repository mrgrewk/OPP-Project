#pragma once

struct Position {
    int x;
    int y;

    // Constructor, initializes x and y (default values are 0)
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};