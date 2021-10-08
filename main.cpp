
#include <iostream>
#include "deliriumUI/deliriumUI.h"
#include "jack_manager.h"
#include "song.h"
#define NANOVG_GL3_IMPLEMENTATION

using namespace std;

int main()
{

	//------------ SET UP GUI

	deliriumUI main_gui;

	int win = main_gui.create_window(0, 30, main_gui.screen_width, main_gui.screen_height, "Hertz (A Digital Audio Workstation) - ThunderOx Software 2021");
	main_gui.set_current_window(win);
	main_gui.set_window_grid(win, 64, 48);

	jack_manager my_jack_manager;
	my_jack_manager.set_current_manager(&my_jack_manager);
	my_jack_manager.create_client("hertz");
	my_jack_manager.create_jack_out("hertz","track-1");	
	my_jack_manager.create_jack_out("hertz","track-2");	
	my_jack_manager.create_jack_out("hertz","track-3");		
	my_jack_manager.create_jack_out("hertz","track-4");	
	my_jack_manager.create_jack_out("hertz","track-5");	
	my_jack_manager.create_jack_out("hertz","track-6");	
	my_jack_manager.connect_audio("hertz", ""); 	 	
	
	song my_song;
	my_song.window_width = main_gui.screen_width;
	my_song.window_height = main_gui.screen_height;
	my_song.set_name("This is my first ever song");
	
	my_song.load_midi_file("test4.mid");
	
	std::stringstream ss;
	
	int panel_mixer = main_gui.create_widget(widget_type_panel, win, 1,0.5, 42,14, "MIXER");
	int panel_editor = main_gui.create_widget(widget_type_panel, win, 44,0.5, 20,30, "EDITOR");
	
	for (int tr=1; tr < my_song.get_number_of_tracks(); tr++)
	{	ss.str("");
		ss << tr;
		main_gui.create_widget(widget_type_button, win, (tr*3.25)+4, 1, 3, 1, my_song.get_track_name(tr));
		
		int fader_widget_number = main_gui.create_widget(widget_type_fader, win, (tr*3.25)+4, 2.25, 1.5, 8, ss.str());
		main_gui.windows[win].widgets[fader_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[fader_widget_number]->set_default_value(0.5);
		
		int pan_widget_number = main_gui.create_widget(widget_type_knob, win, (tr*3.25)+4.5, 11, 2,3, "PAN");
		main_gui.windows[win].widgets[pan_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[pan_widget_number]->set_default_value(0.5);
		
		main_gui.create_widget(widget_type_grid, win, (tr*3.25)+4, 16, 3.2, 24, "");
		
	}
	
	int volume_knob = main_gui.create_widget(widget_type_knob, win, 1, 8, 4,5, "VOLUME");
	main_gui.windows[win].widgets[volume_knob]->set_value(0.5);
	main_gui.windows[win].widgets[volume_knob]->set_default_value(0.5);

	int current_window = main_gui.current_window;
	NVGcontext* vg = main_gui.windows[current_window].vg;
	
	float old_time = 0;

	main_gui.display_all();
	my_song.draw_track_display(vg);
	nvgEndFrame(vg);
	glfwSwapBuffers(main_gui.windows[current_window].window);
	
	main_gui.display_all();
	my_song.draw_track_display(vg);
	nvgEndFrame(vg);
	glfwSwapBuffers(main_gui.windows[current_window].window);
	
	//------------ MAIN LOOP

	do
	{
		glfwWaitEventsTimeout(0.01);

		if (glfwGetTime() - old_time > 0.01)
		{		
			old_time = glfwGetTime();
			main_gui.main_loop();
		
			if (main_gui.windows[current_window].widget_draw)
			{
	
				old_time = glfwGetTime();
				main_gui.windows[current_window].widget_draw = false;	
			
				my_song.draw_track_display(vg);
				glfwSwapBuffers(main_gui.windows[current_window].window);
			}
		}
		
	} while( glfwGetKey(main_gui.windows[current_window].window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& glfwWindowShouldClose(main_gui.windows[current_window].window) == 0 );
		
	 my_jack_manager.close_client("hertz"); 	
	 

	return 0;
	
}





