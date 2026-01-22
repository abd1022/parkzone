#include "../include/RequestState.h"

std::string stateToString(RequestState state) {
    switch(state) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}
