#include <iostream>
#include "gui.h"
#include "main.h"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("bigbee.calculator.base");

  return app->make_window_and_run<CalWindow>(argc, argv);
} 