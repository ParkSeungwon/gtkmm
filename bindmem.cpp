#include<gtkmm.h>
#include<string>
#include<functional>
#include<iostream>
using namespace std;

struct Inter
{
	virtual void vf(int n) = 0; 
} *inter;

class Bt : public Gtk::Button
{
public:
	Bt() {
		signal_clicked().connect(bind(&Bt::on_click, this));
	}

protected:
	int n = 0;
	virtual void on_click() {
		inter->vf(n);
		n++;
	}
};

class Win : public Gtk::Window, public Inter
{
public:
	Win() {
		inter = this;
		add(vb);
		for(int i=0; i<3; i++) {
			bt[i].set_label("this is button" + to_string(i));
			vb.add(bt[i]);
		}
		show_all_children();
	}
	void vf(int n) {
		cout << n << endl;
	}

protected:
	Gtk::VBox vb;
	Bt bt[3];
};

int main(int c, char** v)
{
	auto app = Gtk::Application::create(c, v, "");
	Win win;
	app->run(win);
}
