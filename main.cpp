
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
	
	my_song.load_midi_file("test5.mid");
	
	std::stringstream ss;
	int song_track_widget_number[256];
	int widget_grid_edit[1024];
	
	int panel_mixer = main_gui.create_widget(widget_type_panel, win, 1,0.5, 42,46, "MIXER");
	int panel_editor = main_gui.create_widget(widget_type_panel, win, 50,0.5, 10,30, "EDITOR");
	
	int child_widget = main_gui.create_widget(widget_type_button, win, 0, 2, 3.5, 1, "PICKY");
	main_gui.set_widget_parent(win, child_widget, panel_editor);
	
	child_widget = main_gui.create_widget(widget_type_button, win, 4, 2, 3.5, 1, "BOG 2");
	main_gui.set_widget_parent(win, child_widget, panel_editor);
	
	child_widget = main_gui.create_widget(widget_type_button, win, 8, 2, 3.5, 1, "BOG 3");
	main_gui.set_widget_parent(win, child_widget, panel_editor);

	for (int trk=0; trk < my_song.get_number_of_tracks(); trk++)
	{
		float track_x_pos = 0.5 + (trk * 3.25);
		int wid = main_gui.create_widget(widget_type_grid, win, track_x_pos, 14, 3.2, 24, "");
		main_gui.set_widget_parent(win, wid, panel_mixer);
		
		my_song.tracks[trk].x = main_gui.windows[win].widgets[wid]->x;
		my_song.tracks[trk].y = main_gui.windows[win].widgets[wid]->y;
		my_song.tracks[trk].w = main_gui.windows[win].widgets[wid]->w;
		my_song.tracks[trk].h = main_gui.windows[win].widgets[wid]->h;
		
		widget_grid_edit[trk] = wid;
	}
	
	for (int trk=0; trk < my_song.get_number_of_tracks(); trk++)
	{	
		float track_x_pos = 0.5 + (trk * 3.25);
		ss.str("");
		ss << trk;
		int track_title_button = main_gui.create_widget(widget_type_button, win, track_x_pos, 1, 3, 1, my_song.get_track_name(trk));
		main_gui.set_widget_parent(win, track_title_button, panel_mixer);
		
		int fader_widget_number = main_gui.create_widget(widget_type_fader, win,track_x_pos, 2.25, 1.5, 8, ss.str());
		main_gui.windows[win].widgets[fader_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[fader_widget_number]->set_default_value(0.5);
		main_gui.set_widget_parent(win, fader_widget_number, panel_mixer);
		
		int level_widget_number_left = main_gui.create_widget(widget_type_level, win,track_x_pos + 1.75, 2.25, 0.5, 8, ss.str());
		main_gui.set_widget_parent(win, level_widget_number_left, panel_mixer);
		
		int level_widget_number_right = main_gui.create_widget(widget_type_level, win,track_x_pos + 3 - 0.5, 2.25, 0.5, 8, ss.str());
		main_gui.set_widget_parent(win, level_widget_number_right, panel_mixer);
		
		int pan_widget_number = main_gui.create_widget(widget_type_knob, win, track_x_pos - 0.25, 8.5, 2,3, "PAN");
		main_gui.windows[win].widgets[pan_widget_number]->set_value(0.5);
		main_gui.windows[win].widgets[pan_widget_number]->set_default_value(0.5);
		main_gui.set_widget_parent(win, pan_widget_number, panel_mixer);
	}
	
	int current_window = main_gui.current_window;
	NVGcontext* vg = main_gui.windows[current_window].vg;
	
	float old_time = 0;

	nvgBeginFrame(vg, main_gui.screen_width, main_gui.screen_height, main_gui.pxRatio);
	main_gui.display_all();
	
	for (int trk=0; trk<my_song.get_number_of_tracks(); trk++)
	{
		my_song.draw_track_display(vg, trk);
		glfwSwapBuffers(main_gui.windows[current_window].window);
	}
	nvgEndFrame(vg);
	glfwSwapBuffers(main_gui.windows[current_window].window);
		
	int current_song_track = 0;
	
	my_song.find_visible_blocks();
	
	
	//------------ MAIN LOOP

	do
	{
		glfwWaitEventsTimeout(0.01);

		if (main_gui.windows[current_window].window_gained_focus)
		{
			main_gui.windows[current_window].draw_all = true; // WINDOW GAINS FOCUS DRAW EVERYTHING AGAIN
			main_gui.windows[current_window].window_gained_focus = false;
		}
		
		if (main_gui.windows[current_window].window_resized)
		{
			main_gui.windows[current_window].draw_all = true;  // WINDOW GAINS FOCUS DRAW EVERYTHING AGAIN AT NEW WINDOW DIMENSIONS
			main_gui.windows[current_window].window_resized = false;
		}
		
		if (main_gui.windows[current_window].draw_all)  // AN EVENT REQUIRING ALL OF THE WINDOW TO BE REDRAW HAS OCCURRED
		{
			main_gui.windows[current_window].draw_all = false;
			nvgBeginFrame(vg, main_gui.screen_width, main_gui.screen_height, main_gui.pxRatio);
			main_gui.display_all();
	
			for (int trk=0; trk<my_song.get_number_of_tracks(); trk++)
			{			
				my_song.tracks[trk].x = main_gui.windows[current_window].widgets[widget_grid_edit[trk]]->x;
				my_song.tracks[trk].y = main_gui.windows[current_window].widgets[widget_grid_edit[trk]]->y;
				my_song.tracks[trk].w = main_gui.windows[current_window].widgets[widget_grid_edit[trk]]->w;
				my_song.tracks[trk].h = main_gui.windows[current_window].widgets[widget_grid_edit[trk]]->h;
				my_song.draw_track_display(vg, trk);
				glfwSwapBuffers(main_gui.windows[current_window].window);
			}
			nvgEndFrame(vg);
			glfwSwapBuffers(main_gui.windows[current_window].window);
		}

		if (glfwGetTime() - old_time > 0.01)
		{		
			old_time = glfwGetTime();
			main_gui.main_loop();
						
			int current_widget = main_gui.windows[current_window].current_widget;
								
			if (main_gui.windows[current_window].widget_draw)
			{
	
				main_gui.windows[current_window].widget_draw = false;
				nvgEndFrame(vg);
				glfwSwapBuffers(main_gui.windows[current_window].window);
			}
			
			if (current_widget > -1)
			{
				if (main_gui.windows[current_window].widgets[current_widget]->type == widget_type_grid)
				{
					current_song_track = song_track_widget_number[current_widget];
						my_song.draw_track_display(vg, current_song_track);
						nvgEndFrame(vg);
						glfwSwapBuffers(main_gui.windows[current_window].window);
				}
			}
		}
		
	} while( glfwGetKey(main_gui.windows[current_window].window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& glfwWindowShouldClose(main_gui.windows[current_window].window) == 0 );
		
	 my_jack_manager.close_client("hertz"); 	
	 

	return 0;
	
}





