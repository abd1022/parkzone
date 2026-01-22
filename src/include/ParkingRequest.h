#ifndef PARKING_REQUEST_H
#define PARKING_REQUEST_H

#include <string>
#include <ctime>
#include "RequestState.h"

class ParkingRequest {
public:
    std::string request_id;
    std::string vehicle_id;
    std::string requested_zone;
    RequestState current_state;
    time_t created_at;
    time_t allocated_at;
    time_t occupied_at;
    time_t released_at;
    std::string allocated_slot_id;
    std::string allocated_zone_id;
    int penalty;
    
    ParkingRequest(const std::string& rid, const std::string& vid, const std::string& zone);
};

#endif // PARKING_REQUEST_H
