
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_button::draw(NVGcontext* vg)
{
	nvgRect(vg, x,y,w,h);
	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),10,100, nvgRGBA(100,100,120,255),nvgRGBA(30,30,30,255))); 
		else nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),10,100, nvgRGBA(70,70,90,255),nvgRGBA(10,10,10,255)));
		
	
	nvgFill(vg);

	nvgFontSize(vg, 12.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	if (hover) nvgFillColor(vg, nvgRGBA(220,220,220,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x+11, y+14, text_top.c_str(),NULL);

}






