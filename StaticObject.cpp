#include "StaticObject.h"
#include <iostream>

using namespace std;

StaticObject::StaticObject(int x, int y, std::string id, char glyph) : WorldObject(x, y, id, glyph) {
    if (glyph == 'P') {
        cout << "[+PARKED:] Parked at (" << x << ", " << y << ")" << endl;
    }
}

StaticObject::~StaticObject() {
    if (glyph == 'P') {
        cout << "[ -PARKED:] I 'm being towed away!" << endl;
    }
}

void StaticObject::update() { }