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
	std::thread th;
	bool on = true;
	int time = 0;
	int bomb;

	bool time_pass();
	void message(const char* str);
	int mine_count(int x, int y);
	void dig(int n);
	void domino(int x, int y);
	void show_bestscore();
	void write_score();
};
