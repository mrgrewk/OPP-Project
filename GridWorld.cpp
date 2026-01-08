#include "GridWorld.h"
#include "SelfDrivingCar.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Constructor
GridWorld::GridWorld(int w, int h) : width(w), height(h) {
    cout << "[+WORLD:] Reticulating splines - Hello , world !" << endl;
}

// Destructor
GridWorld::~GridWorld() {
    for (WorldObject* obj : objects) {
        delete obj;
    }
    objects.clear();
    cout << "[ -WORLD:] Goodbye, cruel world !" << endl;
}

void GridWorld::addObject(WorldObject* obj) {
    objects.push_back(obj);
}

bool GridWorld::isInsideBounds(Position pos) {
    return (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height);
}

void GridWorld::update() {
    // Update the Smart Car so it can perceive
    for (WorldObject* obj : objects) {
        SelfDrivingCar* smartCar = dynamic_cast<SelfDrivingCar*>(obj);
        if (smartCar != nullptr) {
            smartCar->perceive(objects);
        }
    }

    // Move all objects
    for (auto it = objects.begin(); it != objects.end(); ) {
        (*it)->update();
        
        // Check if out of bounds, then it should be removed
        MovingObject* mov = dynamic_cast<MovingObject*>(*it);
        SelfDrivingCar* car = dynamic_cast<SelfDrivingCar*>(*it);
        
        if (mov != nullptr && car == nullptr && !isInsideBounds((*it)->getPosition())) {
             delete *it;
             it = objects.erase(it);
        } else {
             ++it;
        }
    }
}

void GridWorld::display() {
    cout << "--- FULL MAP ---" << endl;
    for (int y = height - 1; y >= 0; y--) { // Draw from top to bottom
        for (int x = 0; x < width; x++) {
            char symbol = '.';
            // Priority render: Car > Light/Stop > Moving > Parked
            for (WorldObject* obj : objects) {
                if (obj->getPosition().x == x && obj->getPosition().y == y) {
                    if (dynamic_cast<SelfDrivingCar*>(obj)) {
                         symbol = '@';
                         break;
                    }
                    symbol = obj->getGlyph();
                }
            }
            cout << symbol << " ";
        }
        cout << endl;
    }
}

void GridWorld::displayPOV(Position center, int radius) {
    cout << "--- POV (" << center.x << "," << center.y << ") ---" << endl;
    
    // Calculate window bounds
    int startY = center.y + radius;
    int endY = center.y - radius;
    int startX = center.x - radius;
    int endX = center.x + radius;

    for (int y = startY; y >= endY; y--) {
        for (int x = startX; x <= endX; x++) {
            
            // Check world bounds
            if (x < 0 || x >= width || y < 0 || y >= height) {
                cout << "X "; // Outside bounds
                continue;
            }

            char symbol = '.';
            for (WorldObject* obj : objects) {
                if (obj->getPosition().x == x && obj->getPosition().y == y) {
                    if (dynamic_cast<SelfDrivingCar*>(obj)) {
                        symbol = '@';
                        break;
                    }
                    symbol = obj->getGlyph();
                }
            }
            cout << symbol << " ";
        }
        cout << endl;
    }
}