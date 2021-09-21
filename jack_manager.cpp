
#include "jack_manager.h"

jack_manager* _jackmanager;

//--------------------------------------------------------------------------------------------------
// CONSTRUCTOR

jack_manager::jack_manager()
{
}

//--------------------------------------------------------------------------------------------------
// DECONSTRUCTOR

jack_manager::~jack_manager()
{
}

//--------------------------------------------------------------------------------------------------
// SET CURRENT JACK MANAGER
void jack_manager::set_current_manager(jack_manager* current_jack_manager)
{
	_jackmanager = current_jack_manager;
}

//--------------------------------------------------------------------------------------------------
// CREATE CLIENT

bool jack_manager::create_client(string client_name)
{
	jack_client new_jack_client;

	cout << "Opening jack midi client '" << client_name << "'........";

	if ((new_jack_client.client = jack_client_open (client_name.c_str(), JackNullOption, NULL)) == 0)
	{
        	cout << "FAIL - jack server not running?" << endl;
        	return false;
	}

	new_jack_client.name = client_name;
	jack_clients.push_back(new_jack_client);

	cout << "OK" << endl;

	jack_set_process_callback (new_jack_client.client, process, 0);
	jack_set_sample_rate_callback (new_jack_client.client, cb_srate, 0);

	if (jack_activate(new_jack_client.client) )
	{
	        cout << "cannot activate client" << endl;
		return 1;
	}

	return true;
}

//===================================================================
// OPEN NEW MIDI PORT

int jack_manager::create_jack_out(string client_name,string port_name)
{
	cout << "Opening jack midi port '" << port_name << "'........";

	int client_number = get_client_number(client_name);

	if (client_number==-1)
	{
		cout << "FAIL - Cannot find a client named '" << client_name << "'" << endl;
		return false;
	}

	jack_client_t *client = jack_clients[client_number].client;

	jack_out new_jack_out;
	new_jack_out.name = port_name;
	stringstream name;

	name << port_name  << "_" << "out";

	new_jack_out.output_port = jack_port_register (client, name.str().c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);

	name.str("");
	name << port_name << "_" << "audio_out_left";

	new_jack_out.audio_out_left = jack_port_register (client, name.str().c_str(),
                     JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	name.str("");
	name << port_name << "_" << "audio_out_right";

	new_jack_out.audio_out_right = jack_port_register (client, name.str().c_str(),
                     JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	jack_outs.push_back(new_jack_out);
	cout << "OK" << endl;

	return true;
}

// -------------------------------------------------------------------------------------------------------
// CONNECT MIDI PORTS
void jack_manager::connect_audio(string client_name, string port_name)
{
	int client_number = get_client_number(client_name);

	const char **ports;
	
	ports = jack_get_ports (jack_clients[client_number].client, NULL, NULL,JackPortIsPhysical|JackPortIsInput);

	if (ports == NULL)
	{
		cout << "No physical capture ports." << endl;
		return;
	}
	
	if (jack_connect ( jack_clients[client_number].client, jack_port_name ( jack_outs[0].audio_out_left), ports[0]))
	{
		cout << "Cannot connect input ports - '" << jack_port_name (jack_outs[0].audio_out_left) << "' to '" << ports[0] << "'" << endl;
	}

	if (jack_connect ( jack_clients[client_number].client, jack_port_name ( jack_outs[0].audio_out_right), ports[1]))
	{
		cout << "Cannot connect input ports - '" << jack_port_name (jack_outs[0].audio_out_right) << "' to '" << ports[1] << "'" << endl;
	}

	return;
}

// -------------------------------------------------------------------------------------------------------
// CLOSE CLIENT

void jack_manager::close_client(string client_name)
{
	cout << "Closing jack midi client '" << client_name << "'........";

	int client_number = get_client_number(client_name);
	jack_client_close (jack_clients[client_number].client);
	jack_clients.erase (jack_clients.begin()+client_number);
	cout << endl;
	return;
}


// -------------------------------------------------------------------------------------------------------
// FIND CLIMATE NUMBER ID BY NAME

int jack_manager::get_client_number(string name)
{
	int client_number = -1;

	for(unsigned int x=0; x<jack_clients.size(); x++)
	{
		if (name==jack_clients[x].name) { client_number = x;  break; }
	}

	return client_number;
}


//--------------------------------------------------------------------------------------------------
// JACK PROCESS

int process(jack_nframes_t nframes, void *arg)
{

	jack_port_t *input_port;	

	vector <void*> port_buffers;

	vector <jack_default_audio_sample_t*> audio_buffers_left;
	vector <jack_default_audio_sample_t*> audio_buffers_right;

	for (int port=0; port<_jackmanager->jack_outs.size(); ++port)
	{
		void* port_buf = jack_port_get_buffer(_jackmanager->jack_outs[port].output_port, nframes);
		jack_midi_clear_buffer(port_buf);
		port_buffers.push_back(port_buf);	
			
		jack_default_audio_sample_t* audio_buf_left = (jack_default_audio_sample_t*)
			jack_port_get_buffer(_jackmanager->jack_outs[port].audio_out_left, nframes);

		jack_default_audio_sample_t* audio_buf_right = (jack_default_audio_sample_t*)
			jack_port_get_buffer(_jackmanager->jack_outs[port].audio_out_right, nframes);

		memset( audio_buf_left, 0, sizeof(double)*(nframes*0.5) );
		memset( audio_buf_right, 0, sizeof(double)*(nframes*0.5) );

		audio_buffers_left.push_back(audio_buf_left);
		audio_buffers_right.push_back(audio_buf_right);


	}

	return 0;
}

//-------------------------------------------------------------
// SAMPLE RATE CHANGE

int cb_srate (jack_nframes_t _sample_rate, void *arg)
{
	cout << "Sample rate changed to " << _sample_rate << endl;
	_jackmanager->sample_rate = _sample_rate;
	
	return 0;
}






