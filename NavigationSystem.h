#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "Position.h"
#include "SensorFusionEngine.h"

class NavigationSystem {
private:
    std::vector<Position> gpsTargets;
    std::size_t currentTargetIndex;
    
    SensorFusionEngine fusionEngine;
    std::vector<SensorReading> currentWorldState;

    // World Boundaries
    int worldWidth;
    int worldHeight;

    // Stop sign handling state
    std::string lastStoppedSignId;
    int stopTicksRemaining;

public:
    NavigationSystem(int w, int h, const std::vector<Position>& targets, double minConf);

    void syncNavigationSystem(const std::vector<SensorReading>& rawReadings);
    bool makeDecision(Position currentPos, int& desiredSpeed, int& desiredDirection);
};