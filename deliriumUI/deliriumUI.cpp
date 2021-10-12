

#define NANOVG_GL3_IMPLEMENTATION
#include "deliriumUI.h"

bool mouse_left_button = false;
bool mouse_left_released = false;
bool mouse_middle_button_pressed = false;
bool mouse_middle_button_released = false;
float mouse_scroll_y = 0;

//----------------------------------------------------------------------------------------------

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) mouse_left_button = true;
    	else mouse_left_button = false;
    	
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == 0) mouse_middle_button_pressed = true;
    
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) mouse_middle_button_pressed = true;
    	else mouse_middle_button_pressed = false;
    	
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == 0) mouse_middle_button_released = true;
}

//----------------------------------------------------------------------------------------------

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouse_scroll_y = yoffset;
}


//----------------------------------------------------------------------------------------------

deliriumUI::deliriumUI()
{
	if (!glfwInit())
	{	
 		// Initialization failed
 		cout << "GLFW initialisation failed" << endl;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	screen_width = mode->width;
	screen_height = mode->height;
	
	current_window = -1;
	
}


//----------------------------------------------------------------------------------------------

deliriumUI::~deliriumUI()
{
	for (int x=0; x<windows.size(); x++)
		glfwDestroyWindow(windows[x].window);

	glfwTerminate();
}


//----------------------------------------------------------------------------------------------

int deliriumUI::create_window(int x, int y, int width, int height, string title)
{
	GLFWwindow* glfw_win = glfwCreateWindow(width,height, title.c_str(), NULL, NULL);
	delirium_window new_window;
	new_window.window = glfw_win;
	glfwSetWindowPos(new_window.window, x, y);
	
	glfwMakeContextCurrent( glfw_win ); 
	
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}
	
	new_window.vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	
	if (new_window.vg == NULL) return EXIT_FAILURE;
	
	font = nvgCreateFont(new_window.vg, "sans", "deliriumUI/mono.ttf");
	if (font == -1) cout << "Could not load font" << endl;	
	
	new_window.current_widget = -1;
	new_window.draw_all = true;
	
	windows.push_back(new_window);
	return windows.size()-1;
}

//---------------------------------------------------------------------------------------------

void deliriumUI::set_current_window(int _current_window)
{
	current_window = _current_window;

	if (current_window > -1 && current_window < windows.size())
	{
		GLFWwindow* window = windows[current_window].window;
	
		glfwMakeContextCurrent( window ); 
		glfwGetWindowSize(window, &winWidth, &winHeight);
		fbWidth=winWidth; fbHeight=winHeight;
		pxRatio = (float)fbWidth / (float)winWidth;
		// set up view
		glViewport( 0, 0, winWidth, winHeight);
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glfwSwapInterval( 0.5 );
	}
}

//----------------------------------------------------------------------------------------------

void deliriumUI::set_window_grid(int win, int gridx, int gridy)
{
	if (current_window > -1 && current_window < windows.size())
	{
		windows[win].gridx = gridx;
		windows[win].gridy = gridy;
	
		windows[win].snapx = screen_width / gridx;
		windows[win].snapy = screen_height / gridy;
	}
}

//----------------------------------------------------------------------------------------------

