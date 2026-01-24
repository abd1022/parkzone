#include "RollbackScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <sstream>
#include <iostream>

RollbackScreen::RollbackScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    int label_w = 200;
    int input_w = 150;
    int btn_w = 150;
    int spacing = 10;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "Rollback Operations");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 60;
    
    // Description
    Fl_Box* desc = new Fl_Box(x + 30, y_pos, w - 60, 40, 
        "Rollback allows you to undo the last K operations.\n"
        "This will restore slots and request states.");
    desc->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE | FL_ALIGN_WRAP);
    desc->labelsize(12);
    y_pos += 60;
    
    // Rollback K input
    Fl_Box* k_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Number of operations (K):");
    k_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    rollback_k_input = new Fl_Int_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    rollback_k_input->value("1");
    
    btn_rollback = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "Rollback");
    btn_rollback->callback(cb_rollback, this);
    btn_rollback->color(fl_rgb_color(200, 100, 100));
    btn_rollback->labelcolor(FL_WHITE);
    btn_rollback->labelfont(FL_BOLD);
    y_pos += 50;
    
    // View history button
    btn_view_history = new Fl_Button(x + 30, y_pos, btn_w, 35, "View History");
    btn_view_history->callback(cb_view_history, this);
    btn_view_history->color(fl_rgb_color(70, 130, 180));
    btn_view_history->labelcolor(FL_WHITE);
    y_pos += 60;
    
    // History display
    Fl_Box* history_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Operation History:");
    history_header->labelfont(FL_BOLD);
    history_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 30;
    
    history_buffer = new Fl_Text_Buffer();
    history_display = new Fl_Text_Display(x + 20, y_pos, w - 40, h - (y_pos - y) - 20);
    history_display->buffer(history_buffer);
    history_display->box(FL_DOWN_BOX);
    history_display->textfont(FL_COURIER);
    history_display->textsize(12);
    history_display->color(fl_rgb_color(255, 255, 255));
    
    history_buffer->text("Click 'View History' to see rollback operation history.\n\n"
                        "WARNING: Rollback is a powerful operation!\n"
                        "- It undoes allocations, occupations, and releases\n"
                        "- State changes are reversed in LIFO order\n"
                        "- Use with caution in production\n\n"
                        "Example uses:\n"
                        "- Undo accidental allocation\n"
                        "- Fix incorrect state transitions\n"
                        "- Testing and debugging\n");
    
    end();
}

RollbackScreen::~RollbackScreen() {
    delete history_buffer;
}

void RollbackScreen::cb_rollback(Fl_Widget* w, void* data) {
    RollbackScreen* screen = (RollbackScreen*)data;
    const char* k_str = screen->rollback_k_input->value();
    
    if (strlen(k_str) == 0) {
        screen->history_buffer->text("Error: Please enter the number of operations to rollback.\n");
        return;
    }
    
    int k = atoi(k_str);
    if (k <= 0) {
        screen->history_buffer->text("Error: K must be a positive integer.\n");
        return;
    }
    
    std::ostringstream result;
    result << "=== Performing Rollback ===\n";
    result << "Rolling back last " << k << " operation(s)...\n\n";
    
    bool success = screen->main_window->getParkingSystem()->rollback(k);
    
    if (success) {
        result << "✓ Rollback completed successfully!\n";
        result << "\nLast " << k << " operation(s) have been undone.\n";
        result << "Slots and request states restored.\n\n";
        result << "Click 'View History' to see updated history.\n";
    } else {
        result << "✗ Rollback failed.\n";
        result << "Possible reasons:\n";
        result << "- Not enough operations in history\n";
        result << "- K exceeds available operations\n";
        result << "- Rollback manager error\n";
    }
    
    screen->history_buffer->text(result.str().c_str());
}

void RollbackScreen::cb_view_history(Fl_Widget* w, void* data) {
    RollbackScreen* screen = (RollbackScreen*)data;
    
    // Capture viewRollbackHistory output
    std::ostringstream capture;
    std::streambuf* old_cout = std::cout.rdbuf(capture.rdbuf());
    
    screen->main_window->getParkingSystem()->viewRollbackHistory();
    
    std::cout.rdbuf(old_cout);
    
    std::string output = capture.str();
    
    if (output.empty()) {
        output = "No rollback history available.\n"
                "Operations will appear here after:\n"
                "- Allocations\n"
                "- State changes\n"
                "- Releases\n";
    }
    
    screen->history_buffer->text(output.c_str());
}
