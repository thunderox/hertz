
#include <iostream>
#include "deliriumUI/deliriumUI.h"
#include "jack_manager.h"
#include "song.h"

using namespace std;

int main()
{

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
	
	int panel_mixer = main_gui.create_widget(widget_type_panel, win, 1,1, 50,14, "MIXER");
	
	for (int tr=1; tr < my_song.get_number_of_tracks(); tr++)
	{	ss.str("");
		ss << tr;
		main_gui.create_widget(widget_type_button, win, (tr*3.25)+4, 1, 3, 1, my_song.get_track_name(tr));
		
		int fader_widget_number = main_gui.create_widget(widget_type_fader, win, (tr*3.25)+4, 2.25, 1.5, 8, ss.str());
		main_gui.windows[win].widgets[fader_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[fader_widget_number]->set_default_value(0.5);
		
		int pan_widget_number = main_gui.create_widget(widget_type_knob, win, (tr*3.25)+4, 10.5, 1.5,2.5, "PAN");
		main_gui.windows[win].widgets[pan_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[pan_widget_number]->set_default_value(0.5);
	}
	
	int volume_knob = main_gui.create_widget(widget_type_knob, win, 1, 1, 4,5, "VOLUME");
	main_gui.windows[win].widgets[volume_knob]->set_value(0.5);
	main_gui.windows[win].widgets[volume_knob]->set_default_value(0.5);

	main_gui.main_loop();

	 my_jack_manager.close_client("hertz"); 	
	 

	return 0;
	
}





