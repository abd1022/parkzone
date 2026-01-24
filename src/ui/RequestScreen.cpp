#include "RequestScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <sstream>

RequestScreen::RequestScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    int label_w = 150;
    int input_w = 250;
    int spacing = 10;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "Request Parking");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 60;
    
    // Vehicle ID input
    Fl_Box* vehicle_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Vehicle ID:");
    vehicle_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    vehicle_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 40;
    
    // Zone ID input
    Fl_Box* zone_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Requested Zone:");
    zone_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    zone_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 40;
    
    // Create request button
    btn_create_request = new Fl_Button(x + 30 + label_w + spacing, y_pos, 150, 40, "Create Request");
    btn_create_request->callback(cb_create_request, this);
    btn_create_request->color(fl_rgb_color(100, 200, 100));
    btn_create_request->labelcolor(FL_WHITE);
    btn_create_request->labelfont(FL_BOLD);
    y_pos += 60;
    
    // Result display
    Fl_Box* result_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Result:");
    result_header->labelfont(FL_BOLD);
    result_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 30;
    
    result_buffer = new Fl_Text_Buffer();
    result_display = new Fl_Text_Display(x + 20, y_pos, w - 40, h - (y_pos - y) - 20);
    result_display->buffer(result_buffer);
    result_display->box(FL_DOWN_BOX);
    result_display->textfont(FL_COURIER);
    result_display->textsize(12);
    result_display->color(fl_rgb_color(255, 255, 255));
    
    result_buffer->text("Ready to create parking requests.\n\n"
                       "Instructions:\n"
                       "1. Enter Vehicle ID (must be registered)\n"
                       "2. Enter Requested Zone ID\n"
                       "3. Click 'Create Request'\n\n"
                       "The system will:\n"
                       "- Create a request (ID format: R1, R2, etc.)\\n"
                       "- Attempt allocation\n"
                       "- Show slot details if successful\n");
    
    end();
}

RequestScreen::~RequestScreen() {
    delete result_buffer;
}

void RequestScreen::cb_create_request(Fl_Widget* w, void* data) {
    RequestScreen* screen = (RequestScreen*)data;
    const char* vehicle_id = screen->vehicle_id_input->value();
    const char* zone_id = screen->zone_id_input->value();
    
    if (strlen(vehicle_id) == 0 || strlen(zone_id) == 0) {
        screen->result_buffer->text("Error: Both Vehicle ID and Zone ID are required.\n");
        return;
    }
    
    std::ostringstream result;
    result << "=== Creating Parking Request ===\n";
    result << "Vehicle: " << vehicle_id << "\n";
    result << "Requested Zone: " << zone_id << "\n\n";
    
    // Create request
    std::string request_id = screen->main_window->getParkingSystem()->createParkingRequest(vehicle_id, zone_id);
    
    if (request_id.empty() || request_id == "INVALID") {
        result << "✗ Failed to create request\n";
        result << "Possible reasons:\n";
        result << "- Vehicle not registered\n";
        result << "- Zone does not exist\n";
        result << "- Vehicle already has active request\n";
        screen->result_buffer->text(result.str().c_str());
        return;
    }
    
    result << "✓ Request created: " << request_id << "\n\n";
    screen->last_request_id = request_id;
    
    // Attempt allocation
    result << "=== Attempting Allocation ===\n";
    bool allocated = screen->main_window->getParkingSystem()->allocateParking(request_id);
    
    if (allocated) {
        result << "✓ Parking allocated successfully!\n";
        result << "Request ID: " << request_id << "\n\n";
        result << "Check the Status screen for details.\n\n";
        result << "Next steps:\n";
        result << "1. Go to Status screen\n";
        result << "2. Mark as OCCUPIED when vehicle arrives\n";
        result << "3. RELEASE when vehicle leaves\n";
    } else {
        result << "✗ Allocation failed\n";
        result << "Possible reasons:\n";
        result << "- No slots available in requested zone\n";
        result << "- No slots in adjacent zones\n";
        result << "- System capacity full\n";
    }
    
    screen->result_buffer->text(result.str().c_str());
    
    // Clear inputs
    screen->vehicle_id_input->value("");
    screen->zone_id_input->value("");
}
