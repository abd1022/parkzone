#ifndef SETUP_SCREEN_H
#define SETUP_SCREEN_H

#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class MainWindow;

class SetupScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    // Zone registration
    Fl_Input* zone_id_input;
    Fl_Button* btn_add_zone;
    
    // Parking area registration
    Fl_Input* area_zone_input;
    Fl_Input* area_id_input;
    Fl_Int_Input* area_capacity_input;
    Fl_Button* btn_add_area;
    
    // Adjacent zones
    Fl_Input* adj_zone1_input;
    Fl_Input* adj_zone2_input;
    Fl_Button* btn_link_zones;
    
    // Vehicle registration
    Fl_Input* vehicle_id_input;
    Fl_Input* vehicle_pref_zone_input;
    Fl_Button* btn_register_vehicle;
    
    // Status display
    Fl_Text_Display* status_display;
    Fl_Text_Buffer* status_buffer;
    
    void addStatus(const std::string& message);
    
public:
    SetupScreen(int x, int y, int w, int h, MainWindow* mw);
    ~SetupScreen();
    
    // Callbacks
    static void cb_add_zone(Fl_Widget* w, void* data);
    static void cb_add_area(Fl_Widget* w, void* data);
    static void cb_link_zones(Fl_Widget* w, void* data);
    static void cb_register_vehicle(Fl_Widget* w, void* data);
};

#endif // SETUP_SCREEN_H
