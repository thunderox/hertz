
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int event_type_note = 1;

//-----------------------------------------------------------------------
typedef struct
{
	int top;
	int bottom;
} struct_time_signature;

//-----------------------------------------------------------------------
typedef struct
{
	int tempo;
	long time;
} tempo_event;

//-----------------------------------------------------------------------
typedef struct
{
	float volume;
	vector <tempo_event> tempo_events;
} track;

//-----------------------------------------------------------------------
typedef struct
{
	int event_type;
	int midi_channel;
	int midi_note;
	int midi_volume;
	long time;
	
} block_event;

//-----------------------------------------------------------------------
typedef struct
{
	float volume;
	float start;
	float length;
	vector <block_event> block_events;
} block;

//-------------------------------------------------------------------------------
class song

{
	public:
	song();
	~song();
	
	void set_name(string);
	string get_name();
	
	int sample_rate;
	int ppqn;
	int tempo;
	
	private:
	
	string name;
	struct_time_signature time_signature;

	
};


