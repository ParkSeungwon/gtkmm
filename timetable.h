#pragma once
#include<gtkmm.h>
#include<string>
#include<vector>

#define RATIO 1//table stretch rate
#define START 10//table beginning time
#define END 18//table ending time

struct TimeTable {
	int day, start, end;
	std::string subject, professor, classroom;
} timetable[] = {//new line at space of subject
	{1, 1050, 1200, "인도선 사상", "안병희", "만해관 B263"},
	{1, 1350, 1500, "불교학 탐색", "고영섭", "만해관 B255"},
	{1, 1500, 1800, "C언어 실습", "강군", "여기"},
	{2, 1200, 1350, "철학의 근본 문제", "유흔우", "만해관 B158"},
	{2, 1450, 1600, "자연과 기술 명작 세미나", "손향구", "학술관 357"},
	{2, 1650, 1800, "서양 철학의 고전", "한병준", "만해관 B158"},
	{3, 1050, 1200, "인도선 사상", "안병희", "만해관 B263"},
	{4, 1200, 1350, "철학의 근본 문제", "유흔우", "만해관 B158"},
	{4, 1450, 1600, "자연과 기술 명작 세미나", "손향구", "학술관 357"},
	{4, 1650, 1800, "불교학 탐색", "고영섭", "만해관 B255"},
	{5, 1350, 1500, "서양 철학의 고전", "한병준", "만해관 B158"},
	{0,}//ending mark
};

class CommentPopup : public Gtk::Window
{
public:
	CommentPopup();
	void prepare(int d, int s, std::string str);
	std::string contents;
	//Gtk::ScrolledWindow scwin1;
protected:	
	Gtk::TextView textview;
	Gtk::Button ok, cancel;
	Gtk::VBox vbox;
	Gtk::HBox hbox;
	int day, start;
	void on_ok_clicked();
	void on_cancel_clicked(){hide();}
};

class MButton : public Gtk::Button
{
public:
	MButton(const TimeTable& tt);
	int get_end() {return end;}
	void set_comment();
	void set_label();

protected:
	CommentPopup popup;

	int start, end, day;
	std::string professor, subject, classroom;
	
	std::pair<int,int> time();
	void on_click() {popup.show();}

private:
	std::string utf8chr(int cp);
};

class MVBox : public Gtk::VBox
{
public:
	void pack(const TimeTable& mb);
	virtual ~MVBox();
	
protected:
	std::vector<MButton*> buttons;
	std::vector<Gtk::Frame*> frames;
};

class Win : public Gtk::Window
{
public:
	Win(const TimeTable* tt);

protected:
	Gtk::HBox hbox;
	MVBox vbox[6];
	Gtk::Label label[12];
	Gtk::Frame frame[12];
};


