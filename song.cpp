
#include "song.h"

//--------------------------------------------------------------------------------------------------
// CONSTRUCTOR

song::song()
{
	tempo = 120;
	ppqn = 15360;
	time_signature.top = 4;
	time_signature.bottom = 4;
}
	
//--------------------------------------------------------------------------------------------------
// DECONSTRUCTOR

song::~song()
{
}

//--------------------------------------------------------------------------------------------------
// SET NAME OF SONG

void song::set_name(string _name)
{
	name = _name;
}

//--------------------------------------------------------------------------------------------------
// GET NAME OF SONG

string song::get_name()
{
	return name;
}

