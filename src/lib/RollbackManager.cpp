#include "../include/RollbackManager.h"

RollbackManager::~RollbackManager() {
    for(auto op : operation_stack) {
        delete op;
    }
}

void RollbackManager::recordAllocation(ParkingSlot* slot, ParkingRequest* request) {
    Operation* op = new Operation("ALLOCATION", request->request_id, slot->slot_id);
    op->previous_slot_availability = true;
    op->previous_slot_vehicle_id = "";
    op->previous_request_state = REQUESTED;
    op->previous_allocated_slot_id = "";
    op->previous_allocated_zone_id = "";
    operation_stack.push_back(op);
}

void RollbackManager::recordCancellation(ParkingRequest* request, ParkingSlot* slot) {
    std::string slot_id = (slot != nullptr) ? slot->slot_id : "";
    Operation* op = new Operation("CANCELLATION", request->request_id, slot_id);
    op->previous_request_state = request->current_state;
    op->previous_allocated_slot_id = request->allocated_slot_id;
    op->previous_allocated_zone_id = request->allocated_zone_id;
    
    if(slot != nullptr) {
        op->previous_slot_availability = false;
        op->previous_slot_vehicle_id = request->vehicle_id;
    }
    operation_stack.push_back(op);
}

void RollbackManager::recordOccupy(ParkingRequest* request) {
    Operation* op = new Operation("OCCUPY", request->request_id, request->allocated_slot_id);
    op->previous_request_state = ALLOCATED;
    operation_stack.push_back(op);
}

void RollbackManager::recordRelease(ParkingRequest* request, ParkingSlot* slot) {
    Operation* op = new Operation("RELEASE", request->request_id, request->allocated_slot_id);
    op->previous_request_state = OCCUPIED;
    op->previous_slot_availability = false;
    op->previous_slot_vehicle_id = request->vehicle_id;
    operation_stack.push_back(op);
}

int RollbackManager::getStackSize() const {
    return operation_stack.size();
}

std::vector<Operation*>& RollbackManager::getOperationStack() {
    return operation_stack;
}

void RollbackManager::clearStack() {
    for(auto op : operation_stack) {
        delete op;
    }
    operation_stack.clear();
}
