
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

//----------------------------------------------------------------------------------------------
void widget_knob::draw(NVGcontext* vg)
{


	nvgBeginPath(vg);
	nvgRect(vg, x,y-32,w*1.5,h+32);
	nvgFillPaint(vg, nvgRadialGradient(vg, screen_width[0]/2, screen_height[0]/2,600,1000, nvgRGBA(30,30,30,255),nvgRGBA(10,10,10,255))); 
	nvgFill(vg);
	
	nvgBeginPath(vg);

	int cx = x + (w/2);
	int cy = y + (w/2.3);

	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),5,30, nvgRGBA(100,100,120,255),nvgRGBA(30,30,30,255)));
		else nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),5,30, nvgRGBA(70,70,90,255),nvgRGBA(10,10,10,255)));
	nvgCircle(vg, cx,cy, w/3.14);
	nvgFill(vg);
	
	float cos_x, sin_y, cos_x2, sin_y2;
	
	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(100,100,100,255));

	if (value == 1)nvgStrokeColor(vg, nvgRGBA(255,255,255,255));

	for (float scale=-0.05; scale<1; scale+=0.05 )
	{
 		cos_x =  (h/2.5) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y =  (h/2.5) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		cos_x2 =  (h/2) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y2 =  (h/2) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.95-value && value !=0)
		{
			nvgStroke(vg);
			nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
			nvgBeginPath(vg);
		}
		
		nvgMoveTo(vg,  x + (w/2)  + cos_x, (h/2.2) + sin_y+y);
		nvgLineTo(vg,  x + (w/2) + cos_x2, (h/2.2) + sin_y2+y);
	} 
	nvgStroke(vg);
	

	
	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
	float scale = value+0.05;

	cos_x =  (h/15) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y =  (h/15) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));
	cos_x2 =  (h/3) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y2 =  (h/3) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));

	nvgMoveTo(vg,  x + (w/2) + cos_x, (h/2.2) + sin_y + y);
	nvgLineTo(vg,  x + (w/2) + cos_x2,  (h/2.2) + sin_y2 + y);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgFontSize(vg, 14.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	nvgText(vg, x, y-14, text_top.c_str(),NULL);
}






