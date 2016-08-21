#include<gtkmm.h>
#include<string>
class Winmain : public Gtk::Window
{
public:
	Winmain();
	
protected:
	std::string googlemap(float latitude, float longitude, int zoom_lvl,
			int width_pixel, int height_pixel, std::string additional="");
	std::string googlemap(std::vector<std::pair<float, float>> places);
};
