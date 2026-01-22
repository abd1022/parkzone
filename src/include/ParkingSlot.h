#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>

class ParkingSlot {
public:
    std::string slot_id;
    std::string zone_id;
    bool is_available;
    std::string occupied_vehicle_id;
    
    ParkingSlot(const std::string& sid, const std::string& zid);
    void allocate(const std::string& vehicle_id);
    void release();
};

#endif // PARKING_SLOT_H
