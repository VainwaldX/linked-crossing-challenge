// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707
#include <gtkmm/application.h>
#include <string>
#include "gui.h"
#include "jeu.h"

using namespace std;

int main(int argc, char *argv[])
{
    string file_name("");
    if (argc > 1) {
        file_name = argv[1];
    }
    auto app = Gtk::Application::create();
    return app->make_window_and_run<My_window>(1, argv, file_name);
}
