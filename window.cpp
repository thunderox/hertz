
#include "window.h"

//---------------------------------------------------------------------------------
window::window()
{
	// open connection to the server
	c = xcb_connect(NULL,NULL);
	
	 if (xcb_connection_has_error(c))
	 {
		cout << "Cannot open display." << endl;
	}
	
	// get the first screen
	s = xcb_setup_roots_iterator( xcb_get_setup(c) ).data;
}


//---------------------------------------------------------------------------------
window::~window()
{
	xcb_disconnect(c);
}

//---------------------------------------------------------------------------------
int window::create_window(int _xpos, int _ypos, int _width, int _height)
{
	// open connection to the server
	c = xcb_connect(NULL,NULL);
	
	 if (xcb_connection_has_error(c))
	 {
		cout << "Cannot open display." << endl;
		return -1;
	}
	
	win new_window;
	
	// create window
	new_window.w = xcb_generate_id(c);
	new_window.mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	new_window.values[0] = s->black_pixel;
	new_window.values[1] = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE;
	
	xcb_create_window(c, s->root_depth, new_window.w, s->root,
		_xpos,_ypos,_width,_height, 1,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, s->root_visual,
		new_window.mask, new_window.values);

	new_window.xpos = _xpos;
	new_window.ypos = _ypos;
	new_window.width = _width;
	new_window.height = _height;
	
	// map (show) the window
	xcb_map_window(c, new_window.w);
	xcb_flush(c);
	
	xcb_screen_iterator_t iter;
	xcb_depth_iterator_t depth_iter;
	xcb_visualtype_t *visual_type;
	
	depth_iter = xcb_screen_allowed_depths_iterator(s);
	for (; depth_iter.rem; xcb_depth_next(&depth_iter))
	{
		xcb_visualtype_iterator_t visual_iter;
	
	
		visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
		for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
		{
			if (s->root_visual == visual_iter.data->visual_id)
			{
				visual_type = visual_iter.data;
					goto visual_found;
			}
		}
	}
	
	visual_found: ;
	
	new_window.surface = cairo_xcb_surface_create (c, new_window.w, visual_type, _width, _height);
	new_window.cr = cairo_create (new_window.surface);
		
	windows.push_back(new_window);
	return windows.size()-1;
}

//---------------------------------------------------------------------------------
void window::set_window_title(int window_number, string window_title)
{
	if (window_number < 0 || window_number > windows.size()-1) return;
		
	xcb_change_property (c, XCB_PROP_MODE_REPLACE, windows[window_number].w,
		 XCB_ATOM_WM_NAME, XCB_ATOM_STRING	, 8,
		window_title.length(), window_title.c_str());
}

//---------------------------------------------------------------------------------
cairo_t* window::get_window_cairo_context(int window_number)
{
	if (window_number < 0 || window_number > windows.size()-1) return NULL;
		else return windows[window_number].cr;
}

//---------------------------------------------------------------------------------
cairo_surface_t*  window::get_window_cairo_surface(int window_number)
{
	if (window_number < 0 || window_number > windows.size()-1) return NULL;
		else return windows[window_number].surface;
}

//----------------------------------------------------------------------------------

window_event window::wait_for_event()
{
	xcb_generic_event_t* event;
	window_event win_ev;
	
	while(event = xcb_poll_for_event(c) )
	{

		switch (event->response_type)
		{
			// Expose event
			case XCB_EXPOSE:
			win_ev.type = WINDOW_EVENT_TYPE_NONE;
			win_ev.x = -1;
			win_ev.y = -1;
			win_ev.mouse_button = 0;
			return win_ev;
			break;
			
			// Respond if the event is a key press
			case XCB_KEY_PRESS:
			win_ev.type = WINDOW_EVENT_TYPE_KEY_PRESS;
			break;
  
			// Respond if the event is a mouse click
			case XCB_BUTTON_PRESS:
			xcb_button_press_event_t *button_ev;
			button_ev = (xcb_button_press_event_t *)event;

			win_ev.type = WINDOW_EVENT_TYPE_MOUSE_BUTTON_PRESS;
			win_ev.mouse_button = (int)button_ev->detail;
			win_ev.x = button_ev->event_x;
			win_ev.y = button_ev->event_y;
			return win_ev;
			break;
			
			// Respond if the event is a mouse click released
			case XCB_BUTTON_RELEASE:
			
			win_ev.type = WINDOW_EVENT_TYPE_MOUSE_BUTTON_RELEASE;
			break;
			
			// Respond if the event is a mouse click
			case XCB_MOTION_NOTIFY:
			xcb_motion_notify_event_t *motion_ev;
			motion_ev = (xcb_motion_notify_event_t *)event;
			
			win_ev.type = WINDOW_EVENT_TYPE_MOUSE_OVER;
			win_ev.x = motion_ev->event_x;
			win_ev.y = motion_ev->event_y;
			break;
	  
			// Respond if the event is something else
			default:
			cout << event << endl;
			win_ev.type = WINDOW_EVENT_TYPE_NONE;
			win_ev.x = -1;
			win_ev.y = -1;
			win_ev.mouse_button = 0;
			return win_ev;
			break;
		}	
	}

	free (event);

	return win_ev;
	
}







