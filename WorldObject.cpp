#include "WorldObject.h"

using namespace std;

// Constructor
WorldObject::WorldObject(int x, int y, string myId, char myGlyph) : pos(x,y), id(myId), glyph(myGlyph) { }

WorldObject::~WorldObject() { }

// Getters
Position WorldObject::getPosition() const {
    return pos;
}

char WorldObject::getGlyph() const {
    return glyph;
}

string WorldObject::getId() const {
    return id;
}