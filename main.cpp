
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
	GUI->current_widget = -1;

	int osc_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 64,12, "SONG", -1);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, osc_panel, "global", "");
	
	int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 3,4, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_active, "global", "");

	int widget_osc2_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 5.5, panelY + 1.25, 3,4, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_active, "global", "");

	int widget_osc3_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 10.5, panelY + 1.25, 3,4, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_active, "global", "");

	int widget_osc4_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 15.5, panelY + 1.25, 3,4, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc4_active, "global", "");
        
      	int widget_osc5_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 20.5, panelY + 1.25, 2,8, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc5_active, "global", "");
	
	int widget_osc6_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Label, 0, panelX + 28.5, panelY + 1.25, 10,2, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc6_active, "global", "");
	
	int widget_osc7_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 40.5, panelY + 1.25, 2,8, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc7_active, "global", "");

	int widget_osc8_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 50.5, panelY + 1.25, 4,4, "ACTIVE", 0);
	Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc8_active, "global", "");
        
        
	Delirium_UI_Group_Set_Active_Widgets(GUI);
        Delirium_UI_Display_All(GUI, cr);
        
	cairo_surface_flush(surface);
	xcb_flush(window_manager.c);
	
	song my_song;
	my_song.window_width = width;
	my_song.window_height = height;
	my_song.set_name("This is my first ever song");

	cout << my_song.get_name() << endl;
	
	
	my_song.load_midi_file("test.mid");
		
	jack_manager my_jack_manager;
	my_jack_manager.set_current_manager(&my_jack_manager);

	my_jack_manager.create_client("piggy");
	my_jack_manager.create_jack_out("piggy","track-1");
		
	my_jack_manager.connect_audio("piggy", "");

	my_song.sample_rate = my_jack_manager.sample_rate;
	cout << my_song.get_name() << " - " << my_song.tempo << " - " << my_song.ppqn << " - " << my_song.sample_rate << " - number of tracks: " << my_song.tracks.size() << " - Number of blocks: " << my_song.blocks.size() << "Number of events in block 0: " << my_song.blocks[0].events.size() << endl;

	window_event win_ev;
	win_ev.type = WINDOW_EVENT_TYPE_NONE;
	
	while (win_ev.type != WINDOW_EVENT_TYPE_EXIT)
	{

		win_ev.type = 0;
		
		win_ev = window_manager.wait_for_event();
		
		//---- TIME TO REDRAW WINDOW --------------------------------------------------
		if (win_ev.type == WINDOW_EVENT_TYPE_EXPOSE)
		{
			GUI->draw_flag = true;	
			Delirium_UI_Display_All(GUI, cr);
			
			my_song.draw_track_display(cr);
			
			cairo_surface_flush(surface);
			xcb_flush(window_manager.c);
		}
		
		//---- MOUSE BUTTON PRESSED --------------------------------------------------
		if (win_ev.type == WINDOW_EVENT_TYPE_MOUSE_BUTTON_PRESS)
		{
			if (win_ev.mouse_button == 1) // Mouse 
			{
				Delirium_UI_Left_Button_Press(GUI, cr, win_ev.x, win_ev.y);
				GUI->drag = true;
				xcb_flush(window_manager.c);
			}
			
			if (win_ev.mouse_button >= 4 && win_ev.mouse_button <= 5) // Mouse Scroll Wheel detected
			{
				int delta = (win_ev.mouse_button==4) - (win_ev.mouse_button==5);
				int current_widget = GUI->current_widget;
				if (current_widget > -1)
				{
					GUI->Widgets[current_widget]->Mouse_Scroll(0,0,delta);
					GUI->Widgets[current_widget]->Draw(cr);
					xcb_flush(window_manager.c);
				}
			}
			
		}	
		
		//---- MOUSE BUTTON PRESSED --------------------------------------------------
		if (win_ev.type == WINDOW_EVENT_TYPE_MOUSE_BUTTON_RELEASE)
		{
			GUI->drag = false;
		}
			
		//---- MOUSE MOVED OVER WINDOW --------------------------------------------------
		if (win_ev.type == WINDOW_EVENT_TYPE_MOUSE_OVER)
		{
		
			Delirium_UI_MouseOver(GUI, cr, win_ev.x, win_ev.y);
			xcb_flush(window_manager.c);
		}
		
		
		
	}
		 
	 my_jack_manager.close_client("piggy");
	 	
	cout << endl;
		
	return 0;
}
