#include "Message.h"

string Message::_logFilePath = string();
int Message::_consoleBuffer = 0;

Message::Message()
{

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
	std::fstream fs; // create a file stream object

	fs.open(Message::_logFilePath.c_str(), std::fstream::app); // open the shell file for read write and append

	fs << this->_message << endl; // output song path to file

	fs.close();
	fs.clear(std::ios_base::goodbit);
}

void Message::PrintToConsole()
{
	cout << this->_message << endl;
}

Message::~Message()
{

}