int deliriumUI::main_loop()
{
	double mx,my;
	
	if (current_window > -1 && current_window < windows.size())
	{
		GLFWwindow* window = windows[current_window].window;
		NVGcontext* vg = windows[current_window].vg; 
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
			
			glfwWaitEventsTimeout(0.25);
			

				glfwGetCursorPos(window, &mx, &my);

				int current_widget = windows[current_window].current_widget;
				mouse_left_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			
				if (current_widget > -1 && windows[current_window].widgets[current_widget]->hover)
				{
					if ( mouse_left_released && windows[current_window].widgets[current_widget]->type != widget_type_switch)
						mouse_left_released = false;
				
					if ( mouse_left_released && windows[current_window].widgets[current_widget]->type == widget_type_switch)
					{
						windows[current_window].widgets[current_widget]->left_button();
						windows[current_window].widgets[current_widget]->redraw = true;
						mouse_left_released = false;
					}
				
					if (mouse_left_button)
					{				
						if (windows[current_window].widgets[current_widget]->type != widget_type_switch)
						{
							windows[current_window].widgets[current_widget]->drag(mx, my);
							windows[current_window].widgets[current_widget]->redraw = true;
						}
					}
					
					if (mouse_scroll_y == -1) { windows[current_window].widgets[current_widget]->value_inc(); mouse_scroll_y = 0; }
					if (mouse_scroll_y == 1) { windows[current_window].widgets[current_widget]->value_dec(); mouse_scroll_y = 0; }
					
					if (mouse_middle_button_released)
					{
						mouse_middle_button_released = false;
						windows[current_window].widgets[current_widget]->set_value
							(windows[current_window].widgets[current_widget]->default_value);
						windows[current_window].widgets[current_widget]->redraw = true;
					}
				}
				
				if (!mouse_left_button) mouse_over(mx,my);
			
			refresh_widgets(current_window);
		
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
int deliriumUI::mouse_over(int mx, int my)
{

	Rectangle test_rect;
	windows[current_window].current_widget = -1;

	for (int x=0; x<windows[current_window].widgets.size(); x++)
	{
		test_rect.setX(windows[current_window].widgets[x]->x );
		test_rect.setY(windows[current_window].widgets[x]->y );
		test_rect.setWidth(windows[current_window].widgets[x]->w );
		test_rect.setHeight(windows[current_window].widgets[x]->h );

		if (test_rect.contains(mx,my) && windows[current_window].widgets[x]->type != widget_type_panel)
		{
			windows[current_window].current_widget = x;
			windows[current_window].widgets[x]->hover = true;
			windows[current_window].widgets[x]->redraw = true;
			
		}
		if (!test_rect.contains(mx,my) && windows[current_window].widgets[x]->hover)
		{
			windows[current_window].widgets[x]->hover = false;
			windows[current_window].widgets[x]->redraw = true;
		}
	}

	return windows[current_window].current_widget;
}

//----------------------------------------------------------------------------------------------

void deliriumUI::display_all()
{
	if (current_window > -1 && current_window < windows.size())
	{
		NVGcontext* vg = windows[current_window].vg;
		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
		nvgBeginPath(vg);
		nvgRect(vg, 0,0,screen_width,screen_height);
		nvgFillPaint(vg, nvgRadialGradient(vg, screen_width/2, screen_height/2,600,1000, nvgRGBA(20,20,20,255),nvgRGBA(5,5,5,255))); 
		nvgFill(vg);

		for (int x=0; x<windows[current_window].widgets.size(); x++)
		{
			nvgBeginPath(vg);
			nvgScissor(vg, windows[current_window].widgets[x]->x, windows[current_window].widgets[x]->y,
				windows[current_window].widgets[x]->w, windows[current_window].widgets[x]->h);
				
			windows[current_window].widgets[x]->draw(vg);
						
			nvgScissor(vg, 0, 0, screen_width, screen_height);
		}
		nvgEndFrame(vg);
	}
	
}

//-----------------------------------------------------------------------------------------------


void deliriumUI::refresh_widgets(int window)
{
	windows[window].widget_draw = false;

	if (current_window > -1 && current_window < windows.size())
	{
		NVGcontext* vg = windows[current_window].vg;
		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		nvgBeginPath(vg);
		nvgFillPaint(vg, nvgLinearGradient(vg, 0,0,0, screen_height/2, nvgRGBA(40,40,40,255),nvgRGBA(10,10,10,255)));
		for (int x=0; x<windows[window].widgets.size(); x++)
		{
			if (windows[current_window].widgets[x]->redraw && windows[current_window].widgets[x]->type != widget_type_grid)
			{
				nvgRect(vg, windows[current_window].widgets[x]->x, windows[current_window].widgets[x]->y,
					windows[current_window].widgets[x]->w, windows[current_window].widgets[x]->h);
					windows[window].widget_draw = true; 
			}
			if (windows[window].widget_draw) nvgFill(vg);
		}

		for (int x=0; x<windows[window].widgets.size(); x++)
		{
			if (windows[current_window].widgets[x]->redraw)
			{
				windows[current_window].widgets[x]->redraw = false;
				nvgBeginPath(vg);
				windows[current_window].widgets[x]->draw(vg);
			}			
		}
	nvgEndFrame(vg);
	}
	
}

//-----------------------------------------------------------------------------------------------

int deliriumUI::create_widget(int type, int win, float x, float y, float w, float h, string text_top)
{

	widget* new_widget = NULL;
	bool new_widget_created = false;
	
	if (type == widget_type_button)
	{
		new_widget = new widget_button();
		new_widget->type = widget_type_button;
		new_widget_created = true;
	}
	
	if (type == widget_type_knob)
	{
		new_widget = new widget_knob();
		new_widget->type = widget_type_knob;
		new_widget_created = true;
	}
	
	if (type == widget_type_switch)
	{
		new_widget = new widget_switch();
		new_widget->type = widget_type_switch;
		new_widget_created = true;
	}
	
	if (type == widget_type_fader)
	{
		new_widget = new widget_fader();
		new_widget->type = widget_type_fader;
		new_widget_created = true;
	}
	
	if (type == widget_type_panel)
	{
		new_widget = new widget_panel();
		new_widget->type = widget_type_panel;
		new_widget_created = true;
	}
	
	if (type == widget_type_grid)
	{
		new_widget = new widget_grid();
		new_widget->type = widget_type_grid;
		new_widget_created = true;
	}
	
	if (type == widget_type_level)
	{
		new_widget = new widget_level();
		new_widget->type = widget_type_level;
		new_widget_created = true;
	}
	
	if (new_widget_created)
	{
		new_widget->x = x * windows[win].snapx;
		new_widget->y = y * windows[win].snapy;
		new_widget->w = (w * windows[win].snapx)-1;
		new_widget->h = (h * windows[win].snapy)-1;
		new_widget->screen_width = &screen_width;
		new_widget->screen_height = &screen_height;
		new_widget->text_top = text_top;
		windows[win].widgets.push_back(new_widget);
		
		return windows[win].widgets.size()-1;
	}
	
	return -1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------

	void Rectangle::setX(int _x)
	{
		x = _x;
	}
	
	void Rectangle::setY(int _y)
	{
		y = _y;
	}
	
	void Rectangle::setWidth(int _width)
	{
		width = _width;
	}
	
	void Rectangle::setHeight(int _height)
	{
		height = _height;
	}
	
	bool Rectangle::contains(int cx,int cy)
	{
		if ( cx >= x && cy >= y && cx <= x+width && cy <= y+height)
			return true;
			else return false;
	}




