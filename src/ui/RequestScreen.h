#ifndef REQUEST_SCREEN_H
#define REQUEST_SCREEN_H

#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class MainWindow;

class RequestScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    // Request inputs
    Fl_Input* vehicle_id_input;
    Fl_Input* zone_id_input;
    Fl_Button* btn_create_request;
    
    // Result display
    Fl_Text_Display* result_display;
    Fl_Text_Buffer* result_buffer;
    
    std::string last_request_id;
    
public:
    RequestScreen(int x, int y, int w, int h, MainWindow* mw);
    ~RequestScreen();
    
    static void cb_create_request(Fl_Widget* w, void* data);
};

#endif // REQUEST_SCREEN_H
