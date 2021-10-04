

#include <jack/jack.h>
#include <jack/midiport.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector> 

using namespace std;

int process(jack_nframes_t nframes, void *arg);
int cb_srate (jack_nframes_t, void *arg);

//--------------------------------------------------
typedef struct
{
	jack_port_t *input_port;
	string name;
} jack_in;

//--------------------------------------------------
typedef struct
{
	jack_port_t *input_port;
	jack_port_t *audio_in_left;
	jack_port_t *audio_in_right;
	
	jack_port_t *output_port;
	jack_port_t *audio_out_left;
	jack_port_t *audio_out_right;
	string name;
} track_port;



//--------------------------------------------------
typedef struct
{
	jack_client_t *client;
	string name;
} jack_client;
	
//--------------------------------------------------
class jack_manager
{

	public:
	
		jack_manager();
		~jack_manager();
		void set_current_manager(jack_manager*);
		
		bool create_client(string);
		void close_client(string);
		int create_jack_out(string,string);
		int get_client_number(string);
		void connect_audio(string, string);
		int sample_rate;
		
		vector<track_port> track_ports;
		vector<jack_client> jack_clients;
	
	private:
	


};

//--------------------------------------------------






