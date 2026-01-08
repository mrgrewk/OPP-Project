#pragma once
#include "WorldObject.h"

class MovingObject : public WorldObject {
protected:
    int speed;
    int direction; // 0 = Up, 1 = Right, 2 = Down, 3 = Left

public:
    // Constructor
    MovingObject(int x, int y, std::string id, char glyph, int s, int d);

    virtual ~MovingObject();

    void update() override;

    void move();

    // Getters
    int getSpeed() const {
        return speed;
    }
    
    int getDirection() const {
        return direction;
    }
};