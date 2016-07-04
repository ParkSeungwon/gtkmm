#include<gtkmm.h>
#include<iostream>
using namespace std;
using namespace Gtk;

class SketchBook : public DrawingArea
{
public:
	SketchBook() {}

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		Allocation alloc = get_allocation();
		int w = alloc.get_width();
		int h = alloc.get_height();
		cr->set_line_width(10.0);
		cr->move_to(0,0);
		cr->line_to(w,h);
		cr->stroke();
		cr->arc(400,400,50,0,3);
		cr->stroke();
		return true;
	}
};

class Win : public Window
{
public:
	Win() {
		add(sketchbook);
		show_all_children();
	}
		
protected:
	SketchBook sketchbook;
};

int main(int c, char** v)
{
	auto app = Application::create(c, v);
	Win win;
	return app->run(win);
}
