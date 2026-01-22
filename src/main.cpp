#include <iostream>
#include <string>
#include <limits>
#include "include/ParkingSystem.h"

using namespace std;

void displayMenu() {
    cout << "\n================================================================" << endl;
    cout << "||      SMART PARKING ALLOCATION & MANAGEMENT SYSTEM        ||" << endl;
    cout << "================================================================" << endl;
    cout << "\n--- SETUP OPERATIONS ---" << endl;
    cout << " 1.  Add Zone" << endl;
    cout << " 2.  Add Parking Area to Zone" << endl;
    cout << " 3.  Link Adjacent Zones" << endl;
    cout << " 4.  Register Vehicle" << endl;
    cout << "\n--- PARKING OPERATIONS ---" << endl;
    cout << " 5.  Create Parking Request" << endl;
    cout << " 6.  Allocate Parking" << endl;
    cout << " 7.  Mark Parking as Occupied" << endl;
    cout << " 8.  Release Parking" << endl;
    cout << " 9.  Cancel Parking Request" << endl;
    cout << "\n--- QUERY OPERATIONS ---" << endl;
    cout << " 10. View All Zones" << endl;
    cout << " 11. View Zone Details" << endl;
    cout << " 12. View All Parking Requests" << endl;
    cout << " 13. View Request Details" << endl;
    cout << " 14. View All Vehicles" << endl;
    cout << " 15. Display System Summary" << endl;
    cout << "\n--- ANALYTICS ---" << endl;
    cout << " 16. Display Analytics Dashboard" << endl;
    cout << "\n--- ADVANCED OPERATIONS ---" << endl;
    cout << " 17. Rollback Last k Operations" << endl;
    cout << " 18. View Rollback History" << endl;
    cout << "\n--- SYSTEM ---" << endl;
    cout << " 0.  Exit" << endl;
    cout << "================================================================" << endl;
    cout << "\nEnter your choice: ";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    ParkingSystem system;
    int choice;
    
    while(true) {
        displayMenu();
        
        if(!(cin >> choice)) {
            cout << "\nInvalid input! Please enter a number." << endl;
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        string zone_id, zone1_id, zone2_id, area_id, vehicle_id, preferred_zone;
        string requested_zone, request_id;
        int capacity, k;
        
        switch(choice) {
            case 0:
                cout << "\nThank you for using SmartPark. Goodbye!" << endl;
                return 0;
                
            case 1:  // Add Zone
                cout << "\n--- Add Zone ---" << endl;
                cout << "Enter Zone ID: ";
                getline(cin, zone_id);
                system.addZone(zone_id);
                break;
                
            case 2:  // Add Parking Area to Zone
                cout << "\n--- Add Parking Area to Zone ---" << endl;
                cout << "Enter Zone ID: ";
                getline(cin, zone_id);
                cout << "Enter Area ID: ";
                getline(cin, area_id);
                cout << "Enter Capacity (number of slots): ";
                cin >> capacity;
                clearInputBuffer();
                system.addParkingAreaToZone(zone_id, area_id, capacity);
                break;
                
            case 3:  // Link Adjacent Zones
                cout << "\n--- Link Adjacent Zones ---" << endl;
                cout << "Enter First Zone ID: ";
                getline(cin, zone1_id);
                cout << "Enter Second Zone ID: ";
                getline(cin, zone2_id);
                system.linkAdjacentZones(zone1_id, zone2_id);
                break;
                
            case 4:  // Register Vehicle
                cout << "\n--- Register Vehicle ---" << endl;
                cout << "Enter Vehicle ID: ";
                getline(cin, vehicle_id);
                cout << "Enter Preferred Zone (press Enter to skip): ";
                getline(cin, preferred_zone);
                system.registerVehicle(vehicle_id, preferred_zone);
                break;
                
            case 5:  // Create Parking Request
                cout << "\n--- Create Parking Request ---" << endl;
                cout << "Enter Vehicle ID: ";
                getline(cin, vehicle_id);
                cout << "Enter Requested Zone: ";
                getline(cin, requested_zone);
                system.createParkingRequest(vehicle_id, requested_zone);
                break;
                
            case 6:  // Allocate Parking
                cout << "\n--- Allocate Parking ---" << endl;
                cout << "Enter Request ID: ";
                getline(cin, request_id);
                system.allocateParking(request_id);
                break;
                
            case 7:  // Mark as Occupied
                cout << "\n--- Mark Parking as Occupied ---" << endl;
                cout << "Enter Request ID: ";
                getline(cin, request_id);
                system.markAsOccupied(request_id);
                break;
                
            case 8:  // Release Parking
                cout << "\n--- Release Parking ---" << endl;
                cout << "Enter Request ID: ";
                getline(cin, request_id);
                system.releaseParking(request_id);
                break;
                
            case 9:  // Cancel Request
                cout << "\n--- Cancel Parking Request ---" << endl;
                cout << "Enter Request ID: ";
                getline(cin, request_id);
                system.cancelRequest(request_id);
                break;
                
            case 10:  // View All Zones
                system.viewAllZones();
                break;
                
            case 11:  // View Zone Details
                cout << "\n--- View Zone Details ---" << endl;
                cout << "Enter Zone ID: ";
                getline(cin, zone_id);
                system.viewZoneDetails(zone_id);
                break;
                
            case 12:  // View All Requests
                system.viewAllRequests();
                break;
                
            case 13:  // View Request Details
                cout << "\n--- View Request Details ---" << endl;
                cout << "Enter Request ID: ";
                getline(cin, request_id);
                system.viewRequestDetails(request_id);
                break;
                
            case 14:  // View All Vehicles
                system.viewAllVehicles();
                break;
                
            case 15:  // System Summary
                system.displaySystemSummary();
                break;
                
            case 16:  // Analytics Dashboard
                system.displayAnalytics();
                break;
                
            case 17:  // Rollback
                cout << "\n--- Rollback Last k Operations ---" << endl;
                cout << "Enter number of operations to rollback: ";
                cin >> k;
                clearInputBuffer();
                system.rollback(k);
                break;
                
            case 18:  // View Rollback History
                system.viewRollbackHistory();
                break;
                
            default:
                cout << "\nInvalid choice! Please select a valid option." << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    return 0;
}
