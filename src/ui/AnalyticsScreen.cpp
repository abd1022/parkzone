#include "AnalyticsScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <sstream>
#include <iostream>

AnalyticsScreen::AnalyticsScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "System Analytics");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 50;
    
    // Refresh button
    btn_refresh = new Fl_Button(x + 20, y_pos, 150, 35, "Refresh Analytics");
    btn_refresh->callback(cb_refresh, this);
    btn_refresh->color(fl_rgb_color(70, 130, 180));
    btn_refresh->labelcolor(FL_WHITE);
    y_pos += 50;
    
    // Analytics display
    Fl_Box* analytics_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Analytics Report:");
    analytics_header->labelfont(FL_BOLD);
    analytics_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 30;
    
    analytics_buffer = new Fl_Text_Buffer();
    analytics_display = new Fl_Text_Display(x + 20, y_pos, w - 40, h - (y_pos - y) - 20);
    analytics_display->buffer(analytics_buffer);
    analytics_display->box(FL_DOWN_BOX);
    analytics_display->textfont(FL_COURIER);
    analytics_display->textsize(12);
    analytics_display->color(fl_rgb_color(255, 255, 255));
    
    analytics_buffer->text("Click 'Refresh Analytics' to generate current statistics.\n\n"
                          "Metrics include:\n"
                          "- Total requests (completed, cancelled, active)\n"
                          "- Average parking duration\n"
                          "- Zone utilization rates\n"
                          "- Peak usage zones\n"
                          "- Penalty statistics\n\n"
                          "Note: Analytics reflect rollback-adjusted data.\n");
    
    end();
}

AnalyticsScreen::~AnalyticsScreen() {
    delete analytics_buffer;
}

void AnalyticsScreen::updateAnalytics() {
    // Capture displayAnalytics output
    std::ostringstream capture;
    std::streambuf* old_cout = std::cout.rdbuf(capture.rdbuf());
    
    main_window->getParkingSystem()->displayAnalytics();
    
    std::cout.rdbuf(old_cout);
    
    std::string output = capture.str();
    
    if (output.empty()) {
        output = "No analytics data available yet.\n\n"
                "Analytics will be generated after:\n"
                "- Creating and processing requests\n"
                "- Completing parking sessions\n"
                "- Using the system over time\n\n"
                "Start by:\n"
                "1. Setting up zones and parking areas\n"
                "2. Registering vehicles\n"
                "3. Creating parking requests\n"
                "4. Processing allocations\n";
    }
    
    analytics_buffer->text(output.c_str());
}

void AnalyticsScreen::refresh() {
    updateAnalytics();
    redraw();
}

void AnalyticsScreen::cb_refresh(Fl_Widget* w, void* data) {
    AnalyticsScreen* screen = (AnalyticsScreen*)data;
    screen->refresh();
}
