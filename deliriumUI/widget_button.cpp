
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_button::draw(NVGcontext* vg)
{
	nvgRect(vg, x,y,w,h);
	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),10,100, nvgRGBA(100,100,120,255),nvgRGBA(30,30,30,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),10,100, nvgRGBA(70,70,90,255),nvgRGBA(10,10,10,255)));
		
	
	nvgFill(vg);

	float font_size = 10.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x+4 , y+4, text_top.c_str(),NULL);

}






