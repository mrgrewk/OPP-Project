#pragma once
#include <iostream>
#include <string>
#include "Position.h"

class WorldObject {
protected:
    Position pos;
    std::string id;
    char glyph; // The character shown on the map

public:
    // Constructor
    WorldObject(int x, int y, std::string myId, char myGlyph);

    // Destructor
    virtual ~WorldObject();

    // Getters
    Position getPosition() const;
    char getGlyph() const;
    std::string getId() const;

    virtual void update() = 0;
};