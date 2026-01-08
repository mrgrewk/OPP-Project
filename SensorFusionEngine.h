#pragma once
#include <vector>
#include <map>
#include <string>
#include "Sensor.h"

class SensorFusionEngine {
private:
    double minConfidenceThreshold;

public:
    // Constructor
    SensorFusionEngine(double threshold = 0.40);

    std::vector<SensorReading> fuseSensorData(const std::vector<SensorReading>& rawReadings);
};