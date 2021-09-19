
#include <iostream>
#include <cairo.h>
#include <cairo-xcb.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <vector>
#include <string>

struct window_event
{
	int type;
	int x,y;
	int mouse_button;
};

const int WINDOW_EVENT_TYPE_EXIT = -1;
const int WINDOW_EVENT_TYPE_NONE = 0;
const int WINDOW_EVENT_TYPE_MOUSE_BUTTON_PRESS = 1;
const int WINDOW_EVENT_TYPE_MOUSE_BUTTON_RELEASE = 2;
const int WINDOW_EVENT_TYPE_MOUSE_OVER = 3;
const int WINDOW_EVENT_TYPE_KEY_PRESS = 4;
const int WINDOW_EVENT_TYPE_EXPOSE = 5;

using namespace std;

struct win
{
	xcb_window_t w;
	xcb_gcontext_t g;
	xcb_generic_event_t *e;
	uint32_t mask;
	uint32_t values[2];
	int xpos,ypos,width,height;
	cairo_t *cr;
	cairo_surface_t *surface;
};

class window
{
	public:
	
	window();
	~window();
	int create_window(int,int,int,int);
	window_event wait_for_event();
	cairo_t* get_window_cairo_context(int);
	cairo_surface_t* get_window_cairo_surface(int);
	void set_window_title(int window_number, string);

	xcb_connection_t *c;
	xcb_screen_t *s;

	private:
	
	vector <win> windows;

};
