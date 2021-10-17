
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_panel::draw(NVGcontext* vg)
{
	nvgRect(vg, x,y,w,h);
	nvgFillPaint(vg, nvgLinearGradient(vg, 0,0,0, screen_height[0]/2, nvgRGBA(40,40,40,255),nvgRGBA(10,10,10,255))); 
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,24);
	nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),10,100, nvgRGBA(80,80,120,255),nvgRGBA(40,40,50,255))); 
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgMoveTo(vg, x,y+24);
	nvgLineTo(vg, x+w,y+24);
	nvgStroke(vg);

	float font_size = 14.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x+6 , y+6, text_top.c_str(),NULL);

}






