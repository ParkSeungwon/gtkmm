#include<iostream>
#include<random>
#include<algorithm>
#include<chrono>
#include<fstream>
#include"matrix.cc"
#include"minesweeper.h"
using namespace std;
using namespace Gtk;
using namespace chrono;

MButton::MButton() 
{
	add_events(Gdk::BUTTON_PRESS_MASK);
}

bool MButton::on_button_press_event(GdkEventButton* e) 
{
	if(e->type == GDK_BUTTON_PRESS && e->button == 3) {
		if(get_label() == "v") set_label("");
		else if (get_label() == "") set_label("v");
	} else if(get_label()!="v" && e->type==GDK_BUTTON_PRESS && e->button==1) 
		clicked();
	return true;
}

Win::Win(int w, int h) : Matrix<char>(w, h) {
	add(grid);
	grid.set_row_homogeneous(true);
	auto slot =	sigc::mem_fun(*this, &Win::time_pass);//bool
	//auto slot =	bind(&Win::time_pass, this);//Glib does not have compatibilty 
	//with bind
	conn = Glib::signal_timeout().connect_seconds(slot, 1);//buggy
	grid.set_column_homogeneous(true);
	bomb = w * h / 6;
	vector<int> v;
	for(int i=0; i<w*h; i++) v.push_back(i);
	shuffle(v.begin(), v.end(), random_device());
	for(int i=0; i<bomb; i++) arr[v[i]] = '*';
	for(int i=0; i<w*h; i++) if(arr[i] != '*') arr[i] = mine_count(i%w, i/w);

	btn = new MButton[w*h];
	for(int i=0; i<w*h; i++) {
		btn[i].set_size_request(38, 38);
		grid.attach(btn[i], i%w, i/w, 1, 1);
		btn[i].signal_clicked().connect(bind(&Win::on_click, this, i));
	}
	move(200, 200);
	show_all_children();
}

Win::~Win() { //virtual->on=false ...
	delete [] btn;
}

void Win::on_click(int n) {
	if(arr[n] == 0) domino(n%width, n/width);
	else if(arr[n] == '*') {
		btn[n].set_label("*");
		for(int i=0; i<width*height; i++) if(arr[i] == 42) btn[i].set_label("*");
		message("Boom!!!\n" + show_bestscore());
		hide();
	} else dig(n);
}

bool Win::time_pass() {//start time static으로 해야 
	set_title(to_string(++time) + " sec passed");
//	while(on) {
//		this_thread::sleep_for(chrono::seconds(1));
		//mtx.lock();
		//mtx.unlock();
//		cout << ++time << " seconds passed\r" << flush;
//	}
	return true;
}

void Win::message(string str) {
	MessageDialog dialog(*this, str.c_str());
	int x, y;
	get_position(x, y);
	dialog.move(x-170, y);
	conn.disconnect();
	dialog.run();
}

int Win::mine_count(int x, int y) {
	auto m = this->surround(0);
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

void Win::dig(int n) {
	static int dug = 0;
	if(btn[n].get_label() != to_string(arr[n])) {
		btn[n].set_label(to_string(arr[n]));
		btn[n].set_sensitive(false);
		dug++;
		if(dug == width*height-bomb) {
			write_score();
			message("Complete!!!\n" + show_bestscore());
		}
	}
}

string Win::show_bestscore() 
{
	string rt;
	ifstream file("bestscore.txt");
	if(file.is_open()) {
		vector<int> v;
		int n[3];
		while(file >> n[0] >> n[1] >> n[2])
			if(n[0] == width && n[1] == height) v.push_back(n[2]);

		sort(v.begin(), v.end());
		rt = "\nbest scores of " + to_string(width) + 'X' + to_string(height) + '\n';
		int i=1;
		for(auto& a : v) {
			rt += to_string(i++) + ". " + to_string(a) + '\n';
			if(i>10) break;
		}
	}
	return rt;
}

void Win::write_score() 
{
	ofstream file("bestscore.txt", ofstream::app);
	file << width << ' ' << height << ' ' << time << endl;
}

void Win::domino(int x, int y) {
	dig(y * width + x);
	auto m = this->surround(-1);
	for(int i=-1; i<2; i++) {
		for(int j=-1; j<2; j++) {
			if(m(x+i+1, y+j+1)==0 && btn[(y+j)*width+x+i].get_label() != "0") 
				domino(x+i, y+j);
			else if(m(x+i+1, y+j+1) > 0) dig((y+j)*width+(x+i));
		}
	}
}


int main(int argc, char** argv) {
	if(argc < 3) {
		std::cout << "specify width height." << std::endl;
		return 0;
	}
	int i=1;
	auto app = Gtk::Application::create(i, argv);
	Win win(atoi(argv[1]), atoi(argv[2]));
	return app->run(win);
}
