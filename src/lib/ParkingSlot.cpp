#include "../include/ParkingSlot.h"

ParkingSlot::ParkingSlot(const std::string& sid, const std::string& zid) 
    : slot_id(sid), zone_id(zid), is_available(true), occupied_vehicle_id("") {}

void ParkingSlot::allocate(const std::string& vehicle_id) {
    is_available = false;
    occupied_vehicle_id = vehicle_id;
}

void ParkingSlot::release() {
    is_available = true;
    occupied_vehicle_id = "";
}
