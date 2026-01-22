#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle {
public:
    std::string vehicle_id;
    std::string preferred_zone;
    
    Vehicle(const std::string& vid, const std::string& pzone = "");
};

#endif // VEHICLE_H
