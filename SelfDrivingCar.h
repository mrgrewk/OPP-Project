#pragma once
#include "MovingObject.h"
#include "Sensor.h"
#include "NavigationSystem.h"
#include <vector>
#include <string>

class SelfDrivingCar : public MovingObject {
public:
    SelfDrivingCar(int worldW, int worldH, int x, int y, std::string id, const std::vector<Position>& gpsTargets, double minConf);

    ~SelfDrivingCar();

    void update() override;

    void perceive(const std::vector<WorldObject*>& objects);

    bool hasArrived() const {
        return destinationReached;
    }

private:
    std::vector<Sensor*> sensors;
    NavigationSystem* navSystem; // Pointer to the navigation system

    bool destinationReached;

    std::vector<SensorReading> collectSensorData(const std::vector<WorldObject*>& objects);
};