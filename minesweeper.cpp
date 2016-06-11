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
		if(e->type == GDK_BUTTON_PRESS && e->button == 3) {
			if(get_label() == "v") set_label("");
			else set_label("v");
		} else if(get_label() != "v" && e->type == GDK_BUTTON_PRESS && e->button == 1) clicked();
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
		for(int i=0; i<w*h; i++) if(p[i] != '*') p[i] = mine_count(i%w, i/w);

		btn = new MButton[w*h];
		for(int i=0; i<w*h; i++) {
			btn[i].set_size_request(38, 38);
			grid.attach(btn[i], i%w, i/w, 1, 1);
			btn[i].signal_clicked().connect(bind(&Win::on_click, this, i));
		}
		move(200, 200);
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
			for(int i=0; i<width*height; i++) {
				if(p[i] == 42) btn[i].set_label("*");
				else btn[i].set_label(to_string(p[i]));
			}
			message("Boom!!!");
			hide();
		} else dig(n);
	}

private:
	int width, height, bomb;
	char* p;
	void message(const char* str) {
		MessageDialog dialog(*this, str, false, MESSAGE_QUESTION, BUTTONS_OK);
		int x, y;
		get_position(x, y);
		dialog.move(x-170, y);
		dialog.run();
	}
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
		if(dug == width*height-bomb) message("Complete!!!");
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
	if(argc < 3) {
		cout << "specify width height." << endl;
		return 0;
	}
	int i=1;
	auto app = Application::create(i, argv);
	Win win(atoi(argv[1]), atoi(argv[2]));
	return app->run(win);
}
