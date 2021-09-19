

#include <jack/jack.h>
#include <jack/midiport.h>
#include <iostream>
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
	jack_port_t *output_port;
	jack_port_t *audio_out_left;
	jack_port_t *audio_out_right;
	string name;
} jack_out;

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
		
		bool create_client(string);
		void close_client(string);
		int create_jack_out(string,string);
		int get_client_number(string);
		void connect_audio(string, string);
	
	private:
	
		vector<jack_out> jack_outs;
		vector<jack_in> jack_ins;
		vector<jack_client> jack_clients;

};

//--------------------------------------------------






