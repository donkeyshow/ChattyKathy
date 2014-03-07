#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS // this disables secure warning errors.  it was easier than finding another way to write the time code
#endif

#include "Message.h"

string Message::_logFilePath = string(); // initialize static members
int Message::_consoleBuffer = 0;

Message::Message()
{
	// default constructor does nothing
}

const string Message::GetChatMessage()
{
	return _message;
}

void Message::SetMessage( string message )
{
	_message = message;
}

void Message::AppendToLog()
{
	std::stringstream stringBuffer; // create a stringstream object to insert time
	std::fstream fs; // create a file stream object
	string logString, tempString; // create a string object to get stringstream data

	using std::chrono::system_clock; // make system_clock available for use
	std::time_t tt = system_clock::to_time_t(system_clock::now()); // get now and put it in a time_t object
	struct tm* newtime = std::localtime(&tt); // create a tm object and store the localtime tt in it

	stringBuffer << std::put_time(newtime, "%H:%M:%S") << " " << std::put_time(newtime, "%x"); // put the local time in the string stream
	stringBuffer >> logString >> tempString; // insert the stringstream into logString
	logString.append(" "); // add a space
	logString.append(tempString);
	logString.append(" ");	
	logString.append(this->_message); // add the message
	
	fs.open(Message::_logFilePath.c_str(), std::fstream::app); // open the shell file for append

	fs << logString.c_str() << endl; // output song path to file

	fs.close();
	fs.clear(std::ios_base::goodbit);
}

void Message::PrintToConsole()
{
	cout << this->_message << endl; // output current message
}

Message::~Message()
{
	// default destructor does nothing
}
