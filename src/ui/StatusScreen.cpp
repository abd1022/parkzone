#include "StatusScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <sstream>
#include <iostream>

StatusScreen::StatusScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    int label_w = 120;
    int input_w = 250;
    int btn_w = 120;
    int spacing = 10;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "Allocation Status & Management");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 60;
    
    // Request ID input
    Fl_Box* request_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Request ID:");
    request_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    request_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    
    btn_view_status = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "View Status");
    btn_view_status->callback(cb_view_status, this);
    btn_view_status->color(fl_rgb_color(70, 130, 180));
    btn_view_status->labelcolor(FL_WHITE);
    y_pos += 50;
    
    // Action buttons
    Fl_Box* actions_lbl = new Fl_Box(x + 30, y_pos, w - 60, 25, "Actions:");
    actions_lbl->labelfont(FL_BOLD);
    actions_lbl->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 30;
    
    int btn_x = x + 30;
    btn_allocate = new Fl_Button(btn_x, y_pos, btn_w, 35, "Allocate");
    btn_allocate->callback(cb_allocate, this);
    btn_allocate->color(fl_rgb_color(100, 200, 100));
    btn_allocate->labelcolor(FL_WHITE);
    btn_x += btn_w + spacing;
    
    btn_occupy = new Fl_Button(btn_x, y_pos, btn_w, 35, "Mark Occupied");
    btn_occupy->callback(cb_occupy, this);
    btn_occupy->color(fl_rgb_color(255, 165, 0));
    btn_occupy->labelcolor(FL_WHITE);
    btn_x += btn_w + spacing;
    
    btn_release = new Fl_Button(btn_x, y_pos, btn_w, 35, "Release");
    btn_release->callback(cb_release, this);
    btn_release->color(fl_rgb_color(100, 150, 200));
    btn_release->labelcolor(FL_WHITE);
    btn_x += btn_w + spacing;
    
    btn_cancel = new Fl_Button(btn_x, y_pos, btn_w, 35, "Cancel");
    btn_cancel->callback(cb_cancel, this);
    btn_cancel->color(fl_rgb_color(200, 100, 100));
    btn_cancel->labelcolor(FL_WHITE);
    y_pos += 50;
    
    // View all button
    btn_view_all = new Fl_Button(x + 30, y_pos, 150, 35, "View All Requests");
    btn_view_all->callback(cb_view_all, this);
    btn_view_all->color(fl_rgb_color(150, 150, 150));
    btn_view_all->labelcolor(FL_WHITE);
    y_pos += 50;
    
    // Status display
    Fl_Box* status_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Details:");
    status_header->labelfont(FL_BOLD);
    status_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 30;
    
    status_buffer = new Fl_Text_Buffer();
    status_display = new Fl_Text_Display(x + 20, y_pos, w - 40, h - (y_pos - y) - 20);
    status_display->buffer(status_buffer);
    status_display->box(FL_DOWN_BOX);
    status_display->textfont(FL_COURIER);
    status_display->textsize(12);
    status_display->color(fl_rgb_color(255, 255, 255));
    
    status_buffer->text("Enter a Request ID and click 'View Status' to see details.\n"
                       "Or click 'View All Requests' to see all requests.\n\n"
                       "State Transitions:\n"
                       "REQUESTED → ALLOCATED → OCCUPIED → RELEASED\n"
                       "REQUESTED → CANCELLED\n"
                       "ALLOCATED → CANCELLED\n");
    
    end();
}

StatusScreen::~StatusScreen() {
    delete status_buffer;
}

void StatusScreen::cb_view_status(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    const char* request_id = screen->request_id_input->value();
    
    if (strlen(request_id) == 0) {
        screen->status_buffer->text("Error: Request ID is required.\n");
        return;
    }
    
    // Capture viewRequestDetails output
    std::ostringstream capture;
    std::streambuf* old_cout = std::cout.rdbuf(capture.rdbuf());
    
    screen->main_window->getParkingSystem()->viewRequestDetails(request_id);
    
    std::cout.rdbuf(old_cout);
    
    screen->status_buffer->text(capture.str().c_str());
}

void StatusScreen::cb_allocate(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    const char* request_id = screen->request_id_input->value();
    
    if (strlen(request_id) == 0) {
        screen->status_buffer->text("Error: Request ID is required.\n");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->allocateParking(request_id);
    
    std::ostringstream result;
    result << "=== Allocate Parking ===\n";
    result << "Request ID: " << request_id << "\n\n";
    
    if (success) {
        result << "✓ Parking allocated successfully!\n";
        result << "\nClick 'View Status' to see allocation details.\n";
    } else {
        result << "✗ Allocation failed.\n";
        result << "Possible reasons:\n";
        result << "- Request not in REQUESTED state\n";
        result << "- No available slots\n";
        result << "- Invalid request ID\n";
    }
    
    screen->status_buffer->text(result.str().c_str());
}

void StatusScreen::cb_occupy(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    const char* request_id = screen->request_id_input->value();
    
    if (strlen(request_id) == 0) {
        screen->status_buffer->text("Error: Request ID is required.\n");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->markAsOccupied(request_id);
    
    std::ostringstream result;
    result << "=== Mark as Occupied ===\n";
    result << "Request ID: " << request_id << "\n\n";
    
    if (success) {
        result << "✓ Marked as OCCUPIED successfully!\n";
        result << "\nVehicle is now parked in the allocated slot.\n";
    } else {
        result << "✗ Failed to mark as occupied.\n";
        result << "Possible reasons:\n";
        result << "- Request not in ALLOCATED state\n";
        result << "- Invalid request ID\n";
    }
    
    screen->status_buffer->text(result.str().c_str());
}

void StatusScreen::cb_release(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    const char* request_id = screen->request_id_input->value();
    
    if (strlen(request_id) == 0) {
        screen->status_buffer->text("Error: Request ID is required.\n");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->releaseParking(request_id);
    
    std::ostringstream result;
    result << "=== Release Parking ===\n";
    result << "Request ID: " << request_id << "\n\n";
    
    if (success) {
        result << "✓ Parking released successfully!\n";
        result << "\nSlot is now available for new allocations.\n";
    } else {
        result << "✗ Failed to release parking.\n";
        result << "Possible reasons:\n";
        result << "- Request not in OCCUPIED state\n";
        result << "- Invalid request ID\n";
    }
    
    screen->status_buffer->text(result.str().c_str());
}

void StatusScreen::cb_cancel(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    const char* request_id = screen->request_id_input->value();
    
    if (strlen(request_id) == 0) {
        screen->status_buffer->text("Error: Request ID is required.\n");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->cancelRequest(request_id);
    
    std::ostringstream result;
    result << "=== Cancel Request ===\n";
    result << "Request ID: " << request_id << "\n\n";
    
    if (success) {
        result << "✓ Request cancelled successfully!\n";
        result << "\nSlot (if allocated) is now available.\n";
    } else {
        result << "✗ Failed to cancel request.\n";
        result << "Possible reasons:\n";
        result << "- Request not in valid cancellable state\n";
        result << "- Invalid request ID\n";
    }
    
    screen->status_buffer->text(result.str().c_str());
}

void StatusScreen::cb_view_all(Fl_Widget* w, void* data) {
    StatusScreen* screen = (StatusScreen*)data;
    
    // Capture viewAllRequests output
    std::ostringstream capture;
    std::streambuf* old_cout = std::cout.rdbuf(capture.rdbuf());
    
    screen->main_window->getParkingSystem()->viewAllRequests();
    
    std::cout.rdbuf(old_cout);
    
    screen->status_buffer->text(capture.str().c_str());
}
