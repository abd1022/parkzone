#ifndef ALLOCATION_ENGINE_H
#define ALLOCATION_ENGINE_H

#include <map>
#include <string>
#include "Zone.h"
#include "ParkingRequest.h"

class AllocationEngine {
public:
    struct AllocationResult {
        bool success;
        std::string message;
        std::string slot_id;
        std::string zone_id;
        int penalty;
        ParkingSlot* slot;
        
        AllocationResult();
    };
    
    AllocationResult allocateSlot(ParkingRequest* request, std::map<std::string, Zone*>& zones);
};

#endif // ALLOCATION_ENGINE_H
