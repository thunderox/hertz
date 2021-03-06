
#define NANOVG_GL3_IMPLEMENTATION
#include "widget.h"


//----------------------------------------------------------------------------------------------

widget::widget()
{
	value = 0;
	default_value = 0;
	value_increment = 0.05;
	old_mx = -1;
	old_my = -1;
	redraw = false;
	x=0; y=0; w=0; h=0;
	parent_x=0; parent_y=0; parent_w=0; parent_h=0;
	parent = -1; // THIS IS A STRONG SELF RELIANT INDEPENDENT WIDGET - WELL BY DEFAULT ANYWAY 
}

//----------------------------------------------------------------------------------------------
widget::~widget()
{
}

//----------------------------------------------------------------------------------------------
void widget::set_position(float _x, float _y)
{

}

//----------------------------------------------------------------------------------------------
void widget::set_size(float _w, float _h)
{

}

//----------------------------------------------------------------------------------------------
void widget::draw(NVGcontext* vg)
{


}


//----------------------------------------------------------------------------------------------
void widget::drag(float mx,float my)
{
	value = 1 - ((1.0 / h) * (my - y));	
	
	if (value < 0) value = 0;
	if (value > 1) value = 1;
}

//----------------------------------------------------------------------------------------------
void widget::value_inc()
{
	value -= value_increment;
	if (value < 0) value = 0;
}

//----------------------------------------------------------------------------------------------
void widget::value_dec()
{
	value += value_increment;
	if (value > 1) value = 1;
}

//----------------------------------------------------------------------------------------------
void widget::left_button()
{

}

//----------------------------------------------------------------------------------------------
void widget::middle_button()
{
	value = default_value;
	
}

//----------------------------------------------------------------------------------------------
void widget::set_value(float _value )
{
	if (_value < 0) _value = 0;
	if (_value > 1) _value = 1;
	value = _value;
}


//----------------------------------------------------------------------------------------------
void widget::set_default_value(float _value )
{
	if (_value < 0) _value = 0;
	if (_value > 1) _value = 1;
	default_value = _value;
}


 	





