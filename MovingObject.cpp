#include "MovingObject.h"
#include <iostream>

using namespace std;

// Constructor
MovingObject::MovingObject(int x, int y, string id, char glyph, int s, int d) : WorldObject(x, y, id, glyph), speed(s), direction(d) {
    if (id.find("Tesla_Bot") == string::npos) {
        cout << "[+VEHICLE:] Created at (" << x << ", " << y << ") , heading " << d << " at " << s << " units / tick" << endl;
    }
}

MovingObject::~MovingObject() {
    if (glyph == 'C') {
        cout << "[ -CAR:] Being scrapped . . ." << endl;
    } 
    else if (glyph == 'B') {
        cout << "[ -BIKE:] Being locked away . . ." << endl;
    }
}

void MovingObject::update() {
    move();
}

void MovingObject::move() {
    // If speed is greater than 1, we move multiple times
    for (int i = 0; i < speed; i++) {
        switch (direction) {
            case 0: pos.y++; break; // Up
            case 1: pos.x++; break; // Right
            case 2: pos.y--; break; // Down
            case 3: pos.x--; break; // Left
        }
    }
}