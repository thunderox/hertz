
#include "song.h"
#define NANOVG_GL3_IMPLEMENTATION

//--------------------------------------------------------------------------------------------------
// CONSTRUCTOR

song::song()
{
	bpm = 120;
	ppqn = 15360;
	time_signature.top = 4;
	time_signature.bottom = 4;
	play_head = 0;
}
	
//--------------------------------------------------------------------------------------------------
// DECONSTRUCTOR

song::~song()
{
}


//===============================================================================================================================
// SONG FUNCTIONS
//===============================================================================================================================



//--------------------------------------------------------------------------------------------------
// SET NAME OF SONG

void song::set_name(string _name)
{
	name = _name;
	track_scroll_y = 0;
}

//--------------------------------------------------------------------------------------------------
// GET NAME OF SONG

string song::get_name()
{
	return name;
}




//==============================================================================================================================
// TRACK FUNCTIONS
//==============================================================================================================================



//---------------------------------------------------------------------------------------
// CREATE NEW TRACK

int song::create_track(string name)
{
	track new_track;
	new_track.name = name;
	tracks.push_back(new_track);
	
	return tracks.size()-1;
}


//----------------------------------------------------------------------------------------
// GET TRACK NAME

string song::get_track_name(int track_number)
{
	if (track_number > -1 && track_number < tracks.size() ) 
	{
		return tracks[track_number].name; 
	}
	return NULL;
}

//----------------------------------------------------------------------------------------
// GET NUMBER OF TRACKS

int song::get_number_of_tracks()
{
	return tracks.size();
}




//----------------------------------------------------------------------------------------
// DRAW TRACK DISPLAY

void song::draw_track_display(NVGcontext* vg, int track_number)
{
	if (track_number < 0 || track_number > tracks.size()) return;

	nvgStrokeWidth(vg, 1);	

	int x = tracks[track_number].x;
	int y = tracks[track_number].y;
	int w = tracks[track_number].w;
	int h = tracks[track_number].h;

	nvgScissor(vg, x, y, w, h);

	nvgBeginPath(vg);
	nvgFillPaint(vg, nvgLinearGradient(vg, x,y,x+w,y, nvgRGBA(50,50,80,255),nvgRGBA(30,30,50,255))); 
	nvgRect(vg, x,y,w,h);	
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,120));
	
	int quarter_2_pixels = (bpm / 8);	
	
	int delta = 0;
	
	int note_on_delta[128];
	for (int x=0; x<128; x++)
	{
		note_on_delta[x] = -1;
	}
	
	float zoom_y = 8;
	
	nvgFillColor(vg, nvgRGBA(200,200,200,255));
	
	for (int blkin = 0; blkin < tracks[track_number].block_instances.size(); blkin++)
	{
		int block_number = tracks[track_number].block_instances[blkin].block_number;
		
		int delta_start = blocks[block_number].events[0].delta /zoom_y;
		int delta_end = 0;

		nvgBeginPath(vg);
		nvgFillColor(vg, nvgRGBA(200,200,200,255));	
		for (int ev=0; ev<blocks[block_number].events.size(); ev++)
		{	
			int event_type = blocks[block_number].events[ev].event_type;
			int note = blocks[block_number].events[ev].note;
			delta += blocks[block_number].events[ev].delta;
			delta_end = delta / zoom_y;
			if (delta / zoom_y > h) break;	
				
			if (note > -1 && note < 128)
			{	
			
				if (event_type == block_event_type_note_on)
				{
					note_on_delta[note] = delta;
				}
					
				if (event_type == block_event_type_note_off && note_on_delta[note] != -1)
				{
					float note_x_pos = x + ((float)note * ((float)w / 128.0f));
					int note_y_pos = y + (note_on_delta[note] / zoom_y);
					int note_height = (delta / zoom_y)  - (note_on_delta[note]/zoom_y);
					nvgBeginPath(vg);
					nvgRect(vg, note_x_pos, note_y_pos, ((float)w / 128.0f), note_height);
					nvgFill(vg);
					note_on_delta[note] = -1;
					
				}
			}
		}
			
		if (y + (delta/zoom_y) < y+h)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, nvgRGBA(255,255,255,32));
			nvgRect(vg, x, y + (delta_start/zoom_y), w, delta_end - delta_start);
			nvgFill(vg);
			
			nvgBeginPath(vg);
			nvgFillColor(vg, nvgRGBA(150,150,150,255));
			nvgRect(vg, x, y + delta_start, w, 2);
			nvgFill(vg);
			
			nvgBeginPath(vg);
			nvgFillColor(vg, nvgRGBA(150,150,150,255));
			nvgRect(vg, x, y + delta_end, w, 2);
			nvgFill(vg);
		}

	}
	
	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	for (int gy=0; gy<h; gy+=quarter_2_pixels)
	{
		nvgMoveTo(vg, x, y+gy);
		nvgLineTo(vg, x+w, y+gy);
	}
	
	float octave_width = (float)w / (128.0 / 12.0);
	
	for (float gx=0; gx<w; gx+=octave_width)
	{
		nvgMoveTo(vg, x+gx, y);
		nvgLineTo(vg, x+gx, y+h);
	}
	
	nvgStroke(vg);

	nvgScissor(vg, 0, 0, screen_width, screen_height);
}



