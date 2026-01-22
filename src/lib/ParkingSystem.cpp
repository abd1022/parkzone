#include "../include/ParkingSystem.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>

ParkingSystem::ParkingSystem() : request_counter(0) {}

ParkingSystem::~ParkingSystem() {
    for(auto& pair : zones) delete pair.second;
    for(auto& pair : vehicles) delete pair.second;
    for(auto& pair : requests) delete pair.second;
}

ParkingSlot* ParkingSystem::findSlotById(const std::string& slot_id) {
    for(auto& zone_pair : zones) {
        Zone* zone = zone_pair.second;
        for(auto area : zone->parking_areas) {
            for(auto slot : area->slots) {
                if(slot->slot_id == slot_id) {
                    return slot;
                }
            }
        }
    }
    return nullptr;
}

// ========== SETUP OPERATIONS ==========

bool ParkingSystem::addZone(const std::string& zone_id) {
    if(zones.find(zone_id) != zones.end()) {
        std::cout << "Error: Zone " << zone_id << " already exists" << std::endl;
        return false;
    }
    zones[zone_id] = new Zone(zone_id);
    std::cout << "Zone " << zone_id << " created successfully" << std::endl;
    return true;
}

bool ParkingSystem::addParkingAreaToZone(const std::string& zone_id, const std::string& area_id, int capacity) {
    if(zones.find(zone_id) == zones.end()) {
        std::cout << "Error: Zone " << zone_id << " does not exist" << std::endl;
        return false;
    }
    zones[zone_id]->addParkingArea(area_id, capacity);
    std::cout << "Parking area " << area_id << " added to " << zone_id 
         << " with " << capacity << " slots" << std::endl;
    return true;
}

bool ParkingSystem::linkAdjacentZones(const std::string& zone1_id, const std::string& zone2_id) {
    if(zones.find(zone1_id) == zones.end() || zones.find(zone2_id) == zones.end()) {
        std::cout << "Error: One or both zones do not exist" << std::endl;
        return false;
    }
    zones[zone1_id]->linkAdjacentZone(zone2_id);
    zones[zone2_id]->linkAdjacentZone(zone1_id);
    std::cout << "Zones " << zone1_id << " and " << zone2_id << " linked as adjacent" << std::endl;
    return true;
}

bool ParkingSystem::registerVehicle(const std::string& vehicle_id, const std::string& preferred_zone) {
    if(vehicles.find(vehicle_id) != vehicles.end()) {
        std::cout << "Error: Vehicle " << vehicle_id << " already registered" << std::endl;
        return false;
    }
    vehicles[vehicle_id] = new Vehicle(vehicle_id, preferred_zone);
    std::cout << "Vehicle " << vehicle_id << " registered";
    if(!preferred_zone.empty()) {
        std::cout << " with preferred zone " << preferred_zone;
    }
    std::cout << std::endl;
    return true;
}

// ========== PARKING OPERATIONS ==========

std::string ParkingSystem::createParkingRequest(const std::string& vehicle_id, const std::string& requested_zone) {
    if(vehicles.find(vehicle_id) == vehicles.end()) {
        std::cout << "Error: Vehicle " << vehicle_id << " not registered" << std::endl;
        return "";
    }
    if(zones.find(requested_zone) == zones.end()) {
        std::cout << "Error: Zone " << requested_zone << " does not exist" << std::endl;
        return "";
    }
    
    request_counter++;
    char buffer[20];
    sprintf(buffer, "R%d", request_counter);
    std::string request_id = buffer;
    
    requests[request_id] = new ParkingRequest(request_id, vehicle_id, requested_zone);
    std::cout << "Parking request " << request_id << " created for vehicle " 
         << vehicle_id << " in zone " << requested_zone << std::endl;
    return request_id;
}

