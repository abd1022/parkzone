#ifndef ANALYTICS_ENGINE_H
#define ANALYTICS_ENGINE_H

#include <map>
#include <string>
#include "ParkingRequest.h"
#include "Zone.h"

class AnalyticsEngine {
public:
    struct AnalyticsData {
        int total_requests;
        int completed_requests;
        int cancelled_requests;
        int active_requests;
        double avg_parking_duration;
        std::map<std::string, int> zone_utilization;
        std::map<std::string, int> zone_capacity;
        std::string peak_zone;
    };
    
    AnalyticsData calculateAnalytics(const std::map<std::string, ParkingRequest*>& requests, 
                                     const std::map<std::string, Zone*>& zones);
};

#endif // ANALYTICS_ENGINE_H
