
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cairo/cairo.h>

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
	string name;
	float volume;
} track;

const int block_event_type_note_on = 1;
const int block_event_type_note_off = 2;

//-----------------------------------------------------------------------
typedef struct
{
	int event_type;
	int note;
	int volume;
	long delta;
	
} block_event;

//-----------------------------------------------------------------------
typedef struct
{
	string name;
	float volume;
	long start;
	long length;
	vector <block_event> events;
} block;

//-------------------------------------------------------------------------------
class song

{
	public:
	song();
	~song();
	int create_track(string);
	int create_block(string,long,long);
	bool load_midi_file(string);
	
	void draw_track_display(cairo_t*);
	
	void set_name(string);
	string get_name();
	
	int sample_rate;
	int ppqn;
	int tempo;
	
	vector <track> tracks;
	vector <block> blocks;
	
	int window_width;
	int window_height;
	int track_scroll_y;

	
	enum EventName : uint8_t
	{					
		VoiceNoteOff = 0x80,
		VoiceNoteOn = 0x90,
		VoiceAftertouch = 0xA0,
		VoiceControlChange = 0xB0,
		VoiceProgramChange = 0xC0,
		VoiceChannelPressure = 0xD0,
		VoicePitchBend = 0xE0,
		SystemExclusive = 0xF0,		
	};

	enum MetaEventName : uint8_t
	{
		MetaSequence = 0x00,
		MetaText = 0x01,
		MetaCopyright = 0x02,
		MetaTrackName = 0x03,
		MetaInstrumentName = 0x04,
		MetaLyrics = 0x05,
		MetaMarker = 0x06,
		MetaCuePoint = 0x07,
		MetaChannelPrefix = 0x20,
		MetaEndOfTrack = 0x2F,
		MetaSetTempo = 0x51,
		MetaSMPTEOffset = 0x54,
		MetaTimeSignature = 0x58,
		MetaKeySignature = 0x59,
		MetaSequencerSpecific = 0x7F,
	};
	
	
	struct MidiEvent
	{
		enum class Type
		{
			NoteOff,
			NoteOn,
			Other
		} event;

		uint8_t nKey = 0;
		uint8_t nVelocity = 0;
		uint32_t nDeltaTick = 0;
	};


	struct MidiNote
	{	
		uint8_t nKey = 0;
		uint8_t nVelocity = 0;
		uint32_t nStartTime = 0;
		uint32_t nDuration = 0;
	};	

	struct MidiTrack
	{
		std::string sName;
		std::string sInstrument;
		std::vector<MidiEvent> vecEvents;
		std::vector<MidiNote> vecNotes;
		uint8_t nMaxNote = 64;
		uint8_t nMinNote = 64;
	};
	
	private:
	
	string name;
	struct_time_signature time_signature;

	
};