bool ParkingSystem::allocateParking(const std::string& request_id) {
    if(requests.find(request_id) == requests.end()) {
        std::cout << "Error: Request " << request_id << " not found" << std::endl;
        return false;
    }
    
    ParkingRequest* request = requests[request_id];
    
    if(request->current_state != REQUESTED) {
        std::cout << "Error: Request must be in REQUESTED state. Current state: " 
             << stateToString(request->current_state) << std::endl;
        return false;
    }
    
    AllocationEngine::AllocationResult result = allocation_engine.allocateSlot(request, zones);
    
    if(!result.success) {
        std::cout << "Allocation failed: " << result.message << std::endl;
        return false;
    }
    
    // Update slot
    result.slot->allocate(request->vehicle_id);
    
    // Update request
    request->current_state = ALLOCATED;
    request->allocated_at = std::time(nullptr);
    request->allocated_slot_id = result.slot_id;
    request->allocated_zone_id = result.zone_id;
    request->penalty = result.penalty;
    
    // Record in rollback manager
    rollback_manager.recordAllocation(result.slot, request);
    
    std::cout << "Parking allocated successfully!" << std::endl;
    std::cout << "  Slot: " << result.slot_id << std::endl;
    std::cout << "  Zone: " << result.zone_id << std::endl;
    std::cout << "  Penalty: " << result.penalty << std::endl;
    std::cout << "  Message: " << result.message << std::endl;
    
    return true;
}

bool ParkingSystem::markAsOccupied(const std::string& request_id) {
    if(requests.find(request_id) == requests.end()) {
        std::cout << "Error: Request " << request_id << " not found" << std::endl;
        return false;
    }
    
    ParkingRequest* request = requests[request_id];
    
    if(request->current_state != ALLOCATED) {
        std::cout << "Error: Request must be in ALLOCATED state. Current state: " 
             << stateToString(request->current_state) << std::endl;
        return false;
    }
    
    request->current_state = OCCUPIED;
    request->occupied_at = std::time(nullptr);
    
    // Record in rollback manager
    rollback_manager.recordOccupy(request);
    
    std::cout << "Parking marked as occupied for request " << request_id << std::endl;
    return true;
}

bool ParkingSystem::releaseParking(const std::string& request_id) {
    if(requests.find(request_id) == requests.end()) {
        std::cout << "Error: Request " << request_id << " not found" << std::endl;
        return false;
    }
    
    ParkingRequest* request = requests[request_id];
    
    if(request->current_state != OCCUPIED) {
        std::cout << "Error: Request must be in OCCUPIED state. Current state: " 
             << stateToString(request->current_state) << std::endl;
        return false;
    }
    
    ParkingSlot* slot = findSlotById(request->allocated_slot_id);
    if(slot != nullptr) {
        // Record in rollback manager before releasing
        rollback_manager.recordRelease(request, slot);
        slot->release();
    }
    
    request->current_state = RELEASED;
    request->released_at = std::time(nullptr);
    std::cout << "Parking released for request " << request_id << std::endl;
    
    if(request->occupied_at > 0 && request->released_at > 0) {
        double duration = std::difftime(request->released_at, request->occupied_at);
        std::cout << "  Duration: " << duration << " seconds" << std::endl;
    }
    
    return true;
}

bool ParkingSystem::cancelRequest(const std::string& request_id) {
    if(requests.find(request_id) == requests.end()) {
        std::cout << "Error: Request " << request_id << " not found" << std::endl;
        return false;
    }
    
    ParkingRequest* request = requests[request_id];
    
    if(request->current_state == RELEASED || request->current_state == CANCELLED) {
        std::cout << "Error: Cannot cancel request in state " 
             << stateToString(request->current_state) << std::endl;
        return false;
    }
    
    ParkingSlot* slot = nullptr;
    if(!request->allocated_slot_id.empty()) {
        slot = findSlotById(request->allocated_slot_id);
        if(slot != nullptr) {
            slot->release();
        }
    }
    
    rollback_manager.recordCancellation(request, slot);
    
    request->current_state = CANCELLED;
    std::cout << "Request " << request_id << " cancelled successfully" << std::endl;
    return true;
}

// ========== QUERY OPERATIONS ==========

