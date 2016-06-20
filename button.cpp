#include<gtkmm.h>
#include<thread>
#include<iostream>
#include<chrono>
using namespace std;
using namespace Gtk;
using namespace chrono;

class CommentBox : public EventBox
{
public:
	CommentBox() : frame("fr----"), th(&CommentBox::time, this) {
		add_events(Gdk::BUTTON_PRESS_MASK);
		set_size_request(200);
		add(frame);
		frame.add(label);
	}
	virtual ~CommentBox() {
		on = false;
		th.join();
	}
protected:
	bool on_button_press_event(GdkEventButton* e) {
		cout << label.get_label() << " is here" << endl;
		return false;
	}
	void time() {
		int i = 0;
		while(on) {
			this_thread::sleep_for(milliseconds(100));
			label.set_text(to_string(i++));
		}
	}
	thread th;
	Label label;
	Frame frame;
private:
	bool on = true;
};


class Win : public Window
{
public:
	Win() {
		add(cobox);
		show_all_children();
	}
protected:
	CommentBox cobox;
};

int main(int argc, char** argv)
{
	auto app = Application::create(argc, argv);
	Win win;
	app->run(win);
}

