#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "GridWorld.h"
#include "SelfDrivingCar.h"
#include "TrafficLight.h"
#include "StaticObject.h"
#include "Position.h"

using namespace std;

void printHelp() {
    cout << "Self -Driving Car Simulation" << endl;
    cout << "Usage: " << endl;
    cout << " --seed <int> Random seed ( default : current time)" << endl;
    cout << " --dimX <int> World width ( default : 40)" << endl;
    cout << " --dimY <int> World height ( default : 40)" << endl;
    cout << " --numMovingCars <int> Number of moving cars ( default : 3)" << endl;
    cout << " --numMovingBikes <int> Number of moving bikes ( default : 4)" << endl;
    cout << " --numParkedCars <int> Number of parked cars ( default : 5)" << endl;
    cout << " --numStopSigns <int> Number of stop signs (default : 2)" << endl;
    cout << " --numTrafficLights <int> Number of traffic lights ( default : 2)" << endl;
    cout << " --simulationTicks <int> Maximum simulation ticks ( default : 100)" << endl;
    cout << " --minConfidenceThreshold <double> Minimum confidence cutoff ( default : 0.40)" << endl;
    cout << " --gps [x2 y2 . . .] GPS target coordinates (required)" << endl;
    cout << " --help Show this help message" << endl << endl;
    cout << "Example usage: " << endl;
    cout << " ./ oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
}

int main(int argc, char* argv[]) {
    // Default values
    int seed = time(0);
    int dimX = 40;
    int dimY = 40;
    int numMovingCars = 3;
    int numMovingBikes = 4;
    int numParkedCars = 5;
    int numStopSigns = 2;
    int numTrafficLights = 2;
    int simTicks = 100;
    double minConfidence = 0.40;
    vector<Position> gpsTargets;
    vector<pair<int,int>> gpsRaw;

    // Argument parsing
    if (argc < 2) {
        printHelp();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help") {
            printHelp();
            return 0;
        } 
        else if (arg == "--seed" && i + 1 < argc) {
            seed = atoi(argv[++i]);
        } 
        else if (arg == "--dimX" && i + 1 < argc) {
            dimX = atoi(argv[++i]);
        } 
        else if (arg == "--dimY" && i + 1 < argc) {
            dimY = atoi(argv[++i]);
        } 
        else if (arg == "--numMovingCars" && i + 1 < argc) {
            numMovingCars = atoi(argv[++i]);
        }
        else if (arg == "--numMovingBikes" && i + 1 < argc) {
            numMovingBikes = atoi(argv[++i]);
        }
        else if (arg == "--numParkedCars" && i + 1 < argc) {
            numParkedCars = atoi(argv[++i]);
        }
        else if (arg == "--numStopSigns" && i + 1 < argc) {
            numStopSigns = atoi(argv[++i]);
        }
        else if (arg == "--numTrafficLights" && i + 1 < argc) {
            numTrafficLights = atoi(argv[++i]);
        }
        else if (arg == "--simulationTicks" && i + 1 < argc) {
            simTicks = atoi(argv[++i]);
        }
        else if (arg == "--minConfidenceThreshold" && i + 1 < argc) {
            minConfidence = atof(argv[++i]);
        }
        else if (arg == "--gps") {
            // Collect subsequent non-flag arguments as coordinate pairs (defer validation)
            vector<int> coords;
            while (i + 1 < argc && argv[i+1][0] != '-') {
                coords.push_back(atoi(argv[++i]));
            }
            if (coords.size() % 2 != 0) {
                cout << "Error: --gps requires pairs of coordinates" << endl;
                return 1;
            }
            for (size_t k = 0; k < coords.size(); k += 2) {
                gpsRaw.push_back({coords[k], coords[k+1]});
            }
        }
    }

    // Validate collected GPS coordinates against final world dimensions
    if (gpsRaw.empty()) {
        cout << "Error: No GPS targets provided!" << endl;
        return 1;
    }
    
    for (auto &pp : gpsRaw) {
        int x = pp.first;
        int y = pp.second;
        if (x < 0 || x >= dimX || y < 0 || y >= dimY) {
            cout << "Error: GPS target (" << x << "," << y << ") is out of bounds for world size " << dimX << "x" << dimY << endl;
            return 1;
        }
        gpsTargets.push_back(Position(x, y));
    }

    // Initialization
    srand(seed);
    GridWorld myWorld(dimX, dimY);

    // Create Car
    SelfDrivingCar* myCar = new SelfDrivingCar(dimX, dimY, 0, 0, "Tesla_Bot", gpsTargets, minConfidence);
    myWorld.addObject(myCar);
    
    // Create Traffic Lights
    for (int i = 0; i < numTrafficLights; i++) {
        int rx = rand() % dimX;
        int ry = rand() % dimY;
        myWorld.addObject(new TrafficLight(rx, ry, "TL_" + to_string(i)));
    }

    // Create Moving Cars
    for (int i = 0; i < numMovingCars; i++) {
        int rx = rand() % dimX;
        int ry = rand() % dimY;
        int dir = rand() % 4;

        if (rx == 0 && ry == 0) {
        continue;
        }

        myWorld.addObject(new MovingObject(rx, ry, "CAR:" + to_string(i), 'C', 1, dir));
    }

    // Create Moving Bikes
    for(int i = 0; i < numMovingBikes; i++) {
        int rx = rand() % dimX;
        int ry = rand() % dimY;
        int dir = rand() % 4;

        if (rx == 0 && ry == 0) {
            continue;
        }

        myWorld.addObject(new MovingObject(rx, ry, "BIKE:" + to_string(i), 'B', 1, dir));
    }

    // Create Parked Cars
    for(int i = 0; i < numParkedCars; i++) {
        int rx = rand() % dimX;
        int ry = rand() % dimY;

        if (rx == 0 && ry == 0) {
            continue;
        }

        myWorld.addObject(new StaticObject(rx, ry, "PARKED:" + to_string(i), 'P'));
    }

    // Create Stop Signs
    for(int i = 0; i < numStopSigns; i++) {
        int rx = rand() % dimX;
        int ry = rand() % dimY;

        if (rx == 0 && ry == 0) {
            continue;
        }
            
        myWorld.addObject(new StaticObject(rx, ry, "STOP:" + to_string(i), 'S'));
    }

    // Simulation loop
    cout << "\nStarting Simulation..." << endl;
    myWorld.display(); // Full map at start

    for (int t = 0; t < simTicks; t++) {
        cout << "\n=== TICK " << t << " ===" << endl;
        
        myWorld.update();

        // Check if car went out of bounds
        if (!myWorld.isInsideBounds(myCar->getPosition())) {
            cout << "Car moved out of bounds! Simulation Over." << endl;
            break;
        }
        
        if (myCar->hasArrived()) {
            cout << "\n>>> MISSION ACCOMPLISHED! Car parked safely. <<<" << endl;
            break;
        }

        // POV Display
        myWorld.displayPOV(myCar->getPosition(), 5);

        cout << "Press ENTER...";
        cin.get();
    }

    cout << "\nSimulation Finished." << endl;
    myWorld.display(); // Full map at end

    return 0;
}