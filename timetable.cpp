#include"timetable.h"
#include<iostream>
#include<fstream>
#include<algorithm>
using namespace std;
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
	set_label(subject);
	for(int pos=0; pos != string::npos; ) {//get_width() > 50) {
		pos = subject.find(' ', pos);
		subject[pos] = '\n';
		set_label(subject);
	}
	//set_line_wrap(true);
	signal_clicked().connect(bind(&MButton::on_click, this));
	set_comment();
	if(popup.contents != "") set_label("*\n" + get_label());
}

void MButton::set_comment()
{
	ifstream f("note.txt");
	int d, s, n;
	string str, last, tmp;
	while(f >> d >> s >> n){ 
		while(--n > 0) {
			getline(f, tmp);
			str += tmp + '\n';
		}
		f >> tmp;
		str += tmp;
		if(d == day && s == start) last = str;
		str = "";
	}
	popup.prepare(day, start, last);
}

void MButton::on_click()
{
	cout << professor << ' ' << classroom << endl;
	popup.show();
}

void MVBox::pack(const TimeTable& mb)
{
	int gab;
	if(buttons.size() == 0) gab = mb.start - START*100;
	else gab = mb.start - buttons.back()->get_end();
	if(gab != 0) {
		auto a = make_shared<Frame>();
		a->set_size_request(50, RATIO * gab);
		frames.push_back(a);
		pack_start(*a, PACK_SHRINK);
	}
	auto a = make_shared<MButton>(mb);
	pack_start(*a, PACK_SHRINK);
	buttons.push_back(a);
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


Win::Win(const TimeTable* tt)
{
	add(hbox);
	for(int i=0; i<6; i++) {
		vbox[i].set_size_request(50);
		hbox.pack_start(vbox[i], false, false);//PACK_SHRINK);
	}
	for(int i=0; i<END-START; i++) {
		vbox[0].pack_start(frame[i]);
		frame[i].add(label[i]);
		int j = (i + START) % 12;
		if(j == 0) label[i].set_label("12:00~\n1:00");
		else label[i].set_label(to_string(j) + ":00 ~\n"+to_string(j+1) + ":00");
		label[i].set_size_request(50, RATIO * 98);//frame need 2 pixel more
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
