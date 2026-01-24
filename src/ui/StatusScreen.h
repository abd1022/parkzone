#ifndef STATUS_SCREEN_H
#define STATUS_SCREEN_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class MainWindow;

class StatusScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    // Request ID input
    Fl_Input* request_id_input;
    Fl_Button* btn_view_status;
    Fl_Button* btn_allocate;
    Fl_Button* btn_occupy;
    Fl_Button* btn_release;
    Fl_Button* btn_cancel;
    Fl_Button* btn_view_all;
    
    // Status display
    Fl_Text_Display* status_display;
    Fl_Text_Buffer* status_buffer;
    
public:
    StatusScreen(int x, int y, int w, int h, MainWindow* mw);
    ~StatusScreen();
    
    static void cb_view_status(Fl_Widget* w, void* data);
    static void cb_allocate(Fl_Widget* w, void* data);
    static void cb_occupy(Fl_Widget* w, void* data);
    static void cb_release(Fl_Widget* w, void* data);
    static void cb_cancel(Fl_Widget* w, void* data);
    static void cb_view_all(Fl_Widget* w, void* data);
};

#endif // STATUS_SCREEN_H
