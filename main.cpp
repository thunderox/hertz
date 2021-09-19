
#include "main.h"
#include "song.h"
#include "window.h"
#include "jack_manager.h"
#include "delirium_ui/delirium_ui.hpp"
#include <cairo.h>
#include <cairo-xcb.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <unistd.h>
#include "string"

int main()
{	
        window window_manager;
        int width = window_manager.s->width_in_pixels;
	int height = window_manager.s->height_in_pixels;
	
      	int win_id = window_manager.create_window(0,0,width,height);
      	window_manager.set_window_title(win_id, "Hertz (A Digital Audio Workstation) - ThunderOx Software 2021");

	cairo_surface_t *surface = window_manager.get_window_cairo_surface(win_id);     
      	cairo_t *cr = window_manager.get_window_cairo_context(win_id);
        
      	cairo_select_font_face (cr, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
              
        Delirium_UI_Surface* GUI = Delirium_UI_Init(width,height,160,90);
        
 	float panelX = 0.5;
	float panelY = 0.5;
	
	GUI->group_visible[0] = true;
	GUI->draw_flag = true;					
	GUI->drag = 0;
	GUI->drawn_at_least_once = 1;
	GUI->current_widget = -1;

	int osc_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 64,12, "SONG", -1);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, osc_panel, "global", "");
	
	int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 3,3, "ACTIVE", -1);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_active, "global", "");
        
	Delirium_UI_Group_Set_Active_Widgets(GUI);
        Delirium_UI_Display_All(GUI, cr);
        
	cairo_surface_flush(surface);
	xcb_flush(window_manager.c);
	
	song my_song;
	my_song.set_name("This is my first ever song");

	cout << my_song.get_name() << endl;
		
	jack_manager my_jack_manager;

	my_jack_manager.create_client("piggy");
	my_jack_manager.create_jack_out("piggy","track-1");
		
	my_jack_manager.connect_audio("piggy", "");

	window_event win_ev;
	win_ev.type = WINDOW_EVENT_TYPE_NONE;
	
	while (win_ev.type != WINDOW_EVENT_TYPE_EXIT)
	{
		win_ev = window_manager.wait_for_event();
		
		if (win_ev.mouse_button == WINDOW_EVENT_TYPE_MOUSE_BUTTON_RELEASE)
		{
			GUI->draw_flag = true;	
			Delirium_UI_Display_All(GUI, cr);
			cairo_surface_flush(surface);
			xcb_flush(window_manager.c);
			
		}
		
		if (win_ev.mouse_button == 1) Delirium_UI_Left_Button_Press(GUI, cr, win_ev.x, win_ev.y);
		if (win_ev.type == WINDOW_EVENT_TYPE_MOUSE_OVER) Delirium_UI_MouseOver(GUI, cr, win_ev.x, win_ev.y);
		
		
	}
		 
	 my_jack_manager.close_client("piggy");
	 	
	cout << endl;
		
	return 0;
}
