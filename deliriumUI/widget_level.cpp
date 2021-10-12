
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_level::draw(NVGcontext* vg)
{

	int number_of_leds = 20;
	int draw_led_counter = number_of_leds;
	int y_inc = (float)h / number_of_leds;
	
	int level = (float)number_of_leds * value;
	
	for (int yl=0; yl<h; yl+=y_inc)
	{
		if (draw_led_counter == number_of_leds) // TOP LEVEL GRAD FADES FROM RED TO YELLOW
		{
			nvgFillPaint(vg, nvgLinearGradient(vg, x,y,x,y+(h/2), nvgRGBA(200,0,0,255),nvgRGBA(200,200,0,255))); 
		}
			
		if (draw_led_counter == number_of_leds / 2) // HALF WAY POINT CHANGE GRAD TO FADE FROM YELLOW TO GREEN
		{
			nvgFillPaint(vg, nvgLinearGradient(vg, x,y+(h/2),x,y+(h/2)+(h/2), nvgRGBA(200,200,0,255),nvgRGBA(0,200,0,255)));
		}
	
		draw_led_counter--;
		
		nvgBeginPath(vg);
		if (draw_led_counter < level && value != 0) nvgRect(vg, x,y+yl,w,h/(number_of_leds*2));
		nvgFill(vg);
	}
	
}







