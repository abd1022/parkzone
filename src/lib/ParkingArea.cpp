#include "../include/ParkingArea.h"

ParkingArea::ParkingArea(const std::string& aid) : area_id(aid) {}

ParkingArea::~ParkingArea() {
    for(auto slot : slots) {
        delete slot;
    }
}

void ParkingArea::addSlots(const std::string& zone_id, int capacity) {
    for(int i = 1; i <= capacity; i++) {
        std::string slot_id = zone_id + "-" + area_id + "-" + std::to_string(i);
        slots.push_back(new ParkingSlot(slot_id, zone_id));
    }
}

ParkingSlot* ParkingArea::findAvailableSlot() {
    for(auto slot : slots) {
        if(slot->is_available) {
            return slot;
        }
    }
    return nullptr;
}

int ParkingArea::getAvailableCount() const {
    int count = 0;
    for(auto slot : slots) {
        if(slot->is_available) count++;
    }
    return count;
}

int ParkingArea::getTotalCapacity() const {
    return slots.size();
}
