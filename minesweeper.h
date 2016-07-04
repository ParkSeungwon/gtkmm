#pragma once
#include<gtkmm.h>
#include<thread>
#include<mutex>
#include<chrono>
#include"matrix.h"
class MButton : public Gtk::Button
{
public:
	MButton();
	void set_label(std::string str) { label.set_markup(str); }
	std::string get_label() { return label.get_label(); }
	
protected:
	Gtk::Label label;
	bool on_button_press_event(GdkEventButton* e);
};


class Win : public Gtk::Window, public Matrix<char>
{
public:
	Win(int w, int h);
	virtual ~Win();
	
protected:
	Gtk::Grid grid;
	MButton* btn;
	void on_click(int);

private:
	int time = 0;
	sigc::connection conn;
	int bomb;

	bool time_pass();
	void message(std::string str);
	int mine_count(int x, int y);
	void dig(int n);
	void domino(int x, int y);
	std::string show_bestscore();
	void write_score();
};
