#include<gtkmm.h>
#include<iostream>
#include<functional>
#define RATIO 100
using namespace std;
using namespace Gtk;

float func(float x) {return sin(x);}
float func2(float x) {return -x*x*x;}

float normal(float x) {
	float sigma = 1;;
	float m = 0;
	return 1/(sigma*sqrt(2*M_PI)) * exp(-(x-m)*(x-m)/(2*sigma*sigma)); 
}

class SketchBook : public DrawingArea
{
public:
	SketchBook() {
		set_size_request(1000, 1000);
	}

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		Allocation alloc = get_allocation();
		int w = alloc.get_width();
		int h = alloc.get_height();
		//cr->set_line_width(3.0);
		cr->translate(500, 500);
		cr->arc(0, 0, 150, M_PI, 2*M_PI);
		cr->close_path();
		cr->move_to(0,0);
		float px = 150 * cos(M_PI / 4);
		float py = -150 * sin(M_PI / 4);
		cr->line_to(px, py);
		cr->line_to(-150,0);
		cr->move_to(px, py);
		cr->line_to(150,0);
		cr->move_to(px, py);
		cr->line_to(150 - 2 * (150 - px), 0);
		cr->stroke();
		float r = 150 * (1-cos(M_PI / 4)) * tan(M_PI * 3/16);
		cr->arc(px, -r, r, 0, 2 * M_PI);
		cr->move_to(px, -r);
		cr->line_to(150, 0);
		cr->move_to(px, -r);
		cr->line_to(px, 0);
		cr->stroke();

		Pango::FontDescription font;
		font.set_family("Monospace");
		font.set_weight(Pango::WEIGHT_BOLD);
		font.set_size(30);
		auto layout = create_pango_layout("O");
		//layout->set_font_description(font);
		cr->move_to(0, 0);
		layout->show_in_cairo_context(cr);
		cr->move_to(px, 0);
		layout = create_pango_layout("C");
		layout->show_in_cairo_context(cr);

		cr->move_to(-200, -400);
		for(int i=-200; i<200; i++) cr->line_to(i, -i*i/100);
		//cr->close_path();
		cr->save();
		cr->set_source_rgba(0.3, 0.9, 0.3, 0.3);
		cr->fill();
		cr->restore();
		draw(func, -2 * M_PI, 2 * M_PI, cr);
		draw(func2, -5, 5, cr);
		draw(normal, -5, 5, cr);
		return true;
	}
	void draw(function<float(float)> fn, float sx, float ex, 
			const Cairo::RefPtr<Cairo::Context>& cr) {
		cr->move_to(sx * RATIO, fn(sx) * RATIO);
		for(float f = sx + 1/float(RATIO); f < ex; f += 1/float(RATIO)) 
			cr->line_to(f * RATIO, -fn(f) * RATIO);
		cr->stroke();
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
