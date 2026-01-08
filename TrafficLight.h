#pragma once
#include "StaticObject.h"

class TrafficLight : public StaticObject {
public:
    enum Color {RED, GREEN, YELLOW};

private:
    Color state; // Current Color
    int timer;   // Time Counter

public:
    // Constructor
    TrafficLight(int x, int y, std::string id);
    
    // Destructor
    ~TrafficLight();

    void update() override;

    // Color Getter
    Color getColor() {
        return state;
    }
};