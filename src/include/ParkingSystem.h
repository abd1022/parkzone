#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include <map>
#include <string>
#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"
#include "AnalyticsEngine.h"

class ParkingSystem {
private:
    std::map<std::string, Zone*> zones;
    std::map<std::string, Vehicle*> vehicles;
    std::map<std::string, ParkingRequest*> requests;
    int request_counter;
    AllocationEngine allocation_engine;
    RollbackManager rollback_manager;
    AnalyticsEngine analytics_engine;
    
    ParkingSlot* findSlotById(const std::string& slot_id);
    
public:
    ParkingSystem();
    ~ParkingSystem();
    
    // Setup Operations
    bool addZone(const std::string& zone_id);
    bool addParkingAreaToZone(const std::string& zone_id, const std::string& area_id, int capacity);
    bool linkAdjacentZones(const std::string& zone1_id, const std::string& zone2_id);
    bool registerVehicle(const std::string& vehicle_id, const std::string& preferred_zone = "");
    
    // Parking Operations
    std::string createParkingRequest(const std::string& vehicle_id, const std::string& requested_zone);
    bool allocateParking(const std::string& request_id);
    bool markAsOccupied(const std::string& request_id);
    bool releaseParking(const std::string& request_id);
    bool cancelRequest(const std::string& request_id);
    
    // Query Operations
    void viewAllZones();
    void viewZoneDetails(const std::string& zone_id);
    void viewAllRequests();
    void viewRequestDetails(const std::string& request_id);
    void viewAllVehicles();
    void displaySystemSummary();
    
    // Analytics
    void displayAnalytics();
    
    // Advanced Operations
    bool rollback(int k);
    void viewRollbackHistory();
    
    // Getter methods for UI
    int getTotalSlots() const;
    int getOccupiedSlots() const;
    int getAvailableSlots() const;
    int getActiveRequests() const;
    int getCompletedRequests() const;
    int getCancelledRequests() const;
};

#endif // PARKING_SYSTEM_H
