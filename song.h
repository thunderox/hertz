
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef struct
{
	int top;
	int bottom;
} struct_time_signature;

//-------------------------------------------------------------------------------

class song

{
	public:
	song();
	~song();
	
	void set_name(string);
	string get_name();
	
	private:
	
	string name;
	struct_time_signature time_signature;
	int ppq;
	int tempo;
	
};

