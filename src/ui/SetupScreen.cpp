#include "SetupScreen.h"
#include "MainWindow.h"
#include <FL/fl_draw.H>
#include <sstream>

SetupScreen::SetupScreen(int x, int y, int w, int h, MainWindow* mw) 
    : Fl_Group(x, y, w, h), main_window(mw) {
    
    box(FL_FLAT_BOX);
    color(fl_rgb_color(240, 240, 240));
    
    int y_pos = y + 20;
    int label_w = 150;
    int input_w = 200;
    int btn_w = 120;
    int spacing = 10;
    
    // Title
    Fl_Box* title = new Fl_Box(x + 20, y_pos, w - 40, 30, "System Setup");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y_pos += 50;
    
    // Zone Registration Section
    Fl_Box* zone_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Register Zone");
    zone_header->labelfont(FL_BOLD);
    zone_header->labelsize(14);
    zone_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    zone_header->box(FL_FLAT_BOX);
    zone_header->color(fl_rgb_color(200, 220, 255));
    y_pos += 35;
    
    Fl_Box* zone_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Zone ID:");
    zone_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    zone_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    btn_add_zone = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "Add Zone");
    btn_add_zone->callback(cb_add_zone, this);
    btn_add_zone->color(fl_rgb_color(100, 200, 100));
    y_pos += 50;
    
    // Parking Area Registration Section
    Fl_Box* area_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Register Parking Area");
    area_header->labelfont(FL_BOLD);
    area_header->labelsize(14);
    area_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    area_header->box(FL_FLAT_BOX);
    area_header->color(fl_rgb_color(200, 220, 255));
    y_pos += 35;
    
    Fl_Box* area_zone_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Zone ID:");
    area_zone_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    area_zone_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 35;
    
    Fl_Box* area_id_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Area ID:");
    area_id_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    area_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 35;
    
    Fl_Box* capacity_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Capacity:");
    capacity_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    area_capacity_input = new Fl_Int_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    btn_add_area = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "Add Area");
    btn_add_area->callback(cb_add_area, this);
    btn_add_area->color(fl_rgb_color(100, 200, 100));
    y_pos += 50;
    
    // Adjacent Zones Section
    Fl_Box* adj_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Link Adjacent Zones");
    adj_header->labelfont(FL_BOLD);
    adj_header->labelsize(14);
    adj_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    adj_header->box(FL_FLAT_BOX);
    adj_header->color(fl_rgb_color(200, 220, 255));
    y_pos += 35;
    
    Fl_Box* adj1_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Zone 1 ID:");
    adj1_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    adj_zone1_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 35;
    
    Fl_Box* adj2_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Zone 2 ID:");
    adj2_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    adj_zone2_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    btn_link_zones = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "Link Zones");
    btn_link_zones->callback(cb_link_zones, this);
    btn_link_zones->color(fl_rgb_color(100, 200, 100));
    y_pos += 50;
    
    // Vehicle Registration Section
    Fl_Box* vehicle_header = new Fl_Box(x + 20, y_pos, w - 40, 25, "Register Vehicle");
    vehicle_header->labelfont(FL_BOLD);
    vehicle_header->labelsize(14);
    vehicle_header->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    vehicle_header->box(FL_FLAT_BOX);
    vehicle_header->color(fl_rgb_color(200, 220, 255));
    y_pos += 35;
    
    Fl_Box* vehicle_id_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Vehicle ID:");
    vehicle_id_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    vehicle_id_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    y_pos += 35;
    
    Fl_Box* pref_zone_lbl = new Fl_Box(x + 30, y_pos, label_w, 30, "Preferred Zone:");
    pref_zone_lbl->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
    vehicle_pref_zone_input = new Fl_Input(x + 30 + label_w + spacing, y_pos, input_w, 30);
    btn_register_vehicle = new Fl_Button(x + 30 + label_w + spacing + input_w + spacing, y_pos, btn_w, 30, "Register");
    btn_register_vehicle->callback(cb_register_vehicle, this);
    btn_register_vehicle->color(fl_rgb_color(100, 200, 100));
    y_pos += 50;
    
    // Status Display
    status_buffer = new Fl_Text_Buffer();
    status_display = new Fl_Text_Display(x + w - 400, y + 20, 380, h - 40);
    status_display->buffer(status_buffer);
    status_display->box(FL_DOWN_BOX);
    status_display->textfont(FL_COURIER);
    status_display->textsize(12);
    
    addStatus("System ready. Start by adding zones.");
    
    end();
}

