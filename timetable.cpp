#include<fstream>
#include<chrono>
#include<ctime>
#include<unordered_map>
#include"analogue_clock_01.xpm"
#include"timetable.h"
using namespace std;
using namespace chrono;
using namespace Gtk;
//Image MButton::image("alarm.png");
MButton::MButton(const TimeTable& tt) 
	: image(Gdk::Pixbuf::create_from_xpm_data(analogue_clock_01_xpm))
{
	start = tt.start;
	end = tt.end;
	day = tt.day;
	professor = tt.professor;
	subject = tt.subject;
	classroom = tt.classroom;
	set_size_request(50, RATIO*(end - start));
	//set_line_wrap(true);
	signal_clicked().connect(bind(&MButton::on_click, this));
	set_comment();
	set_label();
	add(vbox);
	vbox.pack_end(label);
}

void MButton::set_label()
{//process subject label
	string s = subject;
	for(int pos=0; pos != string::npos; ) {//get_width() > 50) {
		pos = s.find(' ', pos);
		s[pos] = '\n';
	}
	auto a = time();
	if(a.first == day && a.second + 100 >start && a.second < end) {
		vbox.pack_start(image, PACK_SHRINK);
		//s = "----\n" + s;
		//set_image("bomber_hb.png");
	}
	if(popup.contents.size() > 1) s = "*\n" + s;
	label.set_label(s);
}

void MButton::set_comment()
{//read notices
	ifstream f("note.txt");
	int d, s, n;
	string str, last, tmp;
	while(f >> d >> s >> n){ 
		while(--n > 0) {
			getline(f, tmp);
			str += tmp + '\n';
		}
		getline(f, tmp);
		str += tmp;
		if(d == day && s == start) last = str;
		str = "";
	}
	popup.prepare(day, start, last);
	popup.set_title(professor + '(' + classroom + ')');
}

void MButton::on_click()
{
	popup.show();
}

void MVBox::pack(const TimeTable& mb)
{
	int gab;
	if(buttons.size() == 0) gab = mb.start - START*100;
	else gab = mb.start - buttons.back()->get_end();
	if(gab != 0) {
		auto a = new Frame();
		a->set_size_request(50, RATIO * gab);
		frames.push_back(a);
		pack_start(*a, PACK_SHRINK);
	}
	auto a = new MButton(mb);
	pack_start(*a, PACK_SHRINK);
	buttons.push_back(a);
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
	set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	show_all_children();
}

void CommentPopup::prepare(int d, int s, string contents)
{
	day = d; start = s;
	this->contents = contents;
	textview.get_buffer()->set_text(contents);
}

void CommentPopup::on_ok_clicked()
{
	ofstream f("note.txt", ofstream::app);
	string s = textview.get_buffer()->get_text();
	f << day << ' ' << start << ' ' << textview.get_buffer()->get_line_count();
	f << ' ' << textview.get_buffer()->get_text() << endl;
	hide();
}

pair<int, int> MButton::time() {
	auto now = system_clock::now();
	auto tp = system_clock::to_time_t(now);
	string t = ctime(&tp);
	unordered_map<string, int> days {
		{"Mon", 1}, {"Tue", 2}, {"Wed", 3}, {"Thu", 4}, {"Fri", 5}, {"Sat", 6},
		{"Sun", 7}
	};
	return {days[t.substr(0, 3)], stoi(t.substr(11, 5).erase(2, 1))};
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
//		label[i].set_size_request(50, RATIO * 98);//frame need 2 pixel more
	}
	while(tt->day != 0) {
		vbox[tt->day].pack(*tt);
		tt++;
	}
	show_all_children();
}



int main(int argc, char** argv)
{
	auto app = Application::create(argc, argv);
	Win win(timetable);
	app->run(win);
}
