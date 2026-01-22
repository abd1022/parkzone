#ifndef ZONE_H
#define ZONE_H

#include <string>
#include <vector>
#include "ParkingArea.h"

class Zone {
public:
    std::string zone_id;
    std::vector<ParkingArea*> parking_areas;
    std::vector<std::string> adjacent_zones;
    
    Zone(const std::string& zid);
    ~Zone();
    
    void addParkingArea(const std::string& area_id, int capacity);
    void linkAdjacentZone(const std::string& adj_zone_id);
    ParkingSlot* findAvailableSlot();
    int getAvailableCount() const;
    int getTotalCapacity() const;
};

#endif // ZONE_H
