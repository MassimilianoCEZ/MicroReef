/*
 * main.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 3
 * 50% Massimiliano 50% Carl
 */

#include "gui.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    Simulation simulation;
    auto app = Gtk::Application::create();
	return app->make_window_and_run<Gui>(1, argv, simulation);
}
