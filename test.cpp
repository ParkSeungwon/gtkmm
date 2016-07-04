#include<gtkmm.h>
#include<iostream>
using namespace std;
using namespace Gtk;

class Win : public Window
{
public:
	Win() {
		add(hbox);
		hbox.add(text_field);
		hbox.add(btn);
		btn.add(label);
		label.set_markup("<span foreground=\"red\">OK</span>");
		text_field.set_text("우리는 사나이");
		btn.signal_clicked().connect(bind(&Win::on_click, this));
		show_all_children();
	}

protected:
	Entry text_field;
	Button btn;
	Label label;
	HBox hbox;
	void on_click() {
		MessageDialog msg(*this, "ME");
		msg.set_title("message dialog");
		//msg.set_text("우리는 사나이진짜 ");
		cout << text_field.get_text() << endl;
		msg.run();
	}
};
bool f() { cout << "time" << endl; return true;}

int main(int c, char** v)
{
	auto app = Application::create(c, v);
	Glib::signal_timeout().connect_seconds(sigc::ptr_fun(&f), 3);
	Win win;
	app->run(win);
}
