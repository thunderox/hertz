
#ifndef DELIRIUM_UI_H
#define DELIRIUM_UI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "widget.h"
#include <stdio.h>
#include <stdlib.h>

#include "../nanovg/nanovg.h"
#include "../nanovg/nanovg_gl.h"
#include "../nanovg/nanovg_gl_utils.h"

using namespace glm;
using namespace std;

class Rectangle
{
	public:
	void setX(int);
	void setY(int);
	void setWidth(int);
	void setHeight(int);
	bool contains(int,int);
	
	private:
	
	int x,y,width,height;	
};

struct group
{
	string name;
	vector <string> members;
	int visible_member;
};

class deliriumUI
{
	public:
	
	deliriumUI();
	~deliriumUI();
	
	int create_window(int, int, int, int, string);
	void set_current_window(int);
	void set_window_grid(int,int,int);
	
	int create_widget(int,int,float,float,float,float,string);
	void set_widget_parent(int, int, int);
	void recalc_widget_dimensions(int);

	void display_all();
	void refresh_widgets(int);	
	void draw_widget(int, int);
	int main_loop();
	int mouse_over(int,int);
	
	int screen_width, screen_height;
	string window_title;

	int current_window;
	
	struct delirium_window
	{
		GLFWwindow* window;
		NVGcontext* vg;
		bool draw_all;
		bool widget_draw;
		float gridx,gridy,snapx,snapy;
		vector <widget*> widgets;
		int current_widget;
		bool window_gained_focus;
		bool window_resized;
	};
	
	vector <delirium_window> windows;
	
	float pxRatio;
	
	private:
		

	int font;
	int window_width, window_height;
	int fbWidth, fbHeight;

};

#endif








