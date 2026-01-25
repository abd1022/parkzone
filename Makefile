# Makefile for Smart Parking System
# For Windows with MinGW or Linux/macOS

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src -I./src/include
LDFLAGS = 

# FLTK settings (adjust paths as needed)
ifeq ($(OS),Windows_NT)
    # Windows with MinGW (using vcpkg)
    FLTK_INCLUDE = -IC:/vcpkg/packages/fltk_x64-windows/include
    FLTK_LIBS = -LC:/vcpkg/packages/fltk_x64-windows/lib -lfltk -lole32 -luuid -lcomctl32 -lgdi32 -lws2_32
    FLTK_FLAGS = -mwindows
    RM = del /Q
    RM_DIR = rmdir /S /Q
    MKDIR = mkdir
    CLI_EXE = parking_cli.exe
    GUI_EXE = parking_gui.exe
else
    # Linux/macOS
    FLTK_INCLUDE = $(shell fltk-config --cxxflags)
    FLTK_LIBS = $(shell fltk-config --ldflags)
    FLTK_FLAGS = 
    RM = rm -f
    RM_DIR = rm -rf
    MKDIR = mkdir -p
    CLI_EXE = parking_cli
    GUI_EXE = parking_gui
endif

# Source files
CORE_SOURCES = $(wildcard src/lib/*.cpp)
UI_SOURCES = $(wildcard src/ui/*.cpp)
CORE_OBJECTS = $(CORE_SOURCES:.cpp=.o)
UI_OBJECTS = $(UI_SOURCES:.cpp=.o)

# Targets
.PHONY: all cli gui clean help

all: cli gui

cli: $(CLI_EXE)

gui: $(GUI_EXE)

$(CLI_EXE): src/main.cpp $(CORE_OBJECTS)
	@echo "Building CLI application..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ CLI built: $(CLI_EXE)"

$(GUI_EXE): src/main_gui.cpp $(CORE_OBJECTS) $(UI_OBJECTS)
	@echo "Building GUI application..."
	$(CXX) $(CXXFLAGS) $(FLTK_INCLUDE) -o $@ $^ $(FLTK_LIBS) $(FLTK_FLAGS) $(LDFLAGS)
	@echo "✓ GUI built: $(GUI_EXE)"

# Compile core library
src/lib/%.o: src/lib/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile UI components
src/ui/%.o: src/ui/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(FLTK_INCLUDE) -c $< -o $@

clean:
	@echo "Cleaning build files..."
ifeq ($(OS),Windows_NT)
	-$(RM) $(CLI_EXE) $(GUI_EXE) 2>nul
	-$(RM) src\lib\*.o 2>nul
	-$(RM) src\ui\*.o 2>nul
else
	$(RM) $(CLI_EXE) $(GUI_EXE)
	$(RM) $(CORE_OBJECTS) $(UI_OBJECTS)
endif
	@echo "✓ Clean complete"

help:
	@echo "Smart Parking System - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make         - Build both CLI and GUI"
	@echo "  make cli     - Build CLI only"
	@echo "  make gui     - Build GUI only (requires FLTK)"
	@echo "  make clean   - Remove built files"
	@echo "  make help    - Show this help"
	@echo ""
	@echo "Windows Note:"
	@echo "  Set FLTK path in Makefile if not at C:\fltk"
	@echo ""
	@echo "Linux/macOS:"
	@echo "  Install FLTK: sudo apt-get install libfltk1.3-dev"

# Dependencies (simplified - for production use makedepend or similar)
$(CORE_OBJECTS): $(wildcard src/include/*.h)
$(UI_OBJECTS): $(wildcard src/include/*.h) $(wildcard src/ui/*.h)
