#pragma once
#include "Sensor.h"
#include "TrafficLight.h"
#include "MovingObject.h"

class Camera : public Sensor {
public:
    Camera() : Sensor(7, ACC_HIGH) { }

    std::vector<SensorReading> scan(const std::vector<WorldObject*>& objects, Position carPos, int carDir) override {
        std::vector<SensorReading> readings;

        for (WorldObject* obj : objects) {
            if (obj->getPosition().x == carPos.x && obj->getPosition().y == carPos.y) {
                continue;
            }

            int dx = obj->getPosition().x - carPos.x;

            bool visible = false;
            int dist = dx;

            // Simple Field of View logic (Forward square)
            if (carDir == 1) { // Right
                 if (obj->getPosition().x > carPos.x && std::abs(obj->getPosition().y - carPos.y) <= 3 && (obj->getPosition().x - carPos.x) <= range) {
                     visible = true;
                     dist = obj->getPosition().x - carPos.x;
                 }
            } 
            else if (carDir == 3) { // Left
                 if (obj->getPosition().x < carPos.x && std::abs(obj->getPosition().y - carPos.y) <= 3 && (carPos.x - obj->getPosition().x) <= range) {
                     visible = true;
                     dist = carPos.x - obj->getPosition().x;
                 }
            } 
            else if (carDir == 0) { // Up
                 if (obj->getPosition().y > carPos.y && std::abs(obj->getPosition().x - carPos.x) <= 3 && (obj->getPosition().y - carPos.y) <= range) {
                     visible = true;
                     dist = obj->getPosition().y - carPos.y;
                 }
            } 
            else if (carDir == 2) { // Down
                 if (obj->getPosition().y < carPos.y && std::abs(obj->getPosition().x - carPos.x) <= 3 && (carPos.y - obj->getPosition().y) <= range) {
                     visible = true;
                     dist = carPos.y - obj->getPosition().y;
                 }
            }

            if (visible) {
                double conf = calculateConfidence(dist);
                SensorReading reading("Object", dist, obj->getPosition(), obj->getId(), conf);

                TrafficLight* tl = dynamic_cast<TrafficLight*>(obj);

                if (tl) {
                    reading.type = "TrafficLight";
                    reading.trafficLight = tl->getGlyph();
                }
                else if (dynamic_cast<MovingObject*>(obj)) {
                    reading.type = "Vehicle";
                }

                readings.push_back(reading);
            }
        }
        return readings;
    }
};