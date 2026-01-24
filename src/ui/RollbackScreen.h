#ifndef ROLLBACK_SCREEN_H
#define ROLLBACK_SCREEN_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class MainWindow;

class RollbackScreen : public Fl_Group {
private:
    MainWindow* main_window;
    
    // Rollback controls
    Fl_Int_Input* rollback_k_input;
    Fl_Button* btn_rollback;
    Fl_Button* btn_view_history;
    
    // History display
    Fl_Text_Display* history_display;
    Fl_Text_Buffer* history_buffer;
    
public:
    RollbackScreen(int x, int y, int w, int h, MainWindow* mw);
    ~RollbackScreen();
    
    static void cb_rollback(Fl_Widget* w, void* data);
    static void cb_view_history(Fl_Widget* w, void* data);
};

#endif // ROLLBACK_SCREEN_H
