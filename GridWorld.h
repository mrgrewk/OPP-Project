#pragma once
#include <vector>
#include "WorldObject.h"

class GridWorld {
private:
    int width;
    int height;

    std::vector<WorldObject*> objects;

public:
    // Constructor
    GridWorld(int w, int h);

    // Destructor
    ~GridWorld();

    void addObject(WorldObject* obj);

    void update();

    void display();

    void displayPOV(Position center, int radius);

    bool isInsideBounds(Position pos);
};