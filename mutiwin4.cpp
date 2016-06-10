#include <gtkmm.h>
using namespace std;
using namespace Gtk;

class mywindow1 : public Window
{
public:
	Button b;
};

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.gtkmm.tutorial6.base");

    mywindow1 window;

    return app->run(window);
}

