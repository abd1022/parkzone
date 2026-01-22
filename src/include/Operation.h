#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <ctime>
#include "RequestState.h"

class Operation {
public:
    std::string operation_type;
    std::string request_id;
    std::string slot_id;
    bool previous_slot_availability;
    std::string previous_slot_vehicle_id;
    RequestState previous_request_state;
    std::string previous_allocated_slot_id;
    std::string previous_allocated_zone_id;
    time_t timestamp;
    
    Operation(const std::string& type, const std::string& rid, const std::string& sid);
};

#endif // OPERATION_H
