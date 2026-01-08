#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "Position.h"
#include "WorldObject.h"
#include "MovingObject.h"
#include "TrafficLight.h"

const double ACC_EXCELLENT = 0.99;
const double ACC_HIGH = 0.95;

struct SensorReading {
    std::string type;
    int distance;
    Position position;
    std::string objectId;
    double confidence;

    int speed;
    int direction;
    char trafficLight;

    SensorReading(std::string t, int d, Position p, std::string id, double conf) : type(t), distance(d), position(p), objectId(id),confidence(conf), speed(0), direction(-1), trafficLight(' ') { }
};

class Sensor {
protected:
    int range;
    double baseAccuracy;

    // Accuracy calculation
    double calculateConfidence(int dist) {
        double distFactor = 1.0 - ((double)dist / range);
        if (distFactor < 0) {
            distFactor = 0;
        }

        double val = baseAccuracy * distFactor;

        // Random noise
        double noise = ((rand() % 100) / 1000.0) - 0.05;

        val += noise;

        if (val > 1.0) {
            val = 1.0;
        }

        if (val < 0.0) {
            val = 0.0;
        }

        return val;
    }

public:
    Sensor(int r, double acc) : range(r), baseAccuracy(acc) { }
    
    virtual ~Sensor() { 
        std::cout << "[ -SENSOR:] Sensor destroyed - No further data from me!" << std::endl;
    }

    virtual std::vector<SensorReading> scan(const std::vector<WorldObject*>& objects, Position carPos, int carDir) = 0;

    int getRange() const {
        return range;
    }
};