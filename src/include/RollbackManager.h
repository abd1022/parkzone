#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include <vector>
#include "Operation.h"
#include "ParkingSlot.h"
#include "ParkingRequest.h"

class RollbackManager {
private:
    std::vector<Operation*> operation_stack;
    
public:
    ~RollbackManager();
    
    void recordAllocation(ParkingSlot* slot, ParkingRequest* request);
    void recordCancellation(ParkingRequest* request, ParkingSlot* slot = nullptr);
    void recordOccupy(ParkingRequest* request);
    void recordRelease(ParkingRequest* request, ParkingSlot* slot);
    int getStackSize() const;
    std::vector<Operation*>& getOperationStack();
    void clearStack();
};

#endif // ROLLBACK_MANAGER_H
