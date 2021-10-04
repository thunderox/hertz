
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_switch::draw(NVGcontext* vg)
{


	nvgBeginPath(vg);
	nvgRect(vg, x,y-32,w*1,h+32);
	nvgFillPaint(vg, nvgRadialGradient(vg, screen_width[0]/2, screen_height[0]/2,600,1000, nvgRGBA(30,30,30,255),nvgRGBA(10,10,10,255))); 
	nvgFill(vg);
	
	nvgBeginPath(vg);

	int cx = x + (w/2);
	int cy = y + (w/2.3);
	
	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/6,20, nvgRGBA(180,180,180,255),nvgRGBA(10,10,10,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/6,20, nvgRGBA(130,130,130,255),nvgRGBA(10,10,10,255)));
		
	nvgCircle(vg, cx,cy, w/4);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgCircle(vg, cx,cy, w/7);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	if (value > 0.5) nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/50,9, nvgRGBA(255,0,0,255),nvgRGBA(0,0,0,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, cx,cy,w/50,9, nvgRGBA(80,30,30,255),nvgRGBA(0,0,0,255))); 
	nvgCircle(vg, cx,cy, w/9);
	nvgFill(vg);


	nvgBeginPath(vg);
	nvgFontSize(vg, 14.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x, y-14, text_top.c_str(),NULL);
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


