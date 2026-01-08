#include "SelfDrivingCar.h"
#include "Camera.h"
#include "Lidar.h"
#include "Radar.h"
#include <iostream>

using namespace std;

// Constructor
SelfDrivingCar::SelfDrivingCar(int worldW, int worldH, int x, int y, string id, const vector<Position>& gpsTargets, double minConf)
    : MovingObject(x, y, id, 'S', 1, 1)
{
    cout << "[+CAR:] Initialized at (" << x << ", " << y << ") facing " << direction 
         << " - No driver ' s license required !" << endl;

    // Set up sensors with logs
    cout << "[+CAMERA:] Camera sensor ready - Say cheese !" << endl;
    sensors.push_back(new Camera()); 
    
    cout << "[+LIDAR:] Lidar sensor ready– Sensing with pew pews!" << endl;
    sensors.push_back(new Lidar());  
    
    cout << "[+RADAR:] Radar sensor ready - I 'm a Radio star !" << endl;
    sensors.push_back(new Radar());  

    // Install the navigation system
    navSystem = new NavigationSystem(worldW, worldH, gpsTargets, minConf);
    cout << "[+NAV:] Hello , I ' ll be your GPS today" << endl;

    destinationReached = false;
}

// Destructor
SelfDrivingCar::~SelfDrivingCar() {
    delete navSystem;
    cout << "[ -NAV:] You' ve arrived ! Shutting down. . ." << endl;

    for (Sensor* s : sensors) {
        delete s;
    }
    sensors.clear();

    cout << "[ -CAR:] Our journey is complete !" << endl;
}

// Raw Data Collection
vector<SensorReading> SelfDrivingCar::collectSensorData(const vector<WorldObject*>& objects) {
    vector<SensorReading> allReadings;
    for (Sensor* sensor : sensors) {
        vector<SensorReading> readings = sensor->scan(objects, pos, direction);
        
        allReadings.insert(allReadings.end(), readings.begin(), readings.end());
    }

    return allReadings;
}

// Perception and Decision
void SelfDrivingCar::perceive(const vector<WorldObject*>& objects) {
    // Collect data
    vector<SensorReading> rawData = collectSensorData(objects);

    // Parse to NavigationSystem
    navSystem->syncNavigationSystem(rawData);

    int nextSpeed = speed;
    int nextDir = direction;
    
    bool finished = navSystem->makeDecision(pos, nextSpeed, nextDir);

    if (finished) {
        speed = 0;
        destinationReached = true;
    } 
    else {
        direction = nextDir;
        if (nextSpeed == 0) {
            speed = 0;
        } 
        else if (nextSpeed > speed) {
            speed += 1;
        } 
        else if (nextSpeed < speed) {
            speed -= 1;
        }
    }
}

// Drive
void SelfDrivingCar::update() {
    // If speed is 0, do nothing
    if (speed > 0) {
        MovingObject::update();
    }
}