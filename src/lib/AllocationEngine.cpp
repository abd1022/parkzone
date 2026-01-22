#include "../include/AllocationEngine.h"
#include <algorithm>

AllocationEngine::AllocationResult::AllocationResult() 
    : success(false), penalty(0), slot(nullptr) {}

AllocationEngine::AllocationResult AllocationEngine::allocateSlot(ParkingRequest* request, std::map<std::string, Zone*>& zones) {
    AllocationResult result;
    
    // Priority 1: Same zone (penalty = 0)
    if(zones.find(request->requested_zone) != zones.end()) {
        Zone* zone = zones[request->requested_zone];
        ParkingSlot* slot = zone->findAvailableSlot();
        if(slot != nullptr) {
            result.success = true;
            result.message = "Allocated in requested zone";
            result.slot_id = slot->slot_id;
            result.zone_id = zone->zone_id;
            result.penalty = 0;
            result.slot = slot;
            return result;
        }
    }
    
    // Priority 2: Adjacent zones (penalty = 50)
    if(zones.find(request->requested_zone) != zones.end()) {
        Zone* requested_zone = zones[request->requested_zone];
        for(const std::string& adj_zone_id : requested_zone->adjacent_zones) {
            if(zones.find(adj_zone_id) != zones.end()) {
                Zone* adj_zone = zones[adj_zone_id];
                ParkingSlot* slot = adj_zone->findAvailableSlot();
                if(slot != nullptr) {
                    result.success = true;
                    result.message = "Allocated in adjacent zone (penalty applied)";
                    result.slot_id = slot->slot_id;
                    result.zone_id = adj_zone->zone_id;
                    result.penalty = 50;
                    result.slot = slot;
                    return result;
                }
            }
        }
    }
    
    // Priority 3: Any other zone (penalty = 100)
    for(auto& pair : zones) {
        Zone* zone = pair.second;
        if(zone->zone_id != request->requested_zone) {
            bool is_adjacent = false;
            if(zones.find(request->requested_zone) != zones.end()) {
                Zone* req_zone = zones[request->requested_zone];
                if(std::find(req_zone->adjacent_zones.begin(), req_zone->adjacent_zones.end(), 
                       zone->zone_id) != req_zone->adjacent_zones.end()) {
                    is_adjacent = true;
                }
            }
            
            if(!is_adjacent) {
                ParkingSlot* slot = zone->findAvailableSlot();
                if(slot != nullptr) {
                    result.success = true;
                    result.message = "Allocated in distant zone (higher penalty)";
                    result.slot_id = slot->slot_id;
                    result.zone_id = zone->zone_id;
                    result.penalty = 100;
                    result.slot = slot;
                    return result;
                }
            }
        }
    }
    
    result.success = false;
    result.message = "No available slots in any zone";
    return result;
}
