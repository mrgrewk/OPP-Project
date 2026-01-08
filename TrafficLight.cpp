#include "TrafficLight.h"
#include <iostream>

using namespace std;

TrafficLight::TrafficLight(int x, int y, string id) : StaticObject(x, y, id, 'R') {
    state = RED;
    timer = 0;
    cout << "[+LIGHT:] Initialized at (" << x << ", " << y << ") to " << state << endl;
}

TrafficLight::~TrafficLight() {
    cout << "[ -LIGHT:] Turning off" << endl;
}

void TrafficLight::update() {
    timer++;

    // Color change algorithm: Red(4) -> Green(8) -> Yellow(2) -> Red
    if (state == RED) {
        if (timer >= 4) {
            state = GREEN;
            glyph = 'G';
            timer = 0;
        }
    }
    else if (state == GREEN) {
        if (timer >= 8) {
            state = YELLOW;
            glyph = 'Y';
            timer = 0;
        }
    }
    else if (state == YELLOW) {
        if (timer >= 2) {
            state = RED;
            glyph = 'R';
            timer = 0;
        }
    }
}