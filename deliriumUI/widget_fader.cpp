
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_fader::draw(NVGcontext* vg)
{


	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,h);
	nvgFillPaint(vg, nvgLinearGradient(vg, x,y,x+w,y+h, nvgRGBA(80,80,80,255),nvgRGBA(30,30,30,255))); 
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(200,200,200,32));
	
	float top_y = 16;
	float bottom_y = 16;


	for (int ly=top_y+(h/20); ly<(h-bottom_y)-10; ly+=8)
	{

		nvgMoveTo(vg,  x+(w/2)-(w/3), y + ly);
		nvgLineTo(vg,  x+(w/2)+(w/3), y + ly);
	}
	
	nvgMoveTo(vg, x+(w/2), y+top_y);
	nvgLineTo(vg, x+(w/2), y+(h-bottom_y));
	nvgStroke(vg);


	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 255));
	nvgMoveTo(vg, 1+x + (w / 2), y + top_y);
	nvgLineTo(vg, 1+x + (w / 2), y + (h - bottom_y));
	nvgStroke(vg);
	
	float fade_h = h-64;
	float fade_y = (y+16) + (fade_h * (1-value));
	nvgBeginPath(vg);
	nvgFillPaint(vg, nvgLinearGradient(vg, x, fade_y, x, fade_y+32, nvgRGBA(100,100,100,255),nvgRGBA(10,10,10,255))); 
	nvgRect(vg, x, fade_y, w, 32);
	nvgFill(vg);	

	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(200, 200, 200, 128));
	nvgMoveTo(vg, x, fade_y+12);
	nvgLineTo(vg, x + w, fade_y + 12);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgFontSize(vg, 9.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	float text_width = 4.5f * (float)text_top.length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y+10, text_top.c_str(),NULL);

	nvgFontSize(vg, 12.0f);
	std::stringstream ss;
	float trunc_float = roundf(value * 100) / 100;
	ss << trunc_float;

	text_width = 6 * ss.str().length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y + (h-bottom_y)+10, ss.str().c_str(), NULL);
}


//----------------------------------------------------------------------------------------------
void widget_fader::drag(float mx, float my)
{
	value = 1 - ((1.0 / (h-64)) * ((my-24) - y));

	if (value < 0) value = 0;
	if (value > 1) value = 1;
}



