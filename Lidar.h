#pragma once
#include "Sensor.h"
#include "MovingObject.h"
#include "TrafficLight.h"
#include <cmath>
#include <algorithm>

class Lidar : public Sensor {
public:
    Lidar() : Sensor(9, ACC_EXCELLENT) { } 

    std::vector<SensorReading> scan(const std::vector<WorldObject*>& objects, Position carPos, int carDir) override {
        std::vector<SensorReading> readings;

        (void)carDir;

        for (WorldObject* obj : objects) {
            if (obj->getPosition().x == carPos.x && obj->getPosition().y == carPos.y) {
                continue;
            }      

            int dx = std::abs(obj->getPosition().x - carPos.x);
            int dy = std::abs(obj->getPosition().y - carPos.y);

            // 360 degree 9x9 box
            if (dx <= 4 && dy <= 4) {
                int dist = std::max(dx, dy); 
                double conf = calculateConfidence(dist);
                
                SensorReading reading("Object", dist, obj->getPosition(), obj->getId(), conf);

                // Recognize object type
                if (dynamic_cast<TrafficLight*>(obj)) {
                    reading.type = "TrafficLight";
                }
                else if (MovingObject* mov = dynamic_cast<MovingObject*>(obj)) {
                    reading.type = "Vehicle";
                    reading.speed = mov->getSpeed();
                    reading.direction = mov->getDirection();
                }
                else {
                    reading.type = "Obstacle";
                }
                
                readings.push_back(reading);
            }
        }
        return readings;
    }
};