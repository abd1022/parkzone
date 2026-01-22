#include "../include/ParkingRequest.h"

ParkingRequest::ParkingRequest(const std::string& rid, const std::string& vid, const std::string& zone) 
    : request_id(rid), vehicle_id(vid), requested_zone(zone), 
      current_state(REQUESTED), created_at(std::time(nullptr)), 
      allocated_at(0), occupied_at(0), released_at(0), 
      allocated_slot_id(""), allocated_zone_id(""), penalty(0) {}
