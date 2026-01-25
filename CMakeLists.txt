cmake_minimum_required(VERSION 3.15)
project(SmartParkingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vcpkg integration (if using vcpkg toolchain)
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Find FLTK package
find_package(FLTK REQUIRED)
if(FLTK_FOUND)
    include_directories(${FLTK_INCLUDE_DIR})
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Core library source files
set(CORE_SOURCES
    src/lib/AllocationEngine.cpp
    src/lib/AnalyticsEngine.cpp
    src/lib/Operation.cpp
    src/lib/ParkingArea.cpp
    src/lib/ParkingRequest.cpp
    src/lib/ParkingSlot.cpp
    src/lib/ParkingSystem.cpp
    src/lib/RequestState.cpp
    src/lib/RollbackManager.cpp
    src/lib/Vehicle.cpp
    src/lib/Zone.cpp
)

# UI source files
set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/SetupScreen.cpp
    src/ui/DashboardScreen.cpp
    src/ui/RequestScreen.cpp
    src/ui/StatusScreen.cpp
    src/ui/RollbackScreen.cpp
    src/ui/AnalyticsScreen.cpp
)

# CLI executable
add_executable(parking_cli src/main.cpp ${CORE_SOURCES})

# GUI executable
add_executable(parking_gui src/main_gui.cpp ${CORE_SOURCES} ${UI_SOURCES})
target_link_libraries(parking_gui ${FLTK_LIBRARIES})

# Windows-specific settings
if(WIN32)
    target_link_libraries(parking_gui ws2_32)
    # For FLTK on Windows
    target_link_libraries(parking_gui comctl32)
endif()

# Output directories
set_target_properties(parking_cli parking_gui
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Installation
install(TARGETS parking_cli parking_gui
    RUNTIME DESTINATION bin
)
