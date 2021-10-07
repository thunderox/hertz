
#ifndef DELIRIUM_WIDGET_H
#define DELIRIUM_WIDGET_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "../nanovg/nanovg.h"

using namespace glm;
using namespace std;

typedef enum 
{
	widget_type_button = 1,
	widget_type_knob = 2,
	widget_type_switch = 3,
	widget_type_fader = 4,
	widget_type_panel = 5,
	widget_type_grid = 6
} widget_type;

class widget
{

	public:

	widget();
	~widget();
	virtual void draw(NVGcontext*);
	void set_position(float,float);
	void set_size(float,float);
	virtual void drag(float,float);
	virtual void set_value(float);
	void set_default_value(float);
	virtual void left_button();
	void middle_button();
	virtual void value_inc();
	virtual void value_dec();
	
	int* screen_width;
	int* screen_height;
	bool redraw;
	float x,y,w,h;
	bool hover;
	int type;
	float value;
	float default_value;
	float value_increment;
	float scaled_value;
	float value_min;
	float value_max;
	
	string text_top;
	string text_bottom;
	private:
	
	float old_mx, old_my;
};

class widget_button: public widget
{
	public:
	void draw(NVGcontext*);
};


class widget_knob: public widget
{
	public:
	void draw(NVGcontext*);
};

class widget_switch: public widget
{
	public:
	void draw(NVGcontext*);
	void left_button();
	void value_inc();
	void value_dec();
};

class widget_fader: public widget
{
	public:
	void draw(NVGcontext*);
	void drag(float, float);
};

class widget_panel: public widget
{
	public:
	void draw(NVGcontext*);
};

class widget_grid: public widget
{
	public:
	void draw(NVGcontext*);
};

#endif





