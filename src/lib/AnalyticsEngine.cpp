#include "../include/AnalyticsEngine.h"

AnalyticsEngine::AnalyticsData AnalyticsEngine::calculateAnalytics(
    const std::map<std::string, ParkingRequest*>& requests, 
    const std::map<std::string, Zone*>& zones) {
    
    AnalyticsData data;
    data.total_requests = requests.size();
    data.completed_requests = 0;
    data.cancelled_requests = 0;
    data.active_requests = 0;
    data.avg_parking_duration = 0.0;
    
    double total_duration = 0.0;
    int completed_with_duration = 0;
    
    // Calculate request statistics
    for(const auto& pair : requests) {
        ParkingRequest* req = pair.second;
        if(req->current_state == RELEASED) {
            data.completed_requests++;
            if(req->occupied_at > 0 && req->released_at > 0) {
                total_duration += std::difftime(req->released_at, req->occupied_at);
                completed_with_duration++;
            }
        } else if(req->current_state == CANCELLED) {
            data.cancelled_requests++;
        } else {
            data.active_requests++;
        }
    }
    
    if(completed_with_duration > 0) {
        data.avg_parking_duration = total_duration / completed_with_duration;
    }
    
    // Calculate zone utilization
    int max_occupied = 0;
    for(const auto& pair : zones) {
        Zone* zone = pair.second;
        int total = zone->getTotalCapacity();
        int available = zone->getAvailableCount();
        int occupied = total - available;
        
        data.zone_capacity[zone->zone_id] = total;
        data.zone_utilization[zone->zone_id] = occupied;
        
        if(occupied > max_occupied) {
            max_occupied = occupied;
            data.peak_zone = zone->zone_id;
        }
    }
    
    return data;
}
