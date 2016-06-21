#include<gtkmm.h>
#include"bomber.xpm"
#include"vim.xpm"
using namespace std;
using namespace Gtk;

int main(int argc, char** argv)
{
	int i=1;
	auto app = Application::create(i, argv);
	//Image image(Gdk::Pixbuf::create_from_xpm_data(bomber_xpm));
	Image image2(Gdk::Pixbuf::create_from_xpm_data(vim_xpm));
	Window win;
	win.add(image2);
	win.show_all_children();
	app->run(win);
}
	
