#include "../include/Zone.h"
#include <algorithm>

Zone::Zone(const std::string& zid) : zone_id(zid) {}

Zone::~Zone() {
    for(auto area : parking_areas) {
        delete area;
    }
}

void Zone::addParkingArea(const std::string& area_id, int capacity) {
    ParkingArea* area = new ParkingArea(area_id);
    area->addSlots(zone_id, capacity);
    parking_areas.push_back(area);
}

void Zone::linkAdjacentZone(const std::string& adj_zone_id) {
    if(std::find(adjacent_zones.begin(), adjacent_zones.end(), adj_zone_id) == adjacent_zones.end()) {
        adjacent_zones.push_back(adj_zone_id);
    }
}

ParkingSlot* Zone::findAvailableSlot() {
    for(auto area : parking_areas) {
        ParkingSlot* slot = area->findAvailableSlot();
        if(slot != nullptr) {
            return slot;
        }
    }
    return nullptr;
}

int Zone::getAvailableCount() const {
    int count = 0;
    for(auto area : parking_areas) {
        count += area->getAvailableCount();
    }
    return count;
}

int Zone::getTotalCapacity() const {
    int count = 0;
    for(auto area : parking_areas) {
        count += area->getTotalCapacity();
    }
    return count;
}