SetupScreen::~SetupScreen() {
    delete status_buffer;
}

void SetupScreen::addStatus(const std::string& message) {
    status_buffer->append(message.c_str());
    status_buffer->append("\n");
    status_display->scroll(status_buffer->length(), 0);
}

void SetupScreen::cb_add_zone(Fl_Widget* w, void* data) {
    SetupScreen* screen = (SetupScreen*)data;
    const char* zone_id = screen->zone_id_input->value();
    
    if (strlen(zone_id) == 0) {
        screen->addStatus("Error: Zone ID cannot be empty");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->addZone(zone_id);
    if (success) {
        screen->addStatus("✓ Zone added: " + std::string(zone_id));
        screen->zone_id_input->value("");
    } else {
        screen->addStatus("✗ Failed to add zone (may already exist)");
    }
}

void SetupScreen::cb_add_area(Fl_Widget* w, void* data) {
    SetupScreen* screen = (SetupScreen*)data;
    const char* zone_id = screen->area_zone_input->value();
    const char* area_id = screen->area_id_input->value();
    const char* capacity_str = screen->area_capacity_input->value();
    
    if (strlen(zone_id) == 0 || strlen(area_id) == 0 || strlen(capacity_str) == 0) {
        screen->addStatus("Error: All fields required for parking area");
        return;
    }
    
    int capacity = atoi(capacity_str);
    if (capacity <= 0) {
        screen->addStatus("Error: Capacity must be positive");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->addParkingAreaToZone(zone_id, area_id, capacity);
    if (success) {
        std::ostringstream oss;
        oss << "✓ Area added: " << area_id << " to " << zone_id << " (capacity: " << capacity << ")";
        screen->addStatus(oss.str());
        screen->area_zone_input->value("");
        screen->area_id_input->value("");
        screen->area_capacity_input->value("");
    } else {
        screen->addStatus("✗ Failed to add area (check zone exists)");
    }
}

void SetupScreen::cb_link_zones(Fl_Widget* w, void* data) {
    SetupScreen* screen = (SetupScreen*)data;
    const char* zone1 = screen->adj_zone1_input->value();
    const char* zone2 = screen->adj_zone2_input->value();
    
    if (strlen(zone1) == 0 || strlen(zone2) == 0) {
        screen->addStatus("Error: Both zone IDs required");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->linkAdjacentZones(zone1, zone2);
    if (success) {
        screen->addStatus("✓ Zones linked: " + std::string(zone1) + " <-> " + std::string(zone2));
        screen->adj_zone1_input->value("");
        screen->adj_zone2_input->value("");
    } else {
        screen->addStatus("✗ Failed to link zones (check both exist)");
    }
}

void SetupScreen::cb_register_vehicle(Fl_Widget* w, void* data) {
    SetupScreen* screen = (SetupScreen*)data;
    const char* vehicle_id = screen->vehicle_id_input->value();
    const char* pref_zone = screen->vehicle_pref_zone_input->value();
    
    if (strlen(vehicle_id) == 0) {
        screen->addStatus("Error: Vehicle ID required");
        return;
    }
    
    bool success = screen->main_window->getParkingSystem()->registerVehicle(vehicle_id, pref_zone);
    if (success) {
        std::string msg = "✓ Vehicle registered: " + std::string(vehicle_id);
        if (strlen(pref_zone) > 0) {
            msg += " (prefers " + std::string(pref_zone) + ")";
        }
        screen->addStatus(msg);
        screen->vehicle_id_input->value("");
        screen->vehicle_pref_zone_input->value("");
    } else {
        screen->addStatus("✗ Failed to register vehicle");
    }
}
