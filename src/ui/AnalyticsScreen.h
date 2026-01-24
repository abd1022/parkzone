#ifndef ANALYTICS_SCREEN_H
#define ANALYTICS_SCREEN_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class MainWindow;

class AnalyticsScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    Fl_Button* btn_refresh;
    Fl_Text_Display* analytics_display;
    Fl_Text_Buffer* analytics_buffer;
    
    void updateAnalytics();
    
public:
    AnalyticsScreen(int x, int y, int w, int h, MainWindow* mw);
    ~AnalyticsScreen();
    
    void refresh();
    
    static void cb_refresh(Fl_Widget* w, void* data);
};

#endif // ANALYTICS_SCREEN_H