//=================================================================================================================================
// BLOCK FUNCTIONS
//==================================================================================================================================


//---------------------------------------------------------------------------------------
// CREATE NEW BLOCK
int song::create_block(string name,long start, long length)
{
	block new_block;
	new_block.name = name;
	new_block.volume = 0.75;
	new_block.start = start;
	new_block.length = length;
	blocks.push_back(new_block);
	return blocks.size()-1;
}


//-----------------------------------------------------------------------------------------
// CREATE INSTANCE OF BLOCK

bool song::create_block_instance(string name, int block_number, int track_number, int delta_time, bool visible)
{

	
	if (track_number < 0 || track_number > tracks.size() ) return false;
	if (block_number < 0 || block_number > blocks.size() ) return false;

	
	block_instance new_block_instance;
	new_block_instance.name = name;
	new_block_instance.block_number = block_number;
	new_block_instance.delta_time = delta_time;
	new_block_instance.visible = visible;
	tracks[track_number].block_instances.push_back(new_block_instance);
	return true;
}

//----------------------------------------------------------------------------------------

void song::find_visible_blocks()
{
	for (int trk=0; trk < tracks.size(); trk++)
	{
	 	for (int blkin = 0; blkin < tracks[trk].block_instances.size(); blkin++)
		{
			tracks[trk].block_instances[blkin].visible = false;
			if (play_head == tracks[trk].block_instances[blkin].delta_time)
			{
				tracks[trk].block_instances[blkin].visible = true;
			}
		}
	}}


//=================================================================================================================================
// MIDI FUNCTIONS
//==================================================================================================================================




//--------------------------------------------------------------------------------------------------
// LOAD AND PARSE MIDI FILE

