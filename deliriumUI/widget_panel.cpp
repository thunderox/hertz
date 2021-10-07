
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_panel::draw(NVGcontext* vg)
{
	nvgRect(vg, x,y,w,h);
	nvgFillPaint(vg, nvgLinearGradient(vg, 0,0,0, screen_height[0]/2, nvgRGBA(40,40,40,255),nvgRGBA(10,10,10,255))); 

	nvgFill(vg);

	float font_size = 10.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x+4 , y+4, text_top.c_str(),NULL);

}






