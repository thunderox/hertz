
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
	if (hover) nvgFillPaint(vg, nvgLinearGradient(vg, x, fade_y, x, fade_y+32, nvgRGBA(130,130,130,255),nvgRGBA(20,20,20,255))); 
		else nvgFillPaint(vg, nvgLinearGradient(vg, x, fade_y, x, fade_y+32, nvgRGBA(100,100,100,255),nvgRGBA(10,10,10,255))); 
	nvgRect(vg, x, fade_y, w, 32);
	nvgFill(vg);	

	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(200, 200, 200, 128));
	nvgMoveTo(vg, x, fade_y+12);
	nvgLineTo(vg, x + w, fade_y + 12);
	nvgStroke(vg);

	nvgBeginPath(vg);
	float font_size = 10.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(180,180,180,255));
	float text_width = (font_size/1.75) * text_top.length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y, text_top.c_str(),NULL);

	std::stringstream ss;
	float trunc_float = roundf(value * 1000) / 1000;
	ss << trunc_float;

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);
	text_width = (font_size/1.75) * ss.str().length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y + h, ss.str().c_str(), NULL);
}


//----------------------------------------------------------------------------------------------
void widget_fader::drag(float mx, float my)
{
	value = 1 - ((1.0 / (h-64)) * ((my-24) - y));

	if (value < 0) value = 0;
	if (value > 1) value = 1;
}



