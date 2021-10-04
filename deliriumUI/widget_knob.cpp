
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

//----------------------------------------------------------------------------------------------
void widget_knob::draw(NVGcontext* vg)
{


	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,h);
	nvgFillPaint(vg, nvgRadialGradient(vg, screen_width[0]/2, screen_height[0]/2,600,1000, nvgRGBA(20,20,20,255),nvgRGBA(5,5,5,255))); 
	nvgFill(vg);
	
	nvgBeginPath(vg);

	int cx = x + (w/2);
	int cy = y + (w/1.6);
	int ccf1 = (w+h)/8;
	int ccf2 = (w+h)/3;

	if (hover) nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),5,30, nvgRGBA(100,100,120,255),nvgRGBA(30,30,30,255)));
		else nvgFillPaint(vg, nvgRadialGradient(vg, x+(w/2),y+(h/2),5,30, nvgRGBA(70,70,90,255),nvgRGBA(10,10,10,255)));
	nvgCircle(vg, cx,cy, ccf1);
	nvgFill(vg);
	
	float cos_x, sin_y, cos_x2, sin_y2;
	ccf1 = (w+h)/7;
	ccf2 = (w+h)/5.5;
	
	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(100,100,100,255));

	if (value == 1)nvgStrokeColor(vg, nvgRGBA(255,255,255,255));

	for (float scale=-0.05; scale<1; scale+=0.05 )
	{
 		cos_x = ccf1 * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y = ccf1 * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		cos_x2 = ccf2 * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y2 = ccf2 * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.95-value && value !=0)
		{
			nvgStroke(vg);
			nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
			nvgBeginPath(vg);
		}
		
		nvgMoveTo(vg,  x + (w/2)  + cos_x,  sin_y+cy);
		nvgLineTo(vg,  x + (w/2) + cos_x2, sin_y2+cy);
	} 
	nvgStroke(vg);
	
	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(255,255,255,160));
	float scale = value+0.05;

	ccf1 = 0;
	ccf2 = (w+h)/8.5;

	cos_x =  ccf1 * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y =  ccf1 * (sin(((((scale)*0.75)-0.67)*2) * M_PI));
	cos_x2 =  ccf2 * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y2 =  ccf2 * (sin(((((scale)*0.75)-0.67)*2) * M_PI));

	nvgMoveTo(vg,  x + (w/2) + cos_x, sin_y + cy);
	nvgLineTo(vg,  x + (w/2) + cos_x2,  sin_y2 + cy);
	nvgStroke(vg);

	nvgBeginPath(vg);
	float font_size = 10.0f;
	nvgFontSize(vg, font_size);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	if (hover) nvgFillColor(vg, nvgRGBA(255,255,255,255));
		else nvgFillColor(vg, nvgRGBA(200,200,200,255));
	float text_width = (font_size/1.75) * text_top.length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y, text_top.c_str(),NULL);
	
	std::stringstream ss;
	float trunc_float = roundf(value * 1000) / 1000;
	ss << trunc_float;

	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);
	text_width = (font_size/1.75) * ss.str().length();
	nvgText(vg, x + ((w / 2) - (text_width / 2)), y + h, ss.str().c_str(), NULL);
	
}






