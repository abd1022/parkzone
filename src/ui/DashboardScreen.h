#ifndef DASHBOARD_SCREEN_H
#define DASHBOARD_SCREEN_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

class MainWindow;

class DashboardScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    // Dashboard stats
    Fl_Box* lbl_total_slots;
    Fl_Box* lbl_occupied_slots;
    Fl_Box* lbl_available_slots;
    Fl_Box* lbl_active_requests;
    Fl_Box* lbl_completed_requests;
    Fl_Box* lbl_cancelled_requests;
    
    Fl_Button* btn_refresh;
    
    void updateStats();
    
public:
    DashboardScreen(int x, int y, int w, int h, MainWindow* mw);
    
    void refresh();
    
    static void cb_refresh(Fl_Widget* w, void* data);
};

#endif // DASHBOARD_SCREEN_H
