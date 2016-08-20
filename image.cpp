#include<gtkmm.h>
#include<iostream>
using namespace std;
using namespace Gtk;

class Win : public Window
{
public:
	Win(const char* filename) : image(filename) {
		add(swin);
		swin.add(image);
		int w = image.get_pixbuf()->get_width();
		int h = image.get_pixbuf()->get_height();
		cout << "image size " << w << 'X' << h << endl;
		set_default_size(w + 20, h + 20);
		show_all_children();
	}
		
protected:
	Image image;
	ScrolledWindow swin;
};

int main(int c, char** v)
{
	if(c < 2) {
		cout << "usage: " << v[0] << " [image file]" << endl;
		return 0;
	}
	int i = 0;
	auto app = Application::create(i, v);
	Win win(v[1]);
	app->run(win);
}
