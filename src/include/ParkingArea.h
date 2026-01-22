#ifndef PARKING_AREA_H
#define PARKING_AREA_H

#include <string>
#include <vector>
#include "ParkingSlot.h"

class ParkingArea {
public:
    std::string area_id;
    std::vector<ParkingSlot*> slots;
    
    ParkingArea(const std::string& aid);
    ~ParkingArea();
    
    void addSlots(const std::string& zone_id, int capacity);
    ParkingSlot* findAvailableSlot();
    int getAvailableCount() const;
    int getTotalCapacity() const;
};

#endif // PARKING_AREA_H
