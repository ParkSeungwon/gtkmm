#include<iostream>
#include<fstream>
#include<chrono>
#include<ctime>
#include"timetable.h"
using namespace std;
using namespace chrono;
using namespace Gtk;

MButton::MButton(const TimeTable& tt) 
{
	start = tt.start;
	end = tt.end;
	day = tt.day;
	professor = tt.professor;
	subject = tt.subject;
	classroom = tt.classroom;
	set_size_request(50, RATIO*(end - start));
	signal_clicked().connect(bind(&MButton::on_click, this));
	set_comment();
	set_label();
}

void MButton::set_label()
{//process subject label
	string s = subject;
	for(size_t pos; (pos = s.find(' ')) != string::npos;) s[pos] = '\n';
	if(popup.contents.size() > 1) s = "*\n" + s;
	auto a = time();
	if(a.first == day && start < a.second + 100 && a.second < end) {
		s = utf8chr(0x23f0) + (s[0] == '*' ? ' ' : '\n') + s;
	}
	Button::set_label(s);
}

void MButton::set_comment()
{//read notices
	ifstream f("note.txt");
	int d, s, n;
	string last, tmp;
	char ch;
	while(f >> skipws >> d >> s >> n >> noskipws >> ch) {
		for(int ln = 0; ln < n; ){
			f >> ch;
			tmp += ch;
			if(ch == '\n') ln++;
		}
		if(d == day && s == start) last = tmp;
		tmp.clear();// = "";
	}
	if(last.back() == '\n') last.erase(last.size()-1, 1);
	popup.prepare(day, start, last);
	popup.set_title(professor + '(' + classroom + ')');
}

void CommentPopup::on_ok_clicked()
{
	ofstream f("note.txt", ofstream::app);
	string s = textview.get_buffer()->get_text();
	if(s.back() != '\n') s += '\n';

	f << day << ' ' << start << ' ' << count(s.begin(), s.end(), '\n');
	f << '\n' << s;
	hide();
}

void MVBox::pack(const TimeTable& mb)
{//pack one class
	int gab = mb.start - (  buttons.size() == 0 ? //first class?
							START * 100 : buttons.back()->get_end()  );
	if(gab != 0) {
		auto a = new Frame();
		frames.push_back(a);
		a->set_size_request(50, RATIO * gab);
		pack_start(*a, PACK_SHRINK);
	}
	auto a = new MButton(mb);
	buttons.push_back(a);
	pack_start(*a, PACK_SHRINK);
}

MVBox::~MVBox()
{
	for(auto& a : buttons) delete a;
	for(auto& a : frames) delete a;
}

CommentPopup::CommentPopup() : ok("ok"), cancel("cancel")
{
	set_title("Notice");
	set_default_size(500, 300);
	textview.set_wrap_mode(Gtk::WRAP_CHAR);
	ok.signal_clicked().connect(bind(&CommentPopup::on_ok_clicked, this));
	cancel.signal_clicked().connect(bind(&CommentPopup::on_cancel_clicked,this));
	add(vbox);
	vbox.pack_start(textview);
	vbox.pack_end(hbox, Gtk::PACK_SHRINK);
	hbox.pack_start(ok);
	hbox.pack_start(cancel);
	set_modal(true);
	set_position(Gtk::WIN_POS_MOUSE);
	show_all_children();
}

void CommentPopup::prepare(int d, int s, string contents)
{
	day = d; start = s;
	this->contents = contents;
	textview.get_buffer()->set_text(contents);
}

pair<int, int> MButton::time()
{//return current day & time in integer
	auto now = system_clock::now();
	auto tp = system_clock::to_time_t(now);
	string t = ctime(&tp);
	string d = t.substr(0, 3);
	const char* day[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	int i=0;
	while(d != day[i]) i++; 
	int h = stoi(t.substr(11, 5).erase(2, 1));
	float minute = h % 100;
	h -= minute;
	minute *= 100.0 / 60.0;
	h += minute;
	return {i+1, h};
}

Win::Win(const TimeTable* tt)
{
	set_title("강의 시간표");
	add(hbox);
	for(int i=0; i<6; i++) {
		vbox[i].set_size_request(50);
		hbox.pack_start(vbox[i], false, false);//PACK_SHRINK);
	}
	for(int i=0; i<END-START; i++) {
		vbox[0].pack_start(frame[i], PACK_SHRINK);
		frame[i].add(label[i]);
		frame[i].set_size_request(50, RATIO * 100);
		int j = (i + START) % 12;
		if(j == 0) label[i].set_label("12:00~\n1:00");
		else label[i].set_label(to_string(j) + ":00 ~\n"+to_string(j+1) + ":00");
	}
	for(; tt->day != 0; tt++) vbox[tt->day].pack(*tt);
	show_all_children();
}

string MButton::utf8chr(int cp)
{
	char c[5]={ 0x00,0x00,0x00,0x00,0x00 };
	if     (cp<=0x7F) { c[0] = cp;  }
	else if(cp<=0x7FF) { c[0] = (cp>>6)+192; c[1] = (cp&63)+128; }
	else if(0xd800<=cp && cp<=0xdfff) {} //invalid block of utf8
	else if(cp<=0xFFFF) { c[0] = (cp>>12)+224; c[1]= ((cp>>6)&63)+128; c[2]=(cp&63)+128; }
	else if(cp<=0x10FFFF) { c[0] = (cp>>18)+240; c[1] = ((cp>>12)&63)+128; c[2] = ((cp>>6)&63)+128; c[3]=(cp&63)+128; }
	return string(c);
}


int main(int argc, char** argv)
{
	auto app = Application::create(argc, argv);
	Win win(timetable);
	app->run(win);
}
