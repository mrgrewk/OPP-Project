#include "SensorFusionEngine.h"
#include <iostream>
#include <cmath>

using namespace std;

SensorFusionEngine::SensorFusionEngine(double threshold) : minConfidenceThreshold(threshold) { }

std::vector<SensorReading> SensorFusionEngine::fuseSensorData(const std:: vector<SensorReading>& rawReadings) {
    map<string, vector<SensorReading>> groupedReadings;

    for (const auto& r : rawReadings) {
        groupedReadings[r.objectId].push_back(r);
    }

    std::vector<SensorReading> fusedResults;

    for (auto const& [id, group] :groupedReadings) {
        double totalWeight = 0;
        double weightedDist = 0;
        double weightedConf = 0;

        string bestType = "Unknown";
        char bestTL = ' ';
        int bestSpeed = 0;
        double maxConf = -1.0;

        bool isBike = false;

        for (const auto& r : group) {
            if (r.type == "Bike" || r.objectId.find("BIKE") != string::npos) {
                isBike = true;
            }
            
            if (r.confidence > maxConf) {
                maxConf = r.confidence;
                bestType = r.type;
                if (r.trafficLight != ' ') bestTL = r.trafficLight;
                bestSpeed = r.speed;
            }

            weightedDist += r.distance * r.confidence;
            weightedConf += r.confidence;
            totalWeight += r.confidence;
        }
        int finalDist = (totalWeight > 0) ? round(weightedDist / totalWeight) : 0;
        double finalConf = (group.size() > 0) ? (weightedConf / group.size()) : 0.0;

        SensorReading fused(bestType, finalDist, group[0].position, id, finalConf);
        fused.trafficLight = bestTL;
        fused.speed = bestSpeed;

        if (isBike || finalConf >= minConfidenceThreshold) {
            fusedResults.push_back(fused);
        }
    }
    
    return fusedResults;
}