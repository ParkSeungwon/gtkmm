#include<gtkmm.h>
#include<iostream>
using namespace std;
using namespace Gtk;

class Win : public Window
{
public:
	Win(const char* filename) : image(filename) {
		add(image);
		cout << "image size " << image.get_pixbuf()->get_width() << 'X';
		cout << image.get_pixbuf()->get_height() << endl;
		show_all_children();
	}
		
protected:
	Image image;
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
