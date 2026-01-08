#include "NavigationSystem.h"
#include <iostream>
#include <cmath>

using namespace std;

// Constructor
NavigationSystem::NavigationSystem(int w, int h, const vector<Position>& targets, double minConf) 
    : gpsTargets(targets), currentTargetIndex(0), fusionEngine(minConf), worldWidth(w), worldHeight(h),
      lastStoppedSignId(""), stopTicksRemaining(0)
{
}

void NavigationSystem::syncNavigationSystem(const vector<SensorReading>& rawReadings) {
    currentWorldState = fusionEngine.fuseSensorData(rawReadings);
}

bool NavigationSystem::makeDecision(Position currentPos, int& desiredSpeed, int& desiredDirection) {
    // Check if we're done
    if (gpsTargets.empty() || currentTargetIndex >= gpsTargets.size()) {
        desiredSpeed = 0;
        return true; 
    }

    Position target = gpsTargets[currentTargetIndex];

    // If we are serving a mandatory stop (e.g., STOP sign), hold position
    if (stopTicksRemaining > 0) {
        desiredSpeed = 0;
        stopTicksRemaining--;
        return false;
    }

    // Check if reached current target
    if (currentPos.x == target.x && currentPos.y == target.y) {
        cout << "[NAV] Reached GPS target: (" << target.x << ", " << target.y << ")" << endl;
        currentTargetIndex++;
        
        if (currentTargetIndex >= gpsTargets.size()) {
            desiredSpeed = 0;
            return true;
        }
        target = gpsTargets[currentTargetIndex];
    }

    int dx = target.x - currentPos.x;
    int dy = target.y - currentPos.y;

    int primaryDir = -1;
    int secondaryDir = -1;

    if (abs(dy) >= abs(dx)) {
        primaryDir = (dy > 0) ? 0 : 2;   
        secondaryDir = (dx > 0) ? 1 : 3; 
        if (dx == 0) secondaryDir = -1; 
    } 
    else {
        primaryDir = (dx > 0) ? 1 : 3;
        secondaryDir = (dy > 0) ? 0 : 2;
        
        if (dy == 0) {
            secondaryDir = -1;
        }    
    }

    desiredDirection = primaryDir;
    desiredSpeed = 2; 

    // Target approach
    if (desiredDirection == 1 && dx > 0 && dx < desiredSpeed) {
        desiredSpeed = dx;       
    }    
    else if(desiredDirection == 3 && dx < 0 && abs(dx) < desiredSpeed) {
        desiredSpeed = abs(dx); 
    }    
    else if (desiredDirection == 0 && dy > 0 && dy < desiredSpeed) {
        desiredSpeed = dy;  
    }    
    else if (desiredDirection == 2 && dy < 0 && abs(dy) < desiredSpeed) {
        desiredSpeed = abs(dy); 
    }    
    
    if ((abs(dx) + abs(dy)) <= 5) {
        desiredSpeed = 1;
    }    

    // Traffic awareness
    bool blockedClose = false;
    bool slowDownAhead = false;
    for (const auto& obj : currentWorldState) {
        int objDx = obj.position.x - currentPos.x;
        int objDy = obj.position.y - currentPos.y;

        // Determine if object is directly ahead in our lane and compute forward distance
        bool ahead = false;
        int distAhead = 0;

        if (desiredDirection == 0) { // Up
            ahead = (objDx == 0 && objDy > 0);
            distAhead = objDy;
        } 
        else if (desiredDirection == 2) { // Down
            ahead = (objDx == 0 && objDy < 0);
            distAhead = std::abs(objDy);
        } 
        else if (desiredDirection == 1) { // Right
            ahead = (objDy == 0 && objDx > 0);
            distAhead = objDx;
        } 
        else if (desiredDirection == 3) { // Left
            ahead = (objDy == 0 && objDx < 0);
            distAhead = std::abs(objDx);
        }

        if (!ahead) {
            continue;
        }    

        // Traffic lights
        if (obj.type == "TrafficLight") {
            if (distAhead <= 3) {
                if (obj.trafficLight == 'R') {
                    desiredSpeed = 0;
                } 
                else if (obj.trafficLight == 'Y') {
                    if (desiredSpeed > 1) {
                        desiredSpeed = 1;
                    }    
                }
            }
            continue;
        }

        // Stop signs
        if (obj.objectId.find("STOP:") == 0) {
            if (lastStoppedSignId != obj.objectId) {
                if (distAhead <= 1) {
                    desiredSpeed = 0;
                    lastStoppedSignId = obj.objectId;
                    stopTicksRemaining = 1; // hold for one tick
                } 
                else if (distAhead <= 2) {
                    if (desiredSpeed > 1) {
                        desiredSpeed = 1;
                    }    
                }
            }
            continue;
        }

        // Other obstacles/vehicles
        bool isMovingObj = (obj.type == "Vehicle" || obj.objectId.find("CAR:") == 0 || obj.objectId.find("BIKE:") == 0);
        if (isMovingObj) {
            // slow down if within 2, consider reroute if blocking next cell
            if (distAhead <= 1) {
                blockedClose = true;
            } 
            else if (distAhead <= 2) {
                slowDownAhead = true;
            }
        }
    }

    if (slowDownAhead && desiredSpeed > 1) {
        desiredSpeed = 1;
    }

    // Rerout logic
    if (blockedClose) {
        cout << "[NAV] Blocked in direction " << desiredDirection << ". Attempting reroute..." << endl;
        
        auto isSafe = [&](int dir) {
            int tx = currentPos.x;
            int ty = currentPos.y;

            if (dir == 0) {
                ty++;
            }      
            else if (dir == 1) {
            tx++;
            }
            else if (dir == 2) {
                ty--;
            }    
            else if (dir == 3) {
                tx--; 
            }

            return (tx >= 0 && tx < worldWidth && ty >= 0 && ty < worldHeight);
        };

        bool foundPath = false;
        if (secondaryDir != -1 && isSafe(secondaryDir)) {
            desiredDirection = secondaryDir;
            foundPath = true;
        }
        
        if (!foundPath) {
            if (desiredDirection == 1 || desiredDirection == 3) {
                if (isSafe(0)) {
                    desiredDirection = 0;
                }    
                else if (isSafe(2)) {
                    desiredDirection = 2;
                }    
                else {
                    desiredSpeed = 0; 
                }    
            } 
            else {
                if (isSafe(1)) {
                    desiredDirection = 1;
                }    
                else if (isSafe(3)) {
                    desiredDirection = 3;
                }    
                else {
                    desiredSpeed = 0; 
                }
            }
        }
        desiredSpeed = (desiredSpeed == 0) ? 0 : 1; 
    }

    if (desiredDirection == 0) { // Up
        int distToEdge = worldHeight - 1 - currentPos.y;
        if (desiredSpeed > distToEdge) {
            desiredSpeed = distToEdge;
        }    
    } 
    else if (desiredDirection == 1) { // Right
        int distToEdge = worldWidth - 1 - currentPos.x;
        if (desiredSpeed > distToEdge) {
            desiredSpeed = distToEdge;
        }    
    }
    else if (desiredDirection == 2) { // Down
        int distToEdge = currentPos.y; 
        if (desiredSpeed > distToEdge) {
            desiredSpeed = distToEdge;
        }    
    }
    else if (desiredDirection == 3) { // Left
        int distToEdge = currentPos.x; 
        if (desiredSpeed > distToEdge) {
            desiredSpeed = distToEdge;
        }    
    }

    return false;
}