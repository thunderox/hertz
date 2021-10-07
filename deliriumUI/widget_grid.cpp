
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_grid::draw(NVGcontext* vg)
{
	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,h);
	nvgFillPaint(vg, nvgLinearGradient(vg, 0,0,0,screen_height[0], nvgRGBA(200,200,200,255),nvgRGBA(160,160,160,255))); 
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,120));
	
	for (int gy=0; gy<h; gy+=16)
	{
		nvgMoveTo(vg, x, y+gy);
		nvgLineTo(vg, x+w, y+gy);
	}
	
	for (int gx=0; gx<w; gx+=8)
	{
		nvgMoveTo(vg, x+gx, y);
		nvgLineTo(vg, x+gx, y+h);
	}
	
	nvgStroke(vg);


}






