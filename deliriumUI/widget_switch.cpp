
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_switch::draw(NVGcontext* vg)
{
	// 
	
	nvgBeginPath(vg);

	int cx = x + (w/2);
	int cy = y + (w/1.8);
	
	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/6,20, nvgRGBA(180,180,180,255),nvgRGBA(10,10,10,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/6,20, nvgRGBA(130,130,130,255),nvgRGBA(10,10,10,255)));
		
	nvgCircle(vg, cx,cy, w/4);
	nvgFill(vg);
	
	// 

	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgCircle(vg, cx,cy, w/7);
	nvgFill(vg);


	// 
	
	nvgBeginPath(vg);
	if (value > 0.5) nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/50,9, nvgRGBA(255,0,0,255),nvgRGBA(0,0,0,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/50,9, nvgRGBA(80,30,30,255),nvgRGBA(0,0,0,255))); 
	nvgCircle(vg, cx,cy, w/9);
	nvgFill(vg);

	// DRAW LABEL TEXT
	
	nvgBeginPath(vg);
	nvgStrokeWidth(vg, 1);	
	float font_size = 10.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(220,220,220,220));
		else nvgFillColor(vg, nvgRGBA(180,180,180,255));
	float text_width = (font_size/1.75) * text_top.length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y, text_top.c_str(),NULL);
	
}


//----------------------------------------------------------------------------------------------
void widget_switch::left_button()
{
	value = 1 - value;
}


//----------------------------------------------------------------------------------------------
void widget_switch::value_inc()
{
	value = 0;
}

//----------------------------------------------------------------------------------------------
void widget_switch::value_dec()
{
	value = 1;
}


