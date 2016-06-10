#include <gtkmm.h>
#include<iostream>
#include<random>
#include<algorithm>
#include"matrix.cc"
using namespace std;
using namespace Gtk;

class MButton : public Button
{
public:
	MButton() {
		add_events(Gdk::BUTTON_PRESS_MASK);
	}

protected:
	bool on_button_press_event(GdkEventButton* e) {
		if(e->type == GDK_BUTTON_PRESS && e->button == 3) 
			set_label("v");
		else if(e->type == GDK_BUTTON_PRESS && e->button == 1) clicked();
		return true;
	}
};

class Win : public Window
{
public:
	Win(int w, int h) {
		width = w; height = h;
		add(grid);
		grid.set_row_homogeneous(true);
		grid.set_column_homogeneous(true);
		matrix = new Matrix<char>(w, h);
		bomb = w * h / 6;
		p = matrix->ptr();
		vector<int> v;
		for(int i=0; i<w*h; i++) v.push_back(i);
		shuffle(v.begin(), v.end(), random_device());
		for(int i=0; i<bomb; i++) p[v[i]] = '*';
		for(int i=0; i<w*h; i++) { 
			if(p[i] != '*') p[i] = mine_count(i%w, i/w);
		}
		btn = new MButton[w*h];
		for(int i=0; i<w*h; i++) {
			btn[i].set_size_request(30, 30);
			grid.attach(btn[i], i%w, i/w, 1, 1);
			btn[i].signal_clicked().connect(bind(&Win::on_click, this, i));
		}
		show_all_children();
	}
	~Win() { 
		delete [] btn;
		delete matrix;
	}

protected:
	Matrix<char>* matrix;
	Grid grid;
	MButton* btn;
	void on_click(int n) {
		if(p[n] == 0) domino(n%width, n/width);
		else if(p[n] == '*') {
			btn[n].set_label("*");
			MessageDialog dialog(*this, "Boom!!!",
					false /* use_markup */, MESSAGE_QUESTION,
					Gtk::BUTTONS_OK);
			for(int i=0; i<width*height; i++) {
				if(p[i] == 42) btn[i].set_label("*");
				else btn[i].set_label(to_string(p[i]));
			}
			dialog.set_position(WIN_POS_CENTER_ALWAYS);
			int result = dialog.run();
			hide();
		} else dig(n);
	}

private:
	int width, height, bomb;
	char* p;
	int mine_count(int x, int y) {
		auto m = this->matrix->surround(0);
		x++; y++;
		int n = 0;
		if(m(x-1, y-1) == '*') n++;
		if(m(x, y-1) == '*') n++;
		if(m(x+1, y-1) == '*') n++;
		if(m(x-1, y) == '*') n++;
		if(m(x+1, y) == '*') n++;
		if(m(x+1, y+1) == '*') n++;
		if(m(x-1, y+1) == '*') n++;
		if(m(x, y+1) == '*') n++;
		return n;
	}

	void dig(int n) {
		static int dug = 0;
		if(btn[n].get_label() != to_string(p[n])) {
			btn[n].set_label(to_string(p[n]));
			dug++;
		}
		if(dug == width*height-bomb) { 
			MessageDialog dialog(*this, "Complete!!!",
					false /* use_markup */, MESSAGE_QUESTION, Gtk::BUTTONS_OK);
			dialog.set_position(WIN_POS_CENTER_ALWAYS);
			int result = dialog.run();
		}
	}

	void domino(int x, int y) {
		dig(y * width + x);
		auto m = this->matrix->surround(-1);
		for(int i=-1; i<2; i++) {
			for(int j=-1; j<2; j++) {
				if(m(x+i+1, y+j+1) == 0 && btn[(y+j) * width + x+i].get_label() != "0") domino(x+i, y+j);
				else if(m(x+i+1, y+j+1) > 0) dig((y+j)*width+(x+i));
			}
		}
	}
};

int main(int argc, char** argv) {
	int i, j;
	cout << "specify width height.";
	cin >> i >> j;
	auto app = Application::create(argc, argv);
	Win win(i, j);
	return app->run(win);
}
