#include<webkit2/webkit2.h>
#include"googlemap.h"
using namespace std;

Winmain::Winmain()
{
	auto webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	add(*Glib::wrap(GTK_WIDGET(webview)));
	string content = " <!DOCTYPE html> <html> <head> <script src='http://maps.googleapis.com/maps/api/js'></script> <script>\n" 
		"function initialize() {"
		"var mapProp = {"
		"center:new google.maps.LatLng(51.508742,-0.120850),"
		"zoom:5,"
		"mapTypeId:google.maps.MapTypeId.ROADMAP"
		"};"
		"var map=new google.maps.Map(document.getElementById('googleMap'),mapProp);"
		"}"
		"google.maps.event.addDomListener(window, 'load', initialize);"
		"</script> </head> <body> <div id='googleMap' style='width:500px;height:380px;'></div> </body> </html> ";
	webkit_web_view_load_html(webview, content.c_str(), "");
	set_default_size(520, 400);
	show_all_children();
}

int main(int c, char** v)
{
	auto app = Gtk::Application::create(c, v, "");
	Winmain window;
	app->run(window);
}



