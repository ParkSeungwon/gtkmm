#pragma once
#include<gtkmm.h>
#include<thread>
#include<chrono>
#include"matrix.h"
class MButton : public Gtk::Button
{
public:
	MButton();
	
protected:
	bool on_button_press_event(GdkEventButton* e);
};

class Clock : public Gtk::Window
{
public:
	Clock();
	~Clock();
	void set_label(string s);
	bool on = true;
	int time = 0;

protected:
	std::thread th;
	Gtk::Label label;
	bool on_timeout();
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
	Clock clock;

private:
	int bomb;

	void message(const char* str);
	int mine_count(int x, int y);
	void dig(int n);
	void domino(int x, int y);
	void show_bestscore();
	void write_score();
};
