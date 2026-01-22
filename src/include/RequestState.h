#ifndef REQUEST_STATE_H
#define REQUEST_STATE_H

#include <string>

enum RequestState {
    REQUESTED,
    ALLOCATED,
    OCCUPIED,
    RELEASED,
    CANCELLED
};

std::string stateToString(RequestState state);

#endif // REQUEST_STATE_H
