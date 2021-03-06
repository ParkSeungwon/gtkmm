#include <string>
#include <iostream>
#include "teamsetup.h"
using namespace std;
using namespace Gtk;

TeamSetup::TeamSetup(int c) : bt1("Start game")
{
	add(vbox1);
	hboxes = new HBox[c];
	comboboxes = new ComboBoxText[c * 3];
	string s;
	for(int i=0; i<c; i++) {
		vbox1.pack_start(hboxes[i]);
		hboxes[i].pack_start(comboboxes[i]);
		hboxes[i].pack_start(comboboxes[i + c]);
		hboxes[i].pack_start(comboboxes[i + c + c]);
		comboboxes[i].append("User");
		comboboxes[i].append("Human");
		comboboxes[i].append("CPU");
		comboboxes[i + c].append("Americans");
		comboboxes[i + c].append("Australians");
		comboboxes[i + c].append("British");
		comboboxes[i + c].append("Canadians");
		comboboxes[i + c].append("Chinese");
		comboboxes[i + c].append("French");
		comboboxes[i + c].append("Germans");
		comboboxes[i + c].append("Italians");
		comboboxes[i + c].append("Japanese");
		comboboxes[i + c].append("Neutral");
		comboboxes[i + c].append("Puppet_States");
		comboboxes[i + c].append("Russians");
		for(int j=1; j<=c; j++) {
			s = "Team" + to_string(j);
			comboboxes[i + c + c].append(s);
		}
		comboboxes[i].set_active(2);
		comboboxes[i + c].set_active(i);
		comboboxes[i + c + c].set_active(i);
	}
	comboboxes[0].set_active(0);
	vbox1.pack_start(bt1);
	bt1.signal_clicked().connect(sigc::mem_fun(*this, &TeamSetup::on_click));
	show_all_children();
}

TeamSetup::~TeamSetup()
{
	delete [] comboboxes;
	delete [] hboxes;
}

void TeamSetup::on_click()
{
	ts = make_shared<TeamSetup>(8);
	ts->show();
	cout << "inside on_click" << endl;
	//show_all_children();
}

int main(int argc, char *argv[])
{
  auto app = Application::create(argc, argv, "");

  TeamSetup window(4);

  //Shows the window and returns when it is closed.
  return app->run(window);
}
