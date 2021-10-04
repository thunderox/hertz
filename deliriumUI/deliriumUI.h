
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
// #include <unistd.h>

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

class deliriumUI
{
	public:
	
	deliriumUI();
	~deliriumUI();
	int create_window(int, int, int, int, string);
	void set_current_window(int);
	void set_window_grid(int,int,int);
	
	int main_loop();
	bool mouse_over(int,int);
	void display_all();
	void update_widget(int,int);
	
	int create_widget(int,int,float,float,float,float,string);
	
	int screen_width, screen_height;
	string window_title;

	int current_window;
	
	struct delirium_window
	{
		GLFWwindow* window;
		NVGcontext* vg;
		bool draw_all;
		float gridx,gridy,snapx,snapy;
		vector <widget*> widgets;
		int current_widget;
	};
	
	vector <delirium_window> windows;
	
	private:
		

	int font;
	int winWidth, winHeight;
	int fbWidth, fbHeight;
	float pxRatio;
};

#endif








