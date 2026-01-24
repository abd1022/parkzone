#include "MainWindow.h"
#include "SetupScreen.h"
#include "DashboardScreen.h"
#include "RequestScreen.h"
#include "StatusScreen.h"
#include "RollbackScreen.h"
#include "AnalyticsScreen.h"
#include <FL/fl_draw.H>

MainWindow::MainWindow(int w, int h, const char* title) 
    : Fl_Window(w, h, title) {
    
    parking_system = new ParkingSystem();
    
    // Create navigation bar
    createNavigation();
    
    // Create screens (they will be positioned below the navigation)
    int content_y = 60;
    int content_h = h - content_y;
    
    setup_screen = new SetupScreen(0, content_y, w, content_h, this);
    dashboard_screen = new DashboardScreen(0, content_y, w, content_h, this);
    request_screen = new RequestScreen(0, content_y, w, content_h, this);
    status_screen = new StatusScreen(0, content_y, w, content_h, this);
    rollback_screen = new RollbackScreen(0, content_y, w, content_h, this);
    analytics_screen = new AnalyticsScreen(0, content_y, w, content_h, this);
    
    // Show setup screen by default
    showSetupScreen();
    
    end();
}

MainWindow::~MainWindow() {
    delete parking_system;
}

void MainWindow::createNavigation() {
    nav_group = new Fl_Group(0, 0, w(), 60);
    nav_group->box(FL_FLAT_BOX);
    nav_group->color(fl_rgb_color(50, 50, 50));
    
    int btn_w = 130;
    int btn_h = 40;
    int btn_y = 10;
    int spacing = 5;
    int x_pos = 10;
    
    btn_setup = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Setup");
    btn_setup->callback(cb_setup, this);
    btn_setup->color(fl_rgb_color(70, 130, 180));
    btn_setup->labelcolor(FL_WHITE);
    x_pos += btn_w + spacing;
    
    btn_dashboard = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Dashboard");
    btn_dashboard->callback(cb_dashboard, this);
    btn_dashboard->color(fl_rgb_color(70, 130, 180));
    btn_dashboard->labelcolor(FL_WHITE);
    x_pos += btn_w + spacing;
    
    btn_request = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Request Parking");
    btn_request->callback(cb_request, this);
    btn_request->color(fl_rgb_color(70, 130, 180));
    btn_request->labelcolor(FL_WHITE);
    x_pos += btn_w + spacing;
    
    btn_status = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Status");
    btn_status->callback(cb_status, this);
    btn_status->color(fl_rgb_color(70, 130, 180));
    btn_status->labelcolor(FL_WHITE);
    x_pos += btn_w + spacing;
    
    btn_rollback = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Rollback");
    btn_rollback->callback(cb_rollback, this);
    btn_rollback->color(fl_rgb_color(70, 130, 180));
    btn_rollback->labelcolor(FL_WHITE);
    x_pos += btn_w + spacing;
    
    btn_analytics = new Fl_Button(x_pos, btn_y, btn_w, btn_h, "Analytics");
    btn_analytics->callback(cb_analytics, this);
    btn_analytics->color(fl_rgb_color(70, 130, 180));
    btn_analytics->labelcolor(FL_WHITE);
    
    nav_group->end();
}

void MainWindow::hideAllScreens() {
    setup_screen->hide();
    dashboard_screen->hide();
    request_screen->hide();
    status_screen->hide();
    rollback_screen->hide();
    analytics_screen->hide();
}

void MainWindow::showSetupScreen() {
    hideAllScreens();
    setup_screen->show();
}

void MainWindow::showDashboardScreen() {
    hideAllScreens();
    dashboard_screen->refresh();
    dashboard_screen->show();
}

void MainWindow::showRequestScreen() {
    hideAllScreens();
    request_screen->show();
}

void MainWindow::showStatusScreen() {
    hideAllScreens();
    status_screen->show();
}

void MainWindow::showRollbackScreen() {
    hideAllScreens();
    rollback_screen->show();
}

void MainWindow::showAnalyticsScreen() {
    hideAllScreens();
    analytics_screen->refresh();
    analytics_screen->show();
}

// Navigation callbacks
void MainWindow::cb_setup(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showSetupScreen();
}

void MainWindow::cb_dashboard(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showDashboardScreen();
}

void MainWindow::cb_request(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showRequestScreen();
}

void MainWindow::cb_status(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showStatusScreen();
}

void MainWindow::cb_rollback(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showRollbackScreen();
}

void MainWindow::cb_analytics(Fl_Widget* w, void* data) {
    MainWindow* mw = (MainWindow*)data;
    mw->showAnalyticsScreen();
}
