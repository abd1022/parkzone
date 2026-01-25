#include <FL/Fl.H>
#include "ui/MainWindow.h"

int main(int argc, char** argv) {
    // Create main window
    MainWindow* window = new MainWindow(1200, 700, "Smart Parking Management System");
    
    // Show window
    window->show(argc, argv);
    
    // Run FLTK event loop
    return Fl::run();
}
