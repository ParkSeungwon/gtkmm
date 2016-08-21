#include<webkit2/webkit2.h>
#include<vector>
#include"googlemap.h"
using namespace std;

Winmain::Winmain()
{
	auto webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	add(*Glib::wrap(GTK_WIDGET(webview)));
	string content = googlemap({{37.325630, 129.017094}, {37.3511, 129.600}});
	webkit_web_view_load_html(webview, content.c_str(), "");
	set_default_size(620, 420);
	show_all_children();
}

string Winmain::googlemap(vector<pair<float, float>> pts)
{
	float minx=1000, maxx=-1000, miny=1000, maxy=-1000;
	for(auto& a : pts) {
		if(a.first < minx) minx = a.first;
		else if(a.first > maxx) maxx = a.first;
		if(a.second < miny) miny = a.second;
		else if(a.second > maxy) maxy = a.second;
	}
	float zoom = max(maxx - minx, maxy - miny);
	string ad;
	int i = 0;
	for(auto& a : pts) {
		ad += "var d" + to_string(i) + " = new google.maps.LatLng(";
		ad += to_string(a.first) + ',' + to_string(a.second) + ");";
		ad += "var m" + to_string(i) + " = new google.maps.Marker({";
		ad += "position:d"+ to_string(i) + ",});m" ;
		ad += to_string(i++) + ".setMap(map);";
	}
	ad += "var trip = [";
	for(int j=0; j<i; j++) ad += 'd' + to_string(j) + ',';
	ad.back() = ']';
	ad += ";var flightPath=new google.maps.Polyline({"
		"path:trip, strokeColor:'#0000FF', strokeOpacity:0.8, strokeWeight:2 });"
		"flightPath.setMap(map);";

	string rt = googlemap((minx+maxx)/2, (miny+maxy)/2, 12, 600, 400, ad);
	return rt;
}

string Winmain::googlemap(float lt, float ln, int z, int w, int h, string ad)
{
	string rt = "<!DOCTYPE html> <html> <head>"
		"<script src='http://maps.googleapis.com/maps/api/js'></script>"
		"<script>function initialize() {var mapProp = {"
		"center:new google.maps.LatLng(";
	rt += to_string(lt) + ',' + to_string(ln) + "), zoom:" + to_string(z) + ',';
	rt += "mapTypeId:google.maps.MapTypeId.ROADMAP};"
		"var map=new google.maps.Map(document.getElementById('googleMap'),mapProp);";
	rt += ad + "}google.maps.event.addDomListener(window, 'load', initialize);"
		"</script> </head> <body> <div id='googleMap' style='width:";
	rt += to_string(w) + "px;height:" + to_string(h) + "px;'> </div></body></html>";
	return rt;
}
	

int main(int c, char** v)
{
	auto app = Gtk::Application::create(c, v, "");
	Winmain window;
	app->run(window);
}



