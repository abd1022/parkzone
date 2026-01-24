#include "DashboardScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <sstream>

DashboardScreen::DashboardScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "Dashboard");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 60;
    
    // Stats boxes
    int box_w = (w - 80) / 3;
    int box_h = 80;
    int x_pos = x + 20;
    
    // Total slots
    Fl_Group* total_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    total_group->box(FL_UP_BOX);
    total_group->color(fl_rgb_color(100, 150, 200));
    lbl_total_slots = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Total Slots: 0");
    lbl_total_slots->labelfont(FL_BOLD);
    lbl_total_slots->labelsize(16);
    lbl_total_slots->labelcolor(FL_WHITE);
    lbl_total_slots->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    total_group->end();
    x_pos += box_w + 10;
    
    // Occupied slots
    Fl_Group* occupied_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    occupied_group->box(FL_UP_BOX);
    occupied_group->color(fl_rgb_color(200, 100, 100));
    lbl_occupied_slots = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Occupied: 0");
    lbl_occupied_slots->labelfont(FL_BOLD);
    lbl_occupied_slots->labelsize(16);
    lbl_occupied_slots->labelcolor(FL_WHITE);
    lbl_occupied_slots->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    occupied_group->end();
    x_pos += box_w + 10;
    
    // Available slots
    Fl_Group* available_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    available_group->box(FL_UP_BOX);
    available_group->color(fl_rgb_color(100, 200, 100));
    lbl_available_slots = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Available: 0");
    lbl_available_slots->labelfont(FL_BOLD);
    lbl_available_slots->labelsize(16);
    lbl_available_slots->labelcolor(FL_WHITE);
    lbl_available_slots->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    available_group->end();
    
    y_pos += box_h + 30;
    x_pos = x + 20;
    
    // Request stats
    Fl_Group* active_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    active_group->box(FL_UP_BOX);
    active_group->color(fl_rgb_color(150, 150, 200));
    lbl_active_requests = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Active Requests: 0");
    lbl_active_requests->labelfont(FL_BOLD);
    lbl_active_requests->labelsize(16);
    lbl_active_requests->labelcolor(FL_WHITE);
    lbl_active_requests->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    active_group->end();
    x_pos += box_w + 10;
    
    Fl_Group* completed_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    completed_group->box(FL_UP_BOX);
    completed_group->color(fl_rgb_color(100, 200, 150));
    lbl_completed_requests = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Completed: 0");
    lbl_completed_requests->labelfont(FL_BOLD);
    lbl_completed_requests->labelsize(16);
    lbl_completed_requests->labelcolor(FL_WHITE);
    lbl_completed_requests->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    completed_group->end();
    x_pos += box_w + 10;
    
    Fl_Group* cancelled_group = new Fl_Group(x_pos, y_pos, box_w, box_h);
    cancelled_group->box(FL_UP_BOX);
    cancelled_group->color(fl_rgb_color(200, 150, 100));
    lbl_cancelled_requests = new Fl_Box(x_pos + 10, y_pos + 10, box_w - 20, box_h - 20, "Cancelled: 0");
    lbl_cancelled_requests->labelfont(FL_BOLD);
    lbl_cancelled_requests->labelsize(16);
    lbl_cancelled_requests->labelcolor(FL_WHITE);
    lbl_cancelled_requests->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    cancelled_group->end();
    
    y_pos += box_h + 30;
    
    // Refresh button
    btn_refresh = new Fl_Button(x + 20, y_pos, 150, 40, "Refresh");
    btn_refresh->callback(cb_refresh, this);
    btn_refresh->color(fl_rgb_color(70, 130, 180));
    btn_refresh->labelcolor(FL_WHITE);
    
    end();
    
    updateStats();
}

void DashboardScreen::updateStats() {
    ParkingSystem* system = main_window->getParkingSystem();
    
    // Get actual statistics from the system
    int total = system->getTotalSlots();
    int occupied = system->getOccupiedSlots();
    int available = system->getAvailableSlots();
    int active = system->getActiveRequests();
    int completed = system->getCompletedRequests();
    int cancelled = system->getCancelledRequests();
    
    // Update labels with actual data
    char buffer[100];
    
    sprintf(buffer, "Total Slots: %d", total);
    lbl_total_slots->copy_label(buffer);
    
    sprintf(buffer, "Occupied: %d", occupied);
    lbl_occupied_slots->copy_label(buffer);
    
    sprintf(buffer, "Available: %d", available);
    lbl_available_slots->copy_label(buffer);
    
    sprintf(buffer, "Active Requests: %d", active);
    lbl_active_requests->copy_label(buffer);
    
    sprintf(buffer, "Completed: %d", completed);
    lbl_completed_requests->copy_label(buffer);
    
    sprintf(buffer, "Cancelled: %d", cancelled);
    lbl_cancelled_requests->copy_label(buffer);
}

void DashboardScreen::refresh() {
    updateStats();
    redraw();
}

void DashboardScreen::cb_refresh(Fl_Widget* w, void* data) {
    DashboardScreen* screen = (DashboardScreen*)data;
    screen->refresh();
}