bool song::load_midi_file(string file_name)
{
	// Open the MIDI File as a stream
	std::ifstream ifs;
	ifs.open(file_name, std::fstream::in | std::ios::binary);
	
	if (!ifs.is_open()) return false;
	
	// Swaps byte order of 32-bit integer
	auto Swap32 = [](uint32_t n)
	{
		return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
	};
	
	// Swaps byte order of 16-bit integer
	auto Swap16 = [](uint16_t n)
	{
		return ((n >> 8) | (n << 8));
	};

	// Reads nLength bytes form file stream, and constructs a text string
	auto ReadString = [&ifs](uint32_t nLength)
	{
		std::string s;
		for (uint32_t i = 0; i < nLength; i++) s += ifs.get();
		return s;
	};

	// Reads a compressed MIDI value. This can be up to 32 bits long. Essentially if the first byte, first
	// bit is set to 1, that indicates that the next byte is required to construct the full word. Only
	// the bottom 7 bits of each byte are used to construct the final word value. Each successive byte 
	// that has MSB set, indicates a further byte needs to be read.
	auto ReadValue = [&ifs]()
	{
		uint32_t nValue = 0;
		uint8_t nByte = 0;
		// Read byte
		nValue = ifs.get();

		// Check MSB, if set, more bytes need reading
		if (nValue & 0x80)
		{
			// Extract bottom 7 bits of read byte
			nValue &= 0x7F;
			do
			{
				// Read next byte
				nByte = ifs.get();

				// Construct value by setting bottom 7 bits, then shifting 7 bits
				nValue = (nValue << 7) | (nByte & 0x7F);
			} 
			while (nByte & 0x80); // Loop whilst read byte MSB is 1
		}

		// Return final construction (always 32-bit unsigned integer internally)
		return nValue;
	};
	
	uint32_t n32 = 0;
	uint16_t n16 = 0;
	
	std::vector<MidiTrack> vecTracks;
	uint32_t m_nTempo = 0;
	uint32_t m_nBPM = 0;

	// Read MIDI Header (Fixed Size)
	ifs.read((char*)&n32, sizeof(uint32_t));
	uint32_t nFileID = Swap32(n32);
	ifs.read((char*)&n32, sizeof(uint32_t));
	uint32_t nHeaderLength = Swap32(n32);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nFormat = Swap16(n16);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nTrackChunks = Swap16(n16);
	ifs.read((char*)&n16, sizeof(uint16_t));
	uint16_t nDivision = Swap16(n16);
	
	int current_block = -1;
	int current_track = -1;
	
	for (uint16_t nChunk = 0; nChunk < nTrackChunks; nChunk++)
	{			
		// Read Track Header
		ifs.read((char*)&n32, sizeof(uint32_t));
		uint32_t nTrackID = Swap32(n32);
		ifs.read((char*)&n32, sizeof(uint32_t));
		uint32_t nTrackLength = Swap32(n32);

		bool bEndOfTrack = false;

		vecTracks.push_back(MidiTrack());

		uint32_t nWallTime = 0;

		uint8_t nPreviousStatus = 0;

		while (!ifs.eof() && !bEndOfTrack)
		{
			// Fundamentally all MIDI Events contain a timecode, and a status byte*
			uint32_t nStatusTimeDelta = 0;
			uint8_t nStatus = 0;


			// Read Timecode from MIDI stream. This could be variable in length
			// and is the delta in "ticks" from the previous event. Of course this value
			// could be 0 if two events happen simultaneously.
			nStatusTimeDelta = ReadValue();

			// Read first byte of message, this could be the status byte, or it could not...
			nStatus = ifs.get();

			// All MIDI Status events have the MSB set. The data within a standard MIDI event
			// does not. A crude yet utilised form of compression is to omit sending status
			// bytes if the following sequence of events all refer to the same MIDI Status.
			// This is called MIDI Running Status, and is essential to succesful decoding of
			// MIDI streams and files.
			//
			// If the MSB of the read byte was not set, and on the whole we were expecting a
			// status byte, then Running Status is in effect, so we refer to the previous 
			// confirmed status byte.
			if (nStatus < 0x80)
			{
				// MIDI Running Status is happening, so refer to previous valid MIDI Status byte
				nStatus = nPreviousStatus;

				// We had to read the byte to assess if MIDI Running Status is in effect. But!
				// that read removed the byte form the stream, and that will desync all of the 
				// following code because normally we would have read a status byte, but instead
				// we have read the data contained within a MIDI message. The simple solution is 
				// to put the byte back :P
				ifs.seekg(-1, std::ios_base::cur);
			}

				

			if ((nStatus & 0xF0) == EventName::VoiceNoteOff)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nNoteID = ifs.get();
				uint8_t nNoteVelocity = ifs.get();
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOff, nNoteID, nNoteVelocity, nStatusTimeDelta });
				block_event new_block_event;
				new_block_event.event_type = block_event_type_note_off;
				new_block_event.note = nNoteID;
				new_block_event.volume =  nNoteVelocity;
				new_block_event.delta = nStatusTimeDelta;
				blocks[current_block].events.push_back(new_block_event);
			}

			else if ((nStatus & 0xF0) == EventName::VoiceNoteOn)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nNoteID = ifs.get();
				uint8_t nNoteVelocity = ifs.get();
				if(nNoteVelocity == 0)
				{
					vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOff, nNoteID, nNoteVelocity, nStatusTimeDelta });
					block_event new_block_event;
					new_block_event.event_type = block_event_type_note_off;
					new_block_event.note = nNoteID;
					new_block_event.volume =  nNoteVelocity;
					new_block_event.delta = nStatusTimeDelta;
					blocks[current_block].events.push_back(new_block_event);
				}
				else
				{
					vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOn, nNoteID, nNoteVelocity, nStatusTimeDelta });
					block_event new_block_event;
					new_block_event.event_type = block_event_type_note_on;
					new_block_event.note = nNoteID;
					new_block_event.volume =  nNoteVelocity;
					new_block_event.delta = nStatusTimeDelta;
					blocks[current_block].events.push_back(new_block_event);
					
				}
			}

			else if ((nStatus & 0xF0) == EventName::VoiceAftertouch)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nNoteID = ifs.get();
				uint8_t nNoteVelocity = ifs.get();
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
			}

			else if ((nStatus & 0xF0) == EventName::VoiceControlChange)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nControlID = ifs.get();
				uint8_t nControlValue = ifs.get();
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
			}

			else if ((nStatus & 0xF0) == EventName::VoiceProgramChange)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nProgramID = ifs.get();					
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
			}

			else if ((nStatus & 0xF0) == EventName::VoiceChannelPressure)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nChannelPressure = ifs.get();
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
			}

			else if ((nStatus & 0xF0) == EventName::VoicePitchBend)
			{
				nPreviousStatus = nStatus;
				uint8_t nChannel = nStatus & 0x0F;
				uint8_t nLS7B = ifs.get();
				uint8_t nMS7B = ifs.get();
				vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
			}
			else if ((nStatus & 0xF0) == EventName::SystemExclusive)
			{
				nPreviousStatus = 0;

				if (nStatus == 0xFF)
				{
					// Meta Message
					uint8_t nType = ifs.get();
					uint8_t nLength = ReadValue();

					switch (nType)
					{
					case MetaSequence:
						std::cout << "Sequence Number: " << ifs.get() << ifs.get() << std::endl;
						break;
					case MetaText:
						std::cout << "Text: " << ReadString(nLength) << std::endl;
						break;
					case MetaCopyright:
						std::cout << "Copyright: " << ReadString(nLength) << std::endl;
						break;
					case MetaTrackName:
						vecTracks[nChunk].sName = ReadString(nLength);
						std::cout << "Track Name: " << vecTracks[nChunk].sName << std::endl;	
						current_track = create_track(vecTracks[nChunk].sName);
						current_block = create_block(vecTracks[nChunk].sName,0,8192);
						create_block_instance(vecTracks[nChunk].sName, current_block, current_track,0,false);
						break;
						
					case MetaInstrumentName:
						vecTracks[nChunk].sInstrument = ReadString(nLength);
						std::cout << "Instrument Name: " << vecTracks[nChunk].sInstrument << std::endl;
						break;
					case MetaLyrics:
						std::cout << "Lyrics: " << ReadString(nLength) << std::endl;
						break;
					case MetaMarker:
						std::cout << "Marker: " << ReadString(nLength) << std::endl;
						break;
					case MetaCuePoint:
						std::cout << "Cue: " << ReadString(nLength) << std::endl;
						break;
					case MetaChannelPrefix:
						std::cout << "Prefix: " << ifs.get() << std::endl;
						break;
					case MetaEndOfTrack:
						bEndOfTrack = true;
						break;
					case MetaSetTempo:
						// Tempo is in microseconds per quarter note	
						if (m_nTempo == 0)
						{
							(m_nTempo |= (ifs.get() << 16));
							(m_nTempo |= (ifs.get() << 8));
							(m_nTempo |= (ifs.get() << 0));
							m_nBPM = (60000000 / m_nTempo);
							bpm = m_nBPM;
							std::cout << "Tempo: " << m_nTempo << " (" << m_nBPM << "bpm)" << std::endl;
						}
						break;
					case MetaSMPTEOffset:
						std::cout << "SMPTE: H:" << ifs.get() << " M:" << ifs.get() << " S:" << ifs.get() << " FR:" << ifs.get() << " FF:" << ifs.get() << std::endl;
						break;
					case MetaTimeSignature:
						time_signature.top = ifs.get();
						time_signature.bottom = 2 << ifs.get();
						std::cout << "Time Signature: " << time_signature.top << "/" << time_signature.bottom << std::endl;
						std::cout << "ClocksPerTick: " << ifs.get() << std::endl;

						// A MIDI "Beat" is 24 ticks, so specify how many 32nd notes constitute a beat
						std::cout << "32per24Clocks: " << ifs.get() << std::endl;
						break;
					case MetaKeySignature:
						std::cout << "Key Signature: " << ifs.get() << std::endl;
						std::cout << "Minor Key: " << ifs.get() << std::endl;
						break;
					case MetaSequencerSpecific:
						std::cout << "Sequencer Specific: " << ReadString(nLength) << std::endl;
						break;
					default:
						std::cout << "Unrecognised MetaEvent: " << nType << std::endl;
					}
				}

				if (nStatus == 0xF0)
				{
					// System Exclusive Message Begin
					std::cout << "System Exclusive Begin: " << ReadString(ReadValue())  << std::endl;
				}

				if (nStatus == 0xF7)
				{
					// System Exclusive Message Begin
					std::cout << "System Exclusive End: " << ReadString(ReadValue()) << std::endl;
				}
			}			
			else
			{
				std::cout << "Unrecognised Status Byte: " << nStatus << std::endl;
			}
		}
	}


	// Convert Time Events to Notes
	for (auto& track : vecTracks)
	{
		uint32_t nWallTime = 0;

		std::list<MidiNote> listNotesBeingProcessed;

		for (auto& event : track.vecEvents)
		{
			nWallTime += event.nDeltaTick;

			if (event.event == MidiEvent::Type::NoteOn)
			{
				// New Note
				listNotesBeingProcessed.push_back({ event.nKey, event.nVelocity, nWallTime, 0 });
			}

			if (event.event == MidiEvent::Type::NoteOff)
			{
				auto note = std::find_if(listNotesBeingProcessed.begin(), listNotesBeingProcessed.end(), [&](const MidiNote& n) { return n.nKey == event.nKey; });
				if (note != listNotesBeingProcessed.end())
				{
					note->nDuration = nWallTime - note->nStartTime;
					track.vecNotes.push_back(*note);
					track.nMinNote = std::min(track.nMinNote, note->nKey);
					track.nMaxNote = std::max(track.nMaxNote, note->nKey);
					listNotesBeingProcessed.erase(note);
				}
			}
		}

	}	
	return true;
}










