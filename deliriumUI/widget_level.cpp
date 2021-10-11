
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"



//----------------------------------------------------------------------------------------------
void widget_level::draw(NVGcontext* vg)
{

	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(255,255,255,255));
	nvgFill(vg);
	
	cout << "grog" << endl;
}







