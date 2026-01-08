#pragma once
#include "Sensor.h"
#include "MovingObject.h"
#include <cmath>

class Radar : public Sensor {
public:
    Radar() : Sensor(12, ACC_EXCELLENT) { } 

    std::vector<SensorReading> scan(const std::vector<WorldObject*>& objects, Position carPos, int carDir) override {
        std::vector<SensorReading> readings;

        for (WorldObject* obj : objects) {
            // If object is null, continue
            if (obj == nullptr) {
                continue;
            }    
            
            MovingObject* mov = dynamic_cast<MovingObject*>(obj);
            
            if (mov == nullptr || (mov->getPosition().x == carPos.x && mov->getPosition().y == carPos.y)) {
                continue;
            }    

            bool visible = false;
            int dist = 0;
            
            int dx = obj->getPosition().x - carPos.x;
            int dy = obj->getPosition().y - carPos.y;

            switch (carDir) {
                case 0: // Up
                    if (dx == 0 && dy > 0 && dy <= range) {
                        visible = true;
                        dist = dy;
                    }
                    break;
                    
                case 1: // Right
                    if (dy == 0 && dx > 0 && dx <= range) {
                        visible = true;
                        dist = dx;
                    }
                    break;
                    
                case 2: // Down
                    if (dx == 0 && dy < 0 && std::abs(dy) <= range) {
                        visible = true;
                        dist = std::abs(dy);
                    }
                    break;
                    
                case 3: // Left
                    if (dy == 0 && dx < 0 && std::abs(dx) <= range) {
                        visible = true;
                        dist = std::abs(dx);
                    }
                    break;
            }

            if (visible) {
                double conf = calculateConfidence(dist);
                SensorReading reading("Vehicle", dist, mov->getPosition(), mov->getId(), conf);
                reading.speed = mov->getSpeed();
                reading.direction = mov->getDirection();
                readings.push_back(reading);
            }
        }
        return readings;
    }
};