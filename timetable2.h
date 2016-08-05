#pragma once
#include<gtkmm.h>
#include<string>
#include<vector>

#define RATIO 0.7//table stretch rate
#define START 9//table beginning time
#define END 23//table ending time

struct TimeTable {
	int day, start, end;
	std::string subject, professor, classroom;
} timetable[] = {//new line at space of subject
	{1, 900, 1000, "자아와 명상1", "이기세(범허)", "학술문화관 102-309"},
	{1, 1000, 1150, "자료 구조와 실습", "정준호", "만해관 B263"},
	{1, 1150, 1300, "인도 인식 논리학", "정준호", "만해관 B263"},
	{1, 1500, 1700, "웹 프로그 래밍", "송양의", "신공학관 B4013"},
	{2, 1300, 1450, "빨리어", "황순일", "만해관 B158"},
	{2, 1500, 1700, "창의적 공학 설계", "황순일", "만해관 B158"},
	{2, 1700, 1820, "시스템 소프트 웨어와 실습", "문봉교", "신공학관"},
	{3, 1150, 1300, "인도 인식 논리학", "정준호", "만해관 B263"},
	{3, 1500, 1700, "웹 프로그 래밍", "송양의", "신공학관 B4013"},
	{3, 1800, 1966, "중국 연구", "김택경", "학림관 J413"},
	{3, 2067, 2233, "불교와 인간", "김한상", "학술문화관"},
	{4, 1000, 1200, "자료 구조와 실습", "정준호", "만해관 B263"},
	{4, 1300, 1450, "빨리어", "황순일", "만해관 B158"},
	{4, 1500, 1700, "창의적 공학 설계", "송양의", "만해관 B158"},
	{4, 1800, 2050, "경제와 사회 명작 세미나", "이상호", "학림관 J104"},
	{5, 1200, 1400, "시스템 소프트 웨어와 실습", "문봉교", "신공학관"},
	{5, 1700, 1800, "자아와 명상2", "문선희", "만해관 303-208"},
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
	Gtk::Label label[20];
	Gtk::Frame frame[20];
};


