#include "../include/Operation.h"

Operation::Operation(const std::string& type, const std::string& rid, const std::string& sid) 
    : operation_type(type), request_id(rid), slot_id(sid), 
      previous_slot_availability(true), previous_slot_vehicle_id(""),
      previous_request_state(REQUESTED), previous_allocated_slot_id(""),
      previous_allocated_zone_id(""), timestamp(std::time(nullptr)) {}
