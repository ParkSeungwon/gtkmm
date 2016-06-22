#include<gtkmm.h>
#include"vim.xpm"
using namespace std;
using namespace Gtk;

int main(int argc, char** argv)
{
	auto app = Application::create(argc, argv);
	Image image(Gdk::Pixbuf::create_from_xpm_data(vim_xpm));
	Window win;
	win.add(image);
	win.show_all_children();
	app->run(win);
}
