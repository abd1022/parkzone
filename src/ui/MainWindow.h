#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include "../include/ParkingSystem.h"

// Forward declarations
class SetupScreen;
class DashboardScreen;
class RequestScreen;
class StatusScreen;
class RollbackScreen;
class AnalyticsScreen;

class MainWindow : public Fl_Window {
private:
    ParkingSystem* parking_system;
    
    // Screens
    SetupScreen* setup_screen;
    DashboardScreen* dashboard_screen;
    RequestScreen* request_screen;
    StatusScreen* status_screen;
    RollbackScreen* rollback_screen;
    AnalyticsScreen* analytics_screen;
    
    // Navigation buttons
    Fl_Group* nav_group;
    Fl_Button* btn_setup;
    Fl_Button* btn_dashboard;
    Fl_Button* btn_request;
    Fl_Button* btn_status;
    Fl_Button* btn_rollback;
    Fl_Button* btn_analytics;
    
    void createNavigation();
    void hideAllScreens();
    
public:
    MainWindow(int w, int h, const char* title);
    ~MainWindow();
    
    ParkingSystem* getParkingSystem() { return parking_system; }
    
    void showSetupScreen();
    void showDashboardScreen();
    void showRequestScreen();
    void showStatusScreen();
    void showRollbackScreen();
    void showAnalyticsScreen();
    
    // Navigation callbacks
    static void cb_setup(Fl_Widget* w, void* data);
    static void cb_dashboard(Fl_Widget* w, void* data);
    static void cb_request(Fl_Widget* w, void* data);
    static void cb_status(Fl_Widget* w, void* data);
    static void cb_rollback(Fl_Widget* w, void* data);
    static void cb_analytics(Fl_Widget* w, void* data);
};

#endif // MAIN_WINDOW_H