void ParkingSystem::viewAllZones() {
    if(zones.empty()) {
        std::cout << "No zones registered" << std::endl;
        return;
    }
    
    std::cout << "\n========== ALL ZONES ==========" << std::endl;
    for(const auto& pair : zones) {
        Zone* zone = pair.second;
        std::cout << "Zone: " << zone->zone_id << std::endl;
        std::cout << "  Total Capacity: " << zone->getTotalCapacity() << std::endl;
        std::cout << "  Available: " << zone->getAvailableCount() << std::endl;
        std::cout << "  Occupied: " << (zone->getTotalCapacity() - zone->getAvailableCount()) << std::endl;
        std::cout << "  Parking Areas: " << zone->parking_areas.size() << std::endl;
        std::cout << "  Adjacent Zones: ";
        if(zone->adjacent_zones.empty()) {
            std::cout << "None";
        } else {
            for(size_t i = 0; i < zone->adjacent_zones.size(); i++) {
                std::cout << zone->adjacent_zones[i];
                if(i < zone->adjacent_zones.size() - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl << std::endl;
    }
}

void ParkingSystem::viewZoneDetails(const std::string& zone_id) {
    if(zones.find(zone_id) == zones.end()) {
        std::cout << "Error: Zone " << zone_id << " not found" << std::endl;
        return;
    }
    
    Zone* zone = zones[zone_id];
    std::cout << "\n========== ZONE DETAILS: " << zone_id << " ==========" << std::endl;
    std::cout << "Total Capacity: " << zone->getTotalCapacity() << std::endl;
    std::cout << "Available Slots: " << zone->getAvailableCount() << std::endl;
    std::cout << "Occupied Slots: " << (zone->getTotalCapacity() - zone->getAvailableCount()) << std::endl;
    
    std::cout << "\nParking Areas:" << std::endl;
    for(auto area : zone->parking_areas) {
        std::cout << "  Area " << area->area_id << ": " 
             << area->getAvailableCount() << "/" << area->getTotalCapacity() 
             << " available" << std::endl;
    }
    
    std::cout << "\nAdjacent Zones: ";
    if(zone->adjacent_zones.empty()) {
        std::cout << "None";
    } else {
        for(size_t i = 0; i < zone->adjacent_zones.size(); i++) {
            std::cout << zone->adjacent_zones[i];
            if(i < zone->adjacent_zones.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void ParkingSystem::viewAllRequests() {
    if(requests.empty()) {
        std::cout << "No parking requests" << std::endl;
        return;
    }
    
    std::cout << "\n========== ALL PARKING REQUESTS ==========" << std::endl;
    for(const auto& pair : requests) {
        ParkingRequest* req = pair.second;
        std::cout << "Request ID: " << req->request_id << std::endl;
        std::cout << "  Vehicle: " << req->vehicle_id << std::endl;
        std::cout << "  Requested Zone: " << req->requested_zone << std::endl;
        std::cout << "  State: " << stateToString(req->current_state) << std::endl;
        if(!req->allocated_slot_id.empty()) {
            std::cout << "  Allocated Slot: " << req->allocated_slot_id << std::endl;
            std::cout << "  Allocated Zone: " << req->allocated_zone_id << std::endl;
            std::cout << "  Penalty: " << req->penalty << std::endl;
        }
        std::cout << std::endl;
    }
}

void ParkingSystem::viewRequestDetails(const std::string& request_id) {
    if(requests.find(request_id) == requests.end()) {
        std::cout << "Error: Request " << request_id << " not found" << std::endl;
        return;
    }
    
    ParkingRequest* req = requests[request_id];
    std::cout << "\n========== REQUEST DETAILS: " << request_id << " ==========" << std::endl;
    std::cout << "Vehicle ID: " << req->vehicle_id << std::endl;
    std::cout << "Requested Zone: " << req->requested_zone << std::endl;
    std::cout << "Current State: " << stateToString(req->current_state) << std::endl;
    
    if(!req->allocated_slot_id.empty()) {
        std::cout << "Allocated Slot: " << req->allocated_slot_id << std::endl;
        std::cout << "Allocated Zone: " << req->allocated_zone_id << std::endl;
        std::cout << "Penalty: " << req->penalty << std::endl;
    }
    
    char buffer[26];
    struct tm* timeinfo;
    
    timeinfo = std::localtime(&req->created_at);
    std::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
    std::cout << "Created At: " << buffer << std::endl;
    
    if(req->allocated_at > 0) {
        timeinfo = std::localtime(&req->allocated_at);
        std::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
        std::cout << "Allocated At: " << buffer << std::endl;
    }
    
    if(req->occupied_at > 0) {
        timeinfo = std::localtime(&req->occupied_at);
        std::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
        std::cout << "Occupied At: " << buffer << std::endl;
    }
    
    if(req->released_at > 0) {
        timeinfo = std::localtime(&req->released_at);
        std::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
        std::cout << "Released At: " << buffer << std::endl;
        
        if(req->occupied_at > 0) {
            double duration = std::difftime(req->released_at, req->occupied_at);
            std::cout << "Parking Duration: " << duration << " seconds" << std::endl;
        }
    }
}

void ParkingSystem::viewAllVehicles() {
    if(vehicles.empty()) {
        std::cout << "No vehicles registered" << std::endl;
        return;
    }
    
    std::cout << "\n========== ALL VEHICLES ==========" << std::endl;
    for(const auto& pair : vehicles) {
        Vehicle* v = pair.second;
        std::cout << "Vehicle ID: " << v->vehicle_id;
        if(!v->preferred_zone.empty()) {
            std::cout << " (Preferred Zone: " << v->preferred_zone << ")";
        }
        std::cout << std::endl;
    }
}

void ParkingSystem::displaySystemSummary() {
    std::cout << "\n========== SYSTEM SUMMARY ==========" << std::endl;
    std::cout << "Total Zones: " << zones.size() << std::endl;
    std::cout << "Total Vehicles: " << vehicles.size() << std::endl;
    std::cout << "Total Requests: " << requests.size() << std::endl;
    std::cout << "Operations in Rollback Stack: " << rollback_manager.getStackSize() << std::endl;
    
    int total_capacity = 0;
    int total_available = 0;
    for(const auto& pair : zones) {
        total_capacity += pair.second->getTotalCapacity();
        total_available += pair.second->getAvailableCount();
    }
    
    std::cout << "\nParking Statistics:" << std::endl;
    std::cout << "  Total Slots: " << total_capacity << std::endl;
    std::cout << "  Available Slots: " << total_available << std::endl;
    std::cout << "  Occupied Slots: " << (total_capacity - total_available) << std::endl;
    if(total_capacity > 0) {
        double occupancy = ((total_capacity - total_available) * 100.0) / total_capacity;
        std::cout << "  Occupancy Rate: " << std::fixed << std::setprecision(1) << occupancy << "%" << std::endl;
    }
}

// ========== ANALYTICS ==========

void ParkingSystem::displayAnalytics() {
    AnalyticsEngine::AnalyticsData data = analytics_engine.calculateAnalytics(requests, zones);
    
    std::cout << "\n========== ANALYTICS DASHBOARD ==========" << std::endl;
    std::cout << "Total Requests: " << data.total_requests << std::endl;
    std::cout << "Completed Requests: " << data.completed_requests << std::endl;
    std::cout << "Cancelled Requests: " << data.cancelled_requests << std::endl;
    std::cout << "Active Requests: " << data.active_requests << std::endl;
    std::cout << "Average Parking Duration: " << std::fixed << std::setprecision(2) 
         << data.avg_parking_duration << " seconds" << std::endl;
    
    if(!data.peak_zone.empty()) {
        std::cout << "Peak Usage Zone: " << data.peak_zone 
             << " (" << data.zone_utilization[data.peak_zone] << " occupied)" << std::endl;
    }
    
    std::cout << "\nZone Utilization:" << std::endl;
    for(const auto& pair : data.zone_utilization) {
        std::string zone_id = pair.first;
        int occupied = pair.second;
        int capacity = data.zone_capacity[zone_id];
        double percentage = (capacity > 0) ? (occupied * 100.0 / capacity) : 0.0;
        std::cout << "  " << zone_id << ": " << occupied << "/" << capacity 
             << " (" << std::fixed << std::setprecision(1) << percentage << "%)" << std::endl;
    }
}

// ========== ADVANCED OPERATIONS ==========

bool ParkingSystem::rollback(int k) {
    int stack_size = rollback_manager.getStackSize();
    
    if(k <= 0) {
        std::cout << "Error: k must be positive" << std::endl;
        return false;
    }
    
    if(k > stack_size) {
        std::cout << "Error: Cannot rollback " << k << " operations. Only " 
             << stack_size << " operations in history" << std::endl;
        return false;
    }
    
    std::cout << "Rolling back " << k << " operation(s)..." << std::endl;
    
    std::vector<Operation*>& stack = rollback_manager.getOperationStack();
    
    for(int i = 0; i < k; i++) {
        Operation* op = stack.back();
        stack.pop_back();
        
        if(op->operation_type == "ALLOCATION") {
            ParkingSlot* slot = findSlotById(op->slot_id);
            if(slot != nullptr) {
                slot->is_available = op->previous_slot_availability;
                slot->occupied_vehicle_id = op->previous_slot_vehicle_id;
            }
            
            if(requests.find(op->request_id) != requests.end()) {
                ParkingRequest* req = requests[op->request_id];
                req->current_state = op->previous_request_state;
                req->allocated_slot_id = op->previous_allocated_slot_id;
                req->allocated_zone_id = op->previous_allocated_zone_id;
                req->allocated_at = 0;
                req->penalty = 0;
            }
            
            std::cout << "  Reversed: ALLOCATION for request " << op->request_id << std::endl;
        }
        else if(op->operation_type == "OCCUPY") {
            if(requests.find(op->request_id) != requests.end()) {
                ParkingRequest* req = requests[op->request_id];
                req->current_state = op->previous_request_state;
                req->occupied_at = 0;
            }
            std::cout << "  Reversed: OCCUPY for request " << op->request_id << std::endl;
        }
        else if(op->operation_type == "RELEASE") {
            if(requests.find(op->request_id) != requests.end()) {
                ParkingRequest* req = requests[op->request_id];
                req->current_state = op->previous_request_state;
                req->released_at = 0;
                
                ParkingSlot* slot = findSlotById(op->slot_id);
                if(slot != nullptr) {
                    slot->is_available = op->previous_slot_availability;
                    slot->occupied_vehicle_id = op->previous_slot_vehicle_id;
                }
            }
            std::cout << "  Reversed: RELEASE for request " << op->request_id << std::endl;
        }
        else if(op->operation_type == "CANCELLATION") {
            if(requests.find(op->request_id) != requests.end()) {
                ParkingRequest* req = requests[op->request_id];
                req->current_state = op->previous_request_state;
                req->allocated_slot_id = op->previous_allocated_slot_id;
                req->allocated_zone_id = op->previous_allocated_zone_id;
                
                if(!op->slot_id.empty()) {
                    ParkingSlot* slot = findSlotById(op->slot_id);
                    if(slot != nullptr) {
                        slot->is_available = op->previous_slot_availability;
                        slot->occupied_vehicle_id = op->previous_slot_vehicle_id;
                    }
                }
            }
            
            std::cout << "  Reversed: CANCELLATION for request " << op->request_id << std::endl;
        }
        
        delete op;
    }
    
    std::cout << "Rollback completed successfully!" << std::endl;
    return true;
}

void ParkingSystem::viewRollbackHistory() {
    std::vector<Operation*>& stack = rollback_manager.getOperationStack();
    
    if(stack.empty()) {
        std::cout << "No operations in rollback history" << std::endl;
        return;
    }
    
    std::cout << "\n========== ROLLBACK HISTORY (Stack) ==========" << std::endl;
    std::cout << "Total operations: " << stack.size() << std::endl;
    std::cout << "\nOperations (most recent first):" << std::endl;
    
    for(int i = stack.size() - 1; i >= 0; i--) {
        Operation* op = stack[i];
        std::cout << (stack.size() - i) << ". " << op->operation_type 
             << " - Request: " << op->request_id;
        if(!op->slot_id.empty()) {
            std::cout << ", Slot: " << op->slot_id;
        }
        
        char buffer[26];
        struct tm* timeinfo = std::localtime(&op->timestamp);
        std::strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
        std::cout << " [" << buffer << "]" << std::endl;
    }
}

// ========== GETTER METHODS FOR UI ==========

int ParkingSystem::getTotalSlots() const {
    int total = 0;
    for(const auto& zone_pair : zones) {
        total += zone_pair.second->getTotalCapacity();
    }
    return total;
}

int ParkingSystem::getOccupiedSlots() const {
    int occupied = 0;
    for(const auto& zone_pair : zones) {
        Zone* zone = zone_pair.second;
        for(auto area : zone->parking_areas) {
            for(auto slot : area->slots) {
                if(!slot->is_available) {
                    occupied++;
                }
            }
        }
    }
    return occupied;
}

int ParkingSystem::getAvailableSlots() const {
    return getTotalSlots() - getOccupiedSlots();
}

int ParkingSystem::getActiveRequests() const {
    int active = 0;
    for(const auto& req_pair : requests) {
        RequestState state = req_pair.second->current_state;
        if(state == REQUESTED || state == ALLOCATED || state == OCCUPIED) {
            active++;
        }
    }
    return active;
}

int ParkingSystem::getCompletedRequests() const {
    int completed = 0;
    for(const auto& req_pair : requests) {
        if(req_pair.second->current_state == RELEASED) {
            completed++;
        }
    }
    return completed;
}

int ParkingSystem::getCancelledRequests() const {
    int cancelled = 0;
    for(const auto& req_pair : requests) {
        if(req_pair.second->current_state == CANCELLED) {
            cancelled++;
        }
    }
    return cancelled;
}
