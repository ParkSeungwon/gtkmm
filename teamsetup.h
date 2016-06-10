#pragma once
#include <gtkmm.h>
#include<memory>

class TeamSetup : public Gtk::Window
{
public:
	TeamSetup(int c);
	virtual ~TeamSetup();
	
protected:
	Gtk::VBox vbox1;
	Gtk::HBox* hboxes;
	Gtk::ComboBoxText* comboboxes;
	Gtk::Button bt1;
	void on_click();
	
private:
	std::shared_ptr<TeamSetup> ts;
};
